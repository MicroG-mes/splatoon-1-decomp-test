#!/usr/bin/env python3
"""
Splatoon GamePlayer Code Mapper
Maps out function entry points, string references, and components in the Player range.
"""

from pathlib import Path
from collections import defaultdict

def map_player():
    xrefs_file = Path("tools/extracted/code_to_strings.txt")
    if not xrefs_file.exists():
        print(f"[!] {xrefs_file} not found.")
        return

    player_strings = []
    with open(xrefs_file, "r", encoding="utf-8") as f:
        for line in f:
            parts = line.strip().split(" : ")
            if len(parts) == 2:
                addr_part = parts[0].split(" -> ")[0]
                addr = int(addr_part, 16)
                s = parts[1]
                if 0x02620000 <= addr <= 0x026A0000:
                    player_strings.append((addr, s))

    print(f"[*] Found {len(player_strings):,} string references in Player subsystem (0x02620000 - 0x026A0000):")
    
    # Group strings by approximate 4KB function/module clusters
    clusters = defaultdict(list)
    for addr, s in player_strings:
        clusters[(addr >> 12) << 12].append((addr, s))

    out_file = Path("tools/extracted/player_subsystem_map.txt")
    with open(out_file, "w", encoding="utf-8") as out:
        for base in sorted(clusters.keys()):
            items = clusters[base]
            out.write(f"\n=== Block 0x{base:08X} ({len(items)} refs) ===\n")
            for a, s in items:
                out.write(f"  0x{a:08X}: {s}\n")

    print(f"[+] Written full Player map to: {out_file}")
    print("\n--- Key Player String Highlights ---")
    for a, s in player_strings[:50]:
        print(f"  0x{a:08X}: \"{s}\"")

if __name__ == "__main__":
    map_player()
