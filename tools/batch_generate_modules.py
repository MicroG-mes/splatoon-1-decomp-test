#!/usr/bin/env python3
"""
Autonomous Module Generator for Splatoon 1 (Gambit)
Scaffolds all 12 recovered game subsystems across the source tree.
"""

from pathlib import Path

MODULES = {
    # Module 1: Player System
    "Game/Player/GamePlayerJointHuman": {
        "namespace": "Game",
        "class": "GamePlayerJointHuman",
        "desc": "Humanoid bone joint and animation controller",
        "base": "GambitActor"
    },
    "Game/Player/GamePlayerJointSquid": {
        "namespace": "Game",
        "class": "GamePlayerJointSquid",
        "desc": "Squid form bone joint and tentacle deformation controller",
        "base": "GambitActor"
    },
    "Game/Player/GamePlayerJointSwing": {
        "namespace": "Game",
        "class": "GamePlayerJointSwing",
        "desc": "Secondary physics simulation for hair tentacles and gear sway",
        "base": "GambitActor"
    },

    # Module 2: Main Weapons & Projectiles
    "Game/Bullet/GameBulletPlayerNormalShotBase": {
        "namespace": "Game",
        "class": "GameBulletPlayerNormalShotBase",
        "desc": "Base projectile for all shooter weapons (Splattershot, Aero, etc.)",
        "base": "GameBullet"
    },
    "Game/Bullet/GameBulletSimple": {
        "namespace": "Game",
        "class": "GameBulletSimple",
        "desc": "Lightweight projectile with minimal physics",
        "base": "GameBullet"
    },

    # Module 4: Special Weapons
    "Game/Weapon/GameWeaponSuperShot": {
        "namespace": "Game",
        "class": "GameWeaponSuperShot",
        "desc": "Inkzooka special weapon launcher",
        "base": "GambitActor"
    },
    "Game/Weapon/GameWeaponDaiouIka": {
        "namespace": "Game",
        "class": "GameWeaponDaiouIka",
        "desc": "Kraken special weapon transformation and spinning attack",
        "base": "GambitActor"
    },
    "Game/Weapon/GameWeaponMegaphone": {
        "namespace": "Game",
        "class": "GameWeaponMegaphone",
        "desc": "Killer Wail acoustic ink laser special weapon",
        "base": "GambitActor"
    },

    # Module 5: Octo Valley Enemies
    "Game/Enemy/GameEnemyTakopter": {
        "namespace": "Game",
        "class": "GameEnemyTakopter",
        "desc": "Flying Twintacle Octotrooper with propeller flight AI",
        "base": "GambitActor"
    },
    "Game/Enemy/GameEnemyCleaner": {
        "namespace": "Game",
        "class": "GameEnemyCleaner",
        "desc": "Octosqueegee ground maintenance ink cleaner",
        "base": "GambitActor"
    },
    "Game/Enemy/GameEnemyCharge": {
        "namespace": "Game",
        "class": "GameEnemyCharge",
        "desc": "Octostamp slamming attack enemy",
        "base": "GambitActor"
    },

    # Module 6: Octo Valley Bosses
    "Game/Enemy/GameEnemyBallKing": {
        "namespace": "Game",
        "class": "GameEnemyBallKing",
        "desc": "Octomaw boss AI (swimming under ink, breaching with giant teeth)",
        "base": "GambitActor"
    },
    "Game/Enemy/GameEnemyHideKing": {
        "namespace": "Game",
        "class": "GameEnemyHideKing",
        "desc": "Octonozzle rotating cannon boss AI",
        "base": "GambitActor"
    },

    # Module 7: Map Objects & Stage Mechanics
    "Game/MapObj/GameInkRail": {
        "namespace": "Game",
        "class": "GameInkRail",
        "desc": "Rideable ink rail spline path and squid sliding interaction",
        "base": "GambitActor"
    },
    "Game/MapObj/GameTurnPlate": {
        "namespace": "Game",
        "class": "GameTurnPlate",
        "desc": "Rotating stage platform (Museum d'Alfonsino / Saltspray Rig)",
        "base": "GambitActor"
    },

    # Module 8: Collectibles & Items
    "Game/Item/GameItemBase": {
        "namespace": "Game",
        "class": "GameItemBase",
        "desc": "Base collectible item with pickup trigger",
        "base": "GambitActor"
    },
    "Game/Item/GameItemShachihoko": {
        "namespace": "Game",
        "class": "GameItemShachihoko",
        "desc": "Rainmaker golden fish objective item and carrier charge shot",
        "base": "GameItemBase"
    },

    # Module 9: Plaza
    "Game/Plaza/GamePlazaMiiverseShot": {
        "namespace": "Game",
        "class": "GamePlazaMiiverseShot",
        "desc": "Inkopolis Plaza Miiverse post billboard renderer",
        "base": "GambitActor"
    },

    # Module 10: Paint Engine
    "Game/Paint/WallPaintMgr": {
        "namespace": "Game",
        "class": "WallPaintMgr",
        "desc": "Vertical surface and 3D wall ink texture projection manager",
        "base": "GambitActor"
    }
}

def generate_all():
    print(f"[*] Scaffolding {len(MODULES)} recovered subsystem classes...")

    for rel_path, info in MODULES.items():
        cls_name = info["class"]
        ns = info["namespace"]
        desc = info["desc"]
        base = info["base"]

        h_path = Path("include") / f"{rel_path}.h"
        cpp_path = Path("src") / f"{rel_path}.cpp"

        h_path.parent.mkdir(parents=True, exist_ok=True)
        cpp_path.parent.mkdir(parents=True, exist_ok=True)

        # Base header include
        if base == "GameBullet":
            base_include = '#include "Game/Bullet/GameBullet.h"'
        elif base == "GameItemBase":
            base_include = '#include "Game/Item/GameItemBase.h"'
        else:
            base_include = '#include "Game/Actor/GambitActor.h"'

        h_content = f"""#pragma once

#include "types.h"
{base_include}
#include "sead/math/seadVector.h"

namespace {ns} {{

/**
 * {cls_name}
 * {desc}
 */
class {cls_name} : public {base} {{
public:
    {cls_name}();
    virtual ~{cls_name}() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

protected:
    undefined mModuleData[0x40];
}};

}} // namespace {ns}
"""

        cpp_content = f"""#include "{rel_path}.h"

namespace {ns} {{

{cls_name}::{cls_name}() = default;

{cls_name}::~{cls_name}() = default;

void {cls_name}::init() {{
    {base}::init();
}}

void {cls_name}::update() {{
    {base}::update();
}}

void {cls_name}::draw() {{
    {base}::draw();
}}

}} // namespace {ns}
"""

        h_path.write_text(h_content, encoding="utf-8")
        cpp_path.write_text(cpp_content, encoding="utf-8")
        print(f"  [+] Created {h_path} and {cpp_path}")

    print(f"[+] Successfully scaffolded all {len(MODULES)} subsystem modules!")

if __name__ == "__main__":
    generate_all()
