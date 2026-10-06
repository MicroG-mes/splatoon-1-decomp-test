#!/usr/bin/env python3
"""
Splatoon 1 (Gambit) Decompilation Progress Calculator
Analyzes progress against total cataloged functions and code bytes in original/Gambit.elf.
"""

import json
from pathlib import Path
import re

def calc_progress():
    funcs_file = Path("tools/database/functions.json")
    classes_file = Path("tools/database/classes.json")
    src_dir = Path("src")

    if not funcs_file.exists():
        print(f"[!] {funcs_file} not found. Run tools/scan_functions.py first.")
        return

    with open(funcs_file, "r", encoding="utf-8") as f:
        all_funcs = json.load(f)

    total_funcs = len(all_funcs)
    total_code_bytes = sum(fn["size"] for fn in all_funcs)

    # Count reconstructed source files and methods
    src_files = list(src_dir.rglob("*.cpp"))
    reconstructed_methods = 0
    reconstructed_bytes = 0

    method_regex = re.compile(r"^\w[\w:<>\*\&\s]+\b(\w+::\w+)\s*\(", re.MULTILINE)

    for src in src_files:
        content = src.read_text(encoding="utf-8", errors="ignore")
        matches = method_regex.findall(content)
        reconstructed_methods += len(matches)

    # Match reconstructed functions against functions database
    named_funcs = sum(1 for fn in all_funcs if not fn["name"].startswith("fn_"))

    print("\n" + "=" * 65)
    print(" [Splatoon 1 (Wii U) Decompilation Progress]")
    print("=" * 65)
    print(f"Total Cataloged Functions : {total_funcs:,}")
    print(f"Total .text Code Size      : {total_code_bytes:,} bytes ({total_code_bytes / (1024*1024):.2f} MB)")
    print(f"Directly Identified Funcs : {named_funcs:,} ({named_funcs / total_funcs * 100:.2f}%)")
    print(f"Active Source Files (.cpp): {len(src_files):,}")
    print(f"Reconstructed C++ Methods : {reconstructed_methods:,}")
    print("=" * 65)

    stats = {
        "total_functions": total_funcs,
        "total_code_bytes": total_code_bytes,
        "named_functions": named_funcs,
        "source_files": len(src_files),
        "reconstructed_methods": reconstructed_methods
    }

    out_file = Path("tools/progress.json")
    with open(out_file, "w", encoding="utf-8") as f:
        json.dump(stats, f, indent=2)

    print(f"[+] Progress written to {out_file}\n")

if __name__ == "__main__":
    calc_progress()
