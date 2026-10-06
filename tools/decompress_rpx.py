#!/usr/bin/env python3
"""
RPX to ELF Decompressor
Decompresses Wii U RPX files (.rpx) into standard 32-bit Big Endian PowerPC ELF files (.elf).
Wii U RPX files are ELF files with zlib-compressed sections (marked by SHF_RPX_COMPRESSED 0x08000000).
"""

import sys
import struct
import zlib
from pathlib import Path

SHF_RPX_COMPRESSED = 0x08000000

def decompress_rpx(rpx_path, out_elf_path):
    print(f"[*] Reading RPX: {rpx_path}")
    with open(rpx_path, "rb") as f:
        rpx_data = f.read()

    if len(rpx_data) < 52:
        raise ValueError("File is too small to be an ELF/RPX.")

    magic = rpx_data[:4]
    if magic != b"\x7fELF":
        raise ValueError(f"Invalid ELF magic: {magic}")

    # Parse 32-bit Big Endian ELF Header
    (
        e_type, e_machine, e_version, e_entry,
        e_phoff, e_shoff, e_flags, e_ehsize,
        e_phentsize, e_phnum, e_shentsize, e_shnum, e_shstrndx
    ) = struct.unpack(">HHIIIIIHHHHHH", rpx_data[16:52])

    if e_machine != 20: # EM_PPC = 20
        print(f"[!] Warning: e_machine is {e_machine}, expected 20 (PowerPC).")

    # Read Program Headers
    ph_data = b""
    if e_phnum > 0 and e_phoff > 0:
        ph_end = e_phoff + (e_phnum * e_phentsize)
        ph_data = rpx_data[e_phoff:ph_end]

    # Read Section Headers
    shdrs = []
    for i in range(e_shnum):
        offset = e_shoff + (i * e_shentsize)
        shdr = list(struct.unpack(">IIIIIIIIII", rpx_data[offset:offset + 40]))
        shdrs.append(shdr)

    # Process and decompress sections
    out_sections = []
    for i, sh in enumerate(shdrs):
        name_idx, sh_type, sh_flags, sh_addr, sh_offset, sh_size, sh_link, sh_info, sh_addralign, sh_entsize = sh

        if sh_type == 8: # SHT_NOBITS (.bss)
            out_sections.append(b"")
            continue

        if sh_size == 0:
            out_sections.append(b"")
            continue

        raw_sec = rpx_data[sh_offset:sh_offset + sh_size]

        if sh_flags & SHF_RPX_COMPRESSED:
            expected_size = struct.unpack(">I", raw_sec[:4])[0]
            decomp = zlib.decompress(raw_sec[4:])
            if len(decomp) != expected_size:
                print(f"[!] Warning: section {i} size mismatch: got {len(decomp)}, expected {expected_size}")
            sh[2] = sh_flags & ~SHF_RPX_COMPRESSED # Clear compressed flag
            sh[5] = len(decomp) # Update sh_size
            out_sections.append(decomp)
        else:
            out_sections.append(raw_sec)

    # Reconstruct ELF
    # Structure:
    # 0..52: ELF Header
    # 52..52+len(ph_data): Program Headers
    # Then section data (aligned according to sh_addralign)
    # Finally Section Header Table

    current_offset = 52 + len(ph_data)
    # Align to 64 bytes
    current_offset = (current_offset + 63) & ~63

    section_bytes = bytearray()
    base_file_offset = current_offset

    for i, sh in enumerate(shdrs):
        data = out_sections[i]
        align = sh[8] if sh[8] > 0 else 4
        
        # SHT_NOBITS (.bss) doesn't take file space
        if sh[1] == 8:
            continue

        if len(data) == 0:
            continue

        # Align file offset
        pad = (align - (current_offset % align)) % align
        if pad > 0:
            section_bytes.extend(b"\x00" * pad)
            current_offset += pad

        sh[4] = current_offset # Update sh_offset
        section_bytes.extend(data)
        current_offset += len(data)

    # Align before section headers table
    pad = (16 - (current_offset % 16)) % 16
    if pad > 0:
        section_bytes.extend(b"\x00" * pad)
        current_offset += pad

    new_shoff = current_offset

    # Serialize updated section headers
    sh_table = bytearray()
    for sh in shdrs:
        sh_table.extend(struct.pack(">IIIIIIIIII", *sh))

    # Build new ELF header
    new_hdr = bytearray(rpx_data[:52])
    struct.pack_into(">I", new_hdr, 32, new_shoff) # Update e_shoff

    out_elf_bytes = new_hdr + ph_data
    # Pad to base_file_offset
    pad_to_base = base_file_offset - len(out_elf_bytes)
    if pad_to_base > 0:
        out_elf_bytes += b"\x00" * pad_to_base

    out_elf_bytes += section_bytes + sh_table

    print(f"[*] Writing decompressed ELF to: {out_elf_path}")
    with open(out_elf_path, "wb") as f:
        f.write(out_elf_bytes)

    print(f"[+] Successfully generated: {out_elf_path} ({len(out_elf_bytes):,} bytes)")

def main():
    root = Path(__file__).resolve().parent.parent
    rpx = root / "original" / "Gambit.rpx"
    elf = root / "original" / "Gambit.elf"

    if len(sys.argv) > 1:
        rpx = Path(sys.argv[1])
    if len(sys.argv) > 2:
        elf = Path(sys.argv[2])

    if not rpx.exists():
        print(f"[!] RPX not found at: {rpx}")
        sys.exit(1)

    decompress_rpx(rpx, elf)

if __name__ == "__main__":
    main()
