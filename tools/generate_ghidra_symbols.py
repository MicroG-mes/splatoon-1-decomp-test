#!/usr/bin/env python3
"""
Ghidra Symbol Script Generator for Splatoon 1 (Gambit)
Scans code relocations and strings, generates a Python script to run inside Ghidra's
Script Manager that automatically renames functions, sets labels, and creates namespaces.
"""

from pathlib import Path
import re

def generate_script():
    xrefs_file = Path("tools/extracted/code_to_strings.txt")
    if not xrefs_file.exists():
        print(f"[!] {xrefs_file} not found.")
        return

    # Address -> Name mapping
    symbol_map = {}

    with open(xrefs_file, "r", encoding="utf-8") as f:
        for line in f:
            parts = line.strip().split(" : ")
            if len(parts) == 2:
                addr = int(parts[0].split(" -> ")[0], 16)
                s = parts[1]

                # Match StateIDs (Human/Squid actions)
                if s.startswith("StateID::"):
                    name = "Player_" + s.replace("StateID::", "State_")
                    symbol_map[addr] = name
                elif s.startswith("StateId::"):
                    name = "State_" + s.replace("StateId::", "")
                    symbol_map[addr] = name
                # Match Manager singletons
                elif s.endswith("Mgr") and len(s) < 30:
                    symbol_map[addr] = f"g_{s}_Ref"
                # Match source filenames
                elif s.endswith(".cpp"):
                    cls = s.replace(".cpp", "").split("/")[-1]
                    symbol_map[addr] = f"{cls}_AssertString"

    out_file = Path("tools/import_symbols_to_ghidra.py")
    with open(out_file, "w", encoding="utf-8") as out:
        out.write("""# Auto-generated Splatoon (Gambit) Symbol Importer for Ghidra
# Run this inside Ghidra's Script Manager (Window -> Script Manager -> Run Script)
# @category SplatoonDecomp

from ghidra.program.model.symbol import SourceType

SYMBOLS = [
""")
        for addr, name in sorted(symbol_map.items()):
            # Sanitise name for Ghidra symbol
            sanitized = re.sub(r"[^A-Za-z0-9_:]", "_", name)
            out.write(f"    (0x{addr:08X}, \"{sanitized}\"),\n")
        
        out.write("""
]

def run():
    af = currentProgram.getAddressFactory()
    st = currentProgram.getSymbolTable()
    fm = currentProgram.getFunctionManager()
    
    count = 0
    for addr_val, name in SYMBOLS:
        addr = af.getAddress(hex(addr_val).rstrip('L'))
        if addr is not None:
            # Check if there is a function containing this address
            func = fm.getFunctionContaining(addr)
            if func is not None and func.getName().startswith("FUN_"):
                func.setName(name, SourceType.USER_DEFINED)
                count += 1
            else:
                st.createLabel(addr, name, SourceType.USER_DEFINED)
                count += 1
                
    print("[+] Successfully imported {} Splatoon symbols into Ghidra!".format(count))

run()
""")

    print(f"[+] Generated Ghidra symbol script with {len(symbol_map):,} symbols at: {out_file}")

if __name__ == "__main__":
    generate_script()
