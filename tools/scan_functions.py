#!/usr/bin/env python3
"""
Master Function Boundary and Symbol Cataloger for Splatoon 1 (Gambit)
Scans .text for PowerPC function entries (prologues, bl branches, relocations),
associates each function with strings, assertions, and source files, and produces
tools/database/functions.json.
"""

import struct
import json
import re
from pathlib import Path
from collections import defaultdict

def scan_functions():
    elf_path = Path("original/Gambit.elf")
    if not elf_path.exists():
        print(f"[!] {elf_path} not found.")
        return

    print("[*] Parsing ELF header and section table...")
    with open(elf_path, "rb") as f:
        hdr = f.read(52)
        e_entry = struct.unpack(">I", hdr[24:28])[0]
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
            
        text_sh = sections[".text"]
        text_addr = text_sh[3]
        text_size = text_sh[5]
        text_end = text_addr + text_size
        f.seek(text_sh[4])
        text_data = f.read(text_size)

        # Read relocations pointing into .text (vtable entries, function pointers)
        print("[*] Parsing relocations pointing to function pointers...")
        rela_text_sh = sections[".rela.text"]
        f.seek(rela_text_sh[4])
        rela_text_data = f.read(rela_text_sh[5])
        
        rela_rodata_sh = sections[".rela.rodata"]
        f.seek(rela_rodata_sh[4])
        rela_rodata_data = f.read(rela_rodata_sh[5])

    # Collect entry points from multiple heuristics
    entry_points = set()
    entry_points.add(e_entry)

    # 1. Relocations pointing to code
    for r_data in (rela_text_data, rela_rodata_data):
        for i in range(0, len(r_data), 12):
            r_offset, r_info, r_addend = struct.unpack(">III", r_data[i:i+12])
            sym_idx = r_info >> 8
            # In our symbol table, sym[1] is $TEXT (0x02000000)
            if sym_idx == 1:
                target = 0x02000000 + r_addend
                if text_addr <= target < text_end:
                    entry_points.add(target)

    # 2. PowerPC instruction heuristics (bl targets and stack prologues)
    print("[*] Scanning 14.5 MB of PowerPC instructions...")
    for i in range(0, text_size, 4):
        instr = struct.unpack(">I", text_data[i:i+4])[0]
        pc = text_addr + i

        # bl instruction: opcode 18 (0x48000000), LK=1
        if (instr & 0xFC000003) == 0x48000001:
            disp = instr & 0x03FFFFFC
            if disp & 0x02000000:
                disp -= 0x04000000
            target = pc + disp
            if text_addr <= target < text_end:
                entry_points.add(target)

        # stwu r1, -imm(r1) prologue
        if (instr & 0xFFFF0000) == 0x94210000:
            imm = instr & 0xFFFF
            if imm >= 0x8000:
                # Often the first instruction, or preceded by mflr r0 (0x7C0802A6)
                if i >= 4:
                    prev_instr = struct.unpack(">I", text_data[i-4:i])[0]
                    if prev_instr == 0x7C0802A6:
                        entry_points.add(pc - 4)
                    else:
                        entry_points.add(pc)
                else:
                    entry_points.add(pc)

    sorted_entries = sorted(entry_points)
    print(f"[+] Total unique function entries discovered: {len(sorted_entries):,}")

    # Load string xrefs from Phase 1
    xrefs_file = Path("tools/extracted/code_to_strings.txt")
    code_strings = defaultdict(list)
    if xrefs_file.exists():
        with open(xrefs_file, "r", encoding="utf-8") as f:
            for line in f:
                parts = line.strip().split(" : ")
                if len(parts) == 2:
                    c_addr = int(parts[0].split(" -> ")[0], 16)
                    code_strings[c_addr].append(parts[1])

    # Load known source file assertions
    source_file_xrefs = {}
    src_file = Path("tools/extracted/source_file_xrefs.txt")
    if src_file.exists():
        with open(src_file, "r", encoding="utf-8") as f:
            for line in f:
                parts = line.strip().split(" : ")
                if len(parts) == 2:
                    c_addr = int(parts[0].split(" -> ")[0], 16)
                    source_file_xrefs[c_addr] = parts[1]

    # Map each function to its span, strings, and source file
    functions = []
    print("[*] Correlating functions with string xrefs and source files...")
    
    # Pre-index sorted string xref addresses
    string_addresses = sorted(code_strings.keys())
    str_idx = 0

    current_source_file = "Unknown.cpp"

    for idx, fn_addr in enumerate(sorted_entries):
        next_addr = sorted_entries[idx + 1] if idx + 1 < len(sorted_entries) else text_end
        fn_size = next_addr - fn_addr

        # Collect strings in this function
        fn_strings = []
        while str_idx < len(string_addresses) and string_addresses[str_idx] < next_addr:
            s_addr = string_addresses[str_idx]
            if s_addr >= fn_addr:
                fn_strings.extend(code_strings[s_addr])
                if s_addr in source_file_xrefs:
                    current_source_file = source_file_xrefs[s_addr].split("/")[-1]
            str_idx += 1

        # Heuristic name assignment
        fn_name = f"fn_{fn_addr:08X}"
        for s in fn_strings:
            if s.startswith("StateID::"):
                fn_name = "Player_State_" + s.replace("StateID::", "")
                break
            elif s.startswith("StateId::"):
                fn_name = "State_" + s.replace("StateId::", "")
                break
            elif "::" in s and not s.startswith("D:"):
                # Method signature string
                clean = re.sub(r"[^A-Za-z0-9_:]", "_", s)
                if len(clean) < 40:
                    fn_name = clean
                    break

        functions.append({
            "address": f"0x{fn_addr:08X}",
            "int_address": fn_addr,
            "size": fn_size,
            "name": fn_name,
            "source_file": current_source_file,
            "strings": fn_strings[:5] # keep top 5 strings for index compactness
        })

    # Save to database
    out_dir = Path("tools/database")
    out_dir.mkdir(parents=True, exist_ok=True)
    
    out_json = out_dir / "functions.json"
    print(f"[*] Saving master function database to {out_json}...")
    with open(out_json, "w", encoding="utf-8") as f:
        json.dump(functions, f, indent=2)

    # Generate summary report
    out_summary = out_dir / "functions_summary.txt"
    named_count = sum(1 for f in functions if not f["name"].startswith("fn_"))
    with open(out_summary, "w", encoding="utf-8") as f:
        f.write("Splatoon 1 (Gambit) Function Catalog Summary\n")
        f.write("=" * 60 + "\n")
        f.write(f"Total Functions Cataloged: {len(functions):,}\n")
        f.write(f"Directly Named Functions : {named_count:,}\n")
        f.write(f"Total Code Coverage      : {sum(f['size'] for f in functions):,} bytes\n\n")
        f.write("Sample Cataloged Functions:\n")
        for fn in functions[:50]:
            f.write(f"  {fn['address']} ({fn['size']:4d}B) [{fn['source_file']}] : {fn['name']}\n")

    print(f"[+] Successfully cataloged {len(functions):,} functions ({named_count:,} named) in {out_json}!")

if __name__ == "__main__":
    scan_functions()
