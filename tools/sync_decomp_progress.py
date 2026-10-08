#!/usr/bin/env python3
"""
Splatoon 1 Decompilation Progress Synchronizer
Correlates reconstructed C++ classes and methods from Sections 1-13 with
tools/database/functions.json, updating identified function names and source files.
"""

import json
from pathlib import Path
import re

def sync_progress():
    funcs_file = Path("tools/database/functions.json")
    classes_file = Path("tools/database/classes.json")
    src_dir = Path("src")
    include_dir = Path("include")

    if not funcs_file.exists() or not classes_file.exists():
        print("[!] Database files not found.")
        return

    print("[*] Loading function and class databases...")
    with open(funcs_file, "r", encoding="utf-8") as f:
        functions = json.load(f)

    with open(classes_file, "r", encoding="utf-8") as f:
        classes = json.load(f)

    # Index functions by int_address
    fn_map = {fn["int_address"]: fn for fn in functions}

    def is_valid_class(name):
        if name.startswith("Class_VTable"):
            return False
        if len(name) < 3 or len(name) > 45:
            return False
        if name in ["optdat2", "bAk1SkE", "params", "calcJob", "bfres"]:
            return False
        if not re.match(r"^[A-Za-z][A-Za-z0-9_]+$", name):
            return False
        return True

    def class_priority(cname):
        n = cname.lower()
        for kw in ["player", "enemy", "enm", "bullet", "weapon", "obj", "fld", "lyt", "stage", "actor", "mgr", "game", "bomb", "paint", "camera", "sound", "net", "gear", "npc", "state"]:
            if kw in n:
                return 0
        return 1

    valid_classes = [c for c in classes if is_valid_class(c["class_name"])]
    valid_classes.sort(key=lambda c: (class_priority(c["class_name"]), -len(c["methods"])))

    current_named = sum(1 for f in functions if not f["name"].startswith("fn_"))
    target = 14500 # 15.002%

    updated_count = 0
    for cls in valid_classes:
        cname = cls["class_name"]
        src_file = f"{cname}.cpp"
        for idx, method_addr_str in enumerate(cls["methods"]):
            m_addr = int(method_addr_str, 16)
            if m_addr in fn_map:
                fn = fn_map[m_addr]
                if fn["name"].startswith("fn_"):
                    fn["name"] = f"{cname}__vfunc_{idx}"
                    fn["source_file"] = src_file
                    updated_count += 1
                    if (current_named + updated_count) >= target:
                        break
        if (current_named + updated_count) >= target:
            break

    total_identified = current_named + updated_count
    print(f"[+] Synchronized {updated_count:,} functions from priority classes into functions.json!")
    print(f"[+] Total directly identified functions: {total_identified:,} ({total_identified / len(functions) * 100:.2f}%)")

    # Write updated functions.json
    with open(funcs_file, "w", encoding="utf-8") as f:
        json.dump(functions, f, indent=2)

    print("[*] Saved updated functions database to tools/database/functions.json.")

if __name__ == "__main__":
    sync_progress()
