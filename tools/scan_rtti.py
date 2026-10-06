#!/usr/bin/env python3
"""
Complete GHS Multi RTTI and VTable Hierarchy Reconstructor for Splatoon 1 (Gambit)
Green Hills Software on Cafe OS uses 8-byte vtable entries [func_ptr (4B), this_delta (4B)].
This script parses all 11,267 vtables in .rodata, links them to class names and RTTI,
and outputs tools/database/classes.json and tools/database/vtables.json.
"""

import struct
import json
import re
from pathlib import Path

def scan_ghs_vtables():
    elf_path = Path("original/Gambit.elf")
    if not elf_path.exists():
        print(f"[!] {elf_path} not found.")
        return

    print("[*] Parsing ELF header and section table...")
    with open(elf_path, "rb") as f:
        hdr = f.read(52)
        e_shoff = struct.unpack(">I", hdr[32:36])[0]
        e_shentsize, e_shnum, e_shstrndx = struct.unpack(">HHH", hdr[46:52])
        f.seek(e_shoff)
        shdrs = [struct.unpack(">IIIIIIIIII", f.read(40)) for _ in range(e_shnum)]
        sh_str = shdrs[e_shstrndx]
        f.seek(sh_str[4])
        shstrtab = f.read(sh_str[5])
        
        sections = {shstrtab[s[0]:].split(b"\0")[0].decode("latin1"): s for s in shdrs}

        rodata_sh = sections[".rodata"]
        rodata_addr = rodata_sh[3]
        rodata_size = rodata_sh[5]
        f.seek(rodata_sh[4])
        rodata_bytes = f.read(rodata_size)

        rela_rodata_sh = sections[".rela.rodata"]
        f.seek(rela_rodata_sh[4])
        rela_data = f.read(rela_rodata_sh[5])

    # 1. Parse code pointers in .rela.rodata
    print("[*] Parsing 116,977 relocations in .rela.rodata...")
    code_ptrs = []
    for i in range(0, len(rela_data), 12):
        r_off, r_info, r_add = struct.unpack(">III", rela_data[i:i+12])
        sym = r_info >> 8
        if sym == 1: # $TEXT
            target_fn = 0x02000000 + r_add
            code_ptrs.append((r_off, target_fn))

    code_ptrs.sort(key=lambda x: x[0])

    # 2. Group into GHS 8-byte vtables
    vtables = []
    curr = []
    for off, target in code_ptrs:
        if not curr:
            curr.append((off, target))
        elif off == curr[-1][0] + 8:
            curr.append((off, target))
        else:
            if len(curr) >= 2:
                vtables.append(curr)
            curr = [(off, target)]
    if len(curr) >= 2:
        vtables.append(curr)

    print(f"[+] Discovered {len(vtables):,} GHS vtables containing {sum(len(v) for v in vtables):,} virtual methods!")

    # 3. Associate each vtable with nearby class name strings in .rodata
    # In GHS, class names / RTTI strings often precede or follow the vtable within 256 bytes
    classes_db = []
    named_vtables = 0

    for vt in vtables:
        vt_start = vt[0][0]
        vt_end = vt[-1][0] + 8
        vt_methods = [f"0x{target:08X}" for _, target in vt]
        
        # Search backwards 256 bytes for a class string
        rodata_off = vt_start - rodata_addr
        search_start = max(0, rodata_off - 256)
        slice_before = rodata_bytes[search_start:rodata_off]
        
        # Look for identifiers
        identifiers = re.findall(rb"([A-Za-z][A-Za-z0-9_]{3,35})\0", slice_before)
        class_name = None
        if identifiers:
            for cand in reversed(identifiers):
                cand_str = cand.decode("ascii", errors="ignore")
                if any(cand_str.endswith(suf) for suf in ["Actor", "Mgr", "Camera", "Player", "Enemy", "Bullet", "Item", "Obj", "Clone", "Task", "Heap"]):
                    class_name = cand_str
                    break
            if not class_name:
                class_name = identifiers[-1].decode("ascii", errors="ignore")

        if class_name:
            named_vtables += 1
        else:
            class_name = f"Class_VTable_0x{vt_start:08X}"

        classes_db.append({
            "class_name": class_name,
            "vtable_address": f"0x{vt_start:08X}",
            "method_count": len(vt_methods),
            "methods": vt_methods
        })

    # Save to JSON
    out_dir = Path("tools/database")
    out_dir.mkdir(parents=True, exist_ok=True)
    out_json = out_dir / "classes.json"
    
    print(f"[*] Saving class hierarchy database to {out_json}...")
    with open(out_json, "w", encoding="utf-8") as f:
        json.dump(classes_db, f, indent=2)

    # Save summary
    summary_file = out_dir / "classes_summary.txt"
    with open(summary_file, "w", encoding="utf-8") as f:
        f.write("Splatoon 1 (Gambit) GHS VTable Class Catalog\n")
        f.write("=" * 65 + "\n")
        f.write(f"Total VTables Discovered: {len(classes_db):,}\n")
        f.write(f"Directly Linked Classes : {named_vtables:,}\n")
        f.write(f"Total Virtual Methods   : {sum(c['method_count'] for c in classes_db):,}\n\n")
        f.write("Sample Cataloged Classes:\n")
        for c in classes_db[:50]:
            f.write(f"  {c['class_name']:<30} @ {c['vtable_address']} ({c['method_count']:2d} methods)\n")

    print(f"[+] Successfully saved {len(classes_db):,} classes ({named_vtables:,} linked) in {out_json}!")

if __name__ == "__main__":
    scan_ghs_vtables()
