#!/usr/bin/env python3
"""
ELF Relocation and Symbol Cross-Reference Analyzer
Correlates code relocations with .rodata strings, vtables, and library calls
to name functions and discover class boundaries in Splatoon 1 (Gambit).
"""

import struct
from pathlib import Path
from collections import defaultdict

def analyze():
    elf_path = Path("original/Gambit.elf")
    if not elf_path.exists():
        print(f"[!] {elf_path} not found.")
        return

    with open(elf_path, "rb") as f:
        hdr = f.read(52)
        e_shoff = struct.unpack(">I", hdr[32:36])[0]
        e_shentsize, e_shnum, e_shstrndx = struct.unpack(">HHH", hdr[46:52])
        f.seek(e_shoff)
        shdrs = [struct.unpack(">IIIIIIIIII", f.read(40)) for _ in range(e_shnum)]
        
        sh_str = shdrs[e_shstrndx]
        f.seek(sh_str[4])
        shstrtab = f.read(sh_str[5])
        
        sections = {}
        for sh in shdrs:
            name = shstrtab[sh[0]:].split(b"\0")[0].decode("latin1")
            sections[name] = sh

        # Read symbols
        symtab_sh = sections[".symtab"]
        strtab_sh = sections[".strtab"]
        f.seek(strtab_sh[4])
        strtab = f.read(strtab_sh[5])
        
        f.seek(symtab_sh[4])
        symbols = []
        for _ in range(symtab_sh[5] // 16):
            st_name, st_value, st_size, st_info, st_other, st_shndx = struct.unpack(">IIIBBH", f.read(16))
            sym_name = strtab[st_name:].split(b"\0")[0].decode("latin1")
            symbols.append((st_value, sym_name))

        # Read .rodata
        rodata_sh = sections[".rodata"]
        f.seek(rodata_sh[4])
        rodata = f.read(rodata_sh[5])
        rodata_base = rodata_sh[3]
        rodata_end = rodata_base + rodata_sh[5]
        
        # Read .rela.text
        rela_text_sh = sections[".rela.text"]
        f.seek(rela_text_sh[4])
        rela_data = f.read(rela_text_sh[5])
        num_relas = len(rela_data) // 12
        
        print(f"[*] Analyzing {num_relas:,} code relocations in .rela.text...")
        
        code_to_strings = []
        string_to_code = defaultdict(list)
        
        for i in range(0, len(rela_data), 12):
            r_offset, r_info, r_addend = struct.unpack(">III", rela_data[i:i+12])
            sym_idx = r_info >> 8
            if sym_idx < len(symbols):
                sym_val, _ = symbols[sym_idx]
                target_addr = sym_val + r_addend
                if rodata_base <= target_addr < rodata_end:
                    off = target_addr - rodata_base
                    zero = rodata.find(b"\0", off)
                    if zero != -1 and (zero - off) >= 3:
                        raw = rodata[off:zero]
                        # Only take printable strings
                        if all(0x20 <= b <= 0x7E for b in raw):
                            try:
                                s = raw.decode("ascii")
                                code_to_strings.append((r_offset, target_addr, s))
                                string_to_code[s].append(r_offset)
                            except UnicodeDecodeError:
                                pass
                                
        print(f"[+] Total code string cross-references discovered: {len(code_to_strings):,}")
        print(f"[+] Unique referenced strings: {len(string_to_code):,}")

        # Extract file references to identify functions
        file_refs = [entry for entry in code_to_strings if ".cpp" in entry[2] or ".h" in entry[2]]
        print(f"[+] Found {len(file_refs):,} source file references directly embedded in code!")

        # Write detailed cross-reference dump
        out_dir = Path("tools/extracted")
        out_dir.mkdir(parents=True, exist_ok=True)
        
        with open(out_dir / "code_to_strings.txt", "w", encoding="utf-8") as out:
            for code_addr, target_addr, s in code_to_strings:
                out.write(f"0x{code_addr:08X} -> 0x{target_addr:08X} : {s}\n")
                
        with open(out_dir / "source_file_xrefs.txt", "w", encoding="utf-8") as out:
            for code_addr, target_addr, s in file_refs:
                out.write(f"0x{code_addr:08X} -> 0x{target_addr:08X} : {s}\n")

        print(f"[+] Written to {out_dir / 'code_to_strings.txt'} and {out_dir / 'source_file_xrefs.txt'}")
        print("\nSample Source File Xrefs in Code:")
        for code_addr, target_addr, s in file_refs[:20]:
            print(f"  Code 0x{code_addr:08X} references: {s}")

if __name__ == "__main__":
    analyze()
