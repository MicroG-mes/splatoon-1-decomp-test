#!/usr/bin/env python3
"""
Splatoon (Gambit) Symbol & String Extractor
Scans the binary for source paths, RTTI class names, and assertion strings
to aid in identifying functions, structures, and directory layout.
"""

import os
import re
import sys
from pathlib import Path

def extract_strings(file_path, min_len=4):
    with open(file_path, "rb") as f:
        data = f.read()

    # Regex for printable ASCII strings
    ascii_pattern = re.compile(rb"[\x20-\x7E]{" + str(min_len).encode() + rb",}")
    matches = [m.group(0).decode("ascii", errors="ignore") for m in ascii_pattern.finditer(data)]
    return matches

def categorize_strings(strings):
    source_files = set()
    classes = set()
    sead_agl = set()
    assertions = []

    src_regex = re.compile(r"([a-zA-Z0-9_\-/\\]+\.(?:cpp|cxx|cc|h|hpp|c|s))", re.IGNORECASE)
    rtti_regex = re.compile(r"\b(Game::[A-Za-z0-9_]+|[A-Z][a-zA-Z0-9_]*Mgr|[A-Z][a-zA-Z0-9_]*Controller)\b")

    for s in strings:
        # Check for source file paths
        for match in src_regex.findall(s):
            if "/" in match or "\\" in match or match.startswith("Game"):
                source_files.add(match.replace("\\", "/"))

        # Check for RTTI / namespace class names
        for match in rtti_regex.findall(s):
            classes.add(match)

        # Check for sead / agl references
        if "sead::" in s or "agl::" in s or "nw::" in s:
            sead_agl.add(s)

        # Assertions
        if "assert" in s.lower() or "error" in s.lower() or "fatal" in s.lower():
            if len(s) < 120:
                assertions.append(s)

    return sorted(source_files), sorted(classes), sorted(sead_agl), assertions

def main():
    root = Path(__file__).resolve().parent.parent
    os.chdir(root)

    candidates = [
        Path("original/Gambit.rpx"),
        Path("original/Gambit.elf"),
        Path("Gambit.rpx"),
    ]

    target = None
    if len(sys.argv) > 1:
        target = Path(sys.argv[1])
    else:
        for c in candidates:
            if c.exists():
                target = c
                break

    if not target or not target.exists():
        print("[!] No target executable found. Please provide path:")
        print("    python tools/extract_symbols.py <path_to_Gambit.rpx>")
        return

    print(f"[*] Scanning strings from: {target}...")
    strings = extract_strings(target)
    print(f"[*] Found {len(strings):,} printable strings.")

    sources, classes, sead_agl, asserts = categorize_strings(strings)

    out_dir = root / "tools" / "extracted"
    out_dir.mkdir(parents=True, exist_ok=True)

    with open(out_dir / "source_paths.txt", "w", encoding="utf-8") as f:
        f.write("\n".join(sources))

    with open(out_dir / "classes.txt", "w", encoding="utf-8") as f:
        f.write("\n".join(classes))

    with open(out_dir / "all_strings.txt", "w", encoding="utf-8") as f:
        f.write("\n".join(strings))

    print("\n" + "=" * 60)
    print(f" [String Analysis Summary]")
    print("=" * 60)
    print(f"Source file references discovered: {len(sources):,}")
    print(f"Class/Type signatures discovered  : {len(classes):,}")
    print(f"Outputs written to: {out_dir}")
    print("=" * 60)
    print("\nSample recovered source paths:")
    for src in sources[:10]:
        print(f"  - {src}")
    print("\nSample recovered classes:")
    for cls in classes[:10]:
        print(f"  - {cls}")

if __name__ == "__main__":
    main()
