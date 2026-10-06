#!/usr/bin/env python3
"""
Master Ghidra & IDA Symbol Generator for Splatoon 1 (Gambit)
Merges functions.json and classes.json to create complete automation scripts:
- tools/import_symbols_to_ghidra.py (Ghidra script)
- tools/import_symbols_to_ida.py (IDA Pro script)
"""

import json
from pathlib import Path
import re

def generate_scripts():
    funcs_file = Path("tools/database/functions.json")
    classes_file = Path("tools/database/classes.json")

    symbols = {} # addr_int -> name

    # 1. Add named functions from functions.json
    if funcs_file.exists():
        with open(funcs_file, "r", encoding="utf-8") as f:
            funcs = json.load(f)
            for fn in funcs:
                if not fn["name"].startswith("fn_"):
                    addr = fn["int_address"]
                    symbols[addr] = fn["name"]

    # 2. Add vtables and their virtual methods from classes.json
    if classes_file.exists():
        with open(classes_file, "r", encoding="utf-8") as f:
            classes = json.load(f)
            for c in classes:
                vt_addr = int(c["vtable_address"], 16)
                cls_name = re.sub(r"[^A-Za-z0-9_]", "_", c["class_name"])
                symbols[vt_addr] = f"vtable_{cls_name}"

                # Label virtual methods
                for idx, vfunc in enumerate(c["methods"]):
                    vf_addr = int(vfunc, 16)
                    if vf_addr not in symbols:
                        symbols[vf_addr] = f"{cls_name}__vfunc_{idx}"

    print(f"[*] Aggregated {len(symbols):,} unique named symbols!")

    # 3. Generate Ghidra Script
    ghidra_out = Path("tools/import_symbols_to_ghidra.py")
    with open(ghidra_out, "w", encoding="utf-8") as out:
        out.write("""# Auto-generated Splatoon 1 (Gambit) Symbol Importer for Ghidra
# Run in Ghidra: Window -> Script Manager -> Run Script
# @category SplatoonDecomp

from ghidra.program.model.symbol import SourceType

SYMBOLS = [
""")
        for addr, name in sorted(symbols.items()):
            clean_name = re.sub(r"[^A-Za-z0-9_:]", "_", name)
            out.write(f"    (0x{addr:08X}, \"{clean_name}\"),\n")
        out.write("""
]

def run():
    af = currentProgram.getAddressFactory()
    st = currentProgram.getSymbolTable()
    fm = currentProgram.getFunctionManager()
    
    print("[*] Starting bulk symbol import into Ghidra...")
    count = 0
    for addr_val, name in SYMBOLS:
        addr = af.getAddress(hex(addr_val).rstrip('L'))
        if addr is not None:
            func = fm.getFunctionContaining(addr)
            if func is not None and (func.getName().startswith("FUN_") or func.getName().startswith("fn_")):
                func.setName(name, SourceType.USER_DEFINED)
                count += 1
            else:
                st.createLabel(addr, name, SourceType.USER_DEFINED)
                count += 1
                
    print("[+] Successfully imported {} Splatoon symbols into Ghidra!".format(count))

run()
""")

    # 4. Generate IDA Pro Script
    ida_out = Path("tools/import_symbols_to_ida.py")
    with open(ida_out, "w", encoding="utf-8") as out:
        out.write("""# Auto-generated Splatoon 1 (Gambit) Symbol Importer for IDA Pro
# Run in IDA Pro: File -> Script File...

import ida_name
import ida_funcs
import ida_ida

SYMBOLS = [
""")
        for addr, name in sorted(symbols.items()):
            clean_name = re.sub(r"[^A-Za-z0-9_]", "_", name)
            out.write(f"    (0x{addr:08X}, \"{clean_name}\"),\n")
        out.write("""
]

def run():
    print("[*] Importing symbols into IDA Pro...")
    count = 0
    for addr, name in SYMBOLS:
        ida_name.set_name(addr, name, ida_name.SN_NOWARN)
        count += 1
    print("[+] Successfully imported {} symbols into IDA Pro!".format(count))

run()
""")

    print(f"[+] Written Ghidra script to: {ghidra_out}")
    print(f"[+] Written IDA Pro script to: {ida_out}")

if __name__ == "__main__":
    generate_scripts()
