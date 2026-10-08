import json

with open('tools/database/functions.json', 'r') as f:
    funcs = json.load(f)

# Build map from address to function info
fmap = {fn['address']: fn for fn in funcs}

with open('tools/database/classes.json', 'r') as f:
    classes = json.load(f)

subsystem_classes = [
    'Shop', 'KindOfShop', 'NpcMgrShop', 'PlayerMgrShop', 'LytShopHandler',
    'Npc_ShoesShop', 'Npc_WeaponsShop',
    'Npc_Judge_Flag',
    'LytPlazaGearOrderMgr',
    'Lobby', 'Fld_PlazaLobby', 'LobbyForShow',
    'Obj_PlazaGame', 'MiniGame', 'LytMiniGameHandler'
]

for c in classes:
    cname = c.get('class_name', '')
    if cname in subsystem_classes:
        vt = c.get('vtable_address')
        methods = c.get('methods', [])
        print(f"=== Class: {cname} (VTable: {vt}, Methods: {len(methods)}) ===")
        for i, m in enumerate(methods[:10]):
            fn = fmap.get(m, {})
            strs = [s for s in fn.get('strings', []) if len(s) > 2]
            src = fn.get('source_file', '')
            print(f"  [{i}] {m} (size {fn.get('size', 0)}): {src} strings: {strs[:3]}")
