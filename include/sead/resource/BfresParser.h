#pragma once

#include "types.h"
#include "sead/math/seadVector.h"
#include "sead/math/seadMatrix.h"
#include "sead/resource/seadResource.h"
#include <vector>
#include <string>

namespace sead {

struct BfresVertex {
    Vector3f position;
    Vector3f normal;
    Vector2f uv;
    Vector3f tangent;
    Vector4f color;
    u16 boneIndices[4];
    f32 boneWeights[4];

    BfresVertex()
        : position(0, 0, 0)
        , normal(0, 1, 0)
        , uv(0, 0)
        , tangent(1, 0, 0)
        , color(1, 1, 1, 1)
    {
        boneIndices[0] = 0; boneIndices[1] = 0; boneIndices[2] = 0; boneIndices[3] = 0;
        boneWeights[0] = 1.0f; boneWeights[1] = 0.0f; boneWeights[2] = 0.0f; boneWeights[3] = 0.0f;
    }
};

struct BfresMaterial {
    std::string name;
    std::string shaderName;
    std::string albedoTexture;
    std::string normalTexture;
    std::string inkMaskTexture;
    std::string specularTexture;
    Vector4f teamColor;

    BfresMaterial()
        : name("DefaultMat")
        , shaderName("splatoon_standard")
        , albedoTexture("")
        , normalTexture("")
        , inkMaskTexture("")
        , specularTexture("")
        , teamColor(1.0f, 0.5f, 0.0f, 1.0f)
    {}
};

struct BfresMesh {
    std::string name;
    u32 materialIndex;
    std::vector<BfresVertex> vertices;
    std::vector<u32> indices;

    BfresMesh() : name("SubMesh"), materialIndex(0) {}
};

struct BfresBone {
    std::string name;
    s32 parentIndex;
    Vector3f position;
    Vector3f rotation;
    Vector3f scale;

    BfresBone()
        : name("Root")
        , parentIndex(-1)
        , position(0, 0, 0)
        , rotation(0, 0, 0)
        , scale(1, 1, 1)
    {}
};

struct BfresModel {
    std::string name;
    std::vector<BfresMesh> meshes;
    std::vector<BfresMaterial> materials;
    std::vector<BfresBone> bones;

    size_t getTotalVertexCount() const {
        size_t total = 0;
        for (const auto& m : meshes) total += m.vertices.size();
        return total;
    }

    size_t getTotalIndexCount() const {
        size_t total = 0;
        for (const auto& m : meshes) total += m.indices.size();
        return total;
    }
};

class BfresParser {
public:
    static constexpr u32 cMagic = 0x46524553; // 'FRES'
    static constexpr u32 cMagicLE = 0x53455246;

    BfresParser();
    ~BfresParser();

    bool load(const u8* data, size_t size);
    bool loadFromSarc(const SarcArchive& sarc, const char* path);
    void clear();

    size_t getModelCount() const { return mModels.size(); }
    const BfresModel* getModel(size_t index) const;
    const BfresModel* findModel(const char* name) const;

    // Creates procedural geometry (quad, cube, or stage segment) for renderer testing
    static BfresModel createProceduralCube(const char* name, f32 size);
    static BfresModel createProceduralGroundPlane(const char* name, f32 width, f32 depth);
    static BfresModel createSplatoonCrateModel(const char* name, f32 size);
    static BfresModel createInklingHumanModel(const char* name = "InklingPlayerHuman", u32 teamId = 0);
    static BfresModel createInklingSquidModel(const char* name = "InklingPlayerSquid", u32 teamId = 0);
    static BfresModel createInkBulletModel(const char* name = "InkBullet", f32 radius = 0.35f);
    static BfresModel createSighterTargetModel(const char* name = "SighterTarget");
    static BfresModel createKillerWailModel(const char* name = "Weapon_KillerWail", u32 teamId = 0);
    static BfresModel createInkzookaModel(const char* name = "Weapon_Inkzooka", u32 teamId = 0);
    static BfresModel createSplattershotModel(const char* name = "Weapon_Splattershot", u32 teamId = 0);
    static BfresModel createSplatRollerModel(const char* name = "Weapon_SplatRoller", u32 teamId = 0);
    static BfresModel createSplatChargerModel(const char* name = "Weapon_SplatCharger", u32 teamId = 0);
    static BfresModel createKrakenModel(const char* name = "Weapon_Kraken", u32 teamId = 0);
    static BfresModel createOctotrooperModel(const char* name = "Enemy_Octotrooper", u32 teamId = 1);
    static BfresModel createRainmakerPedestalModel(const char* name = "Obj_ShrBasketGoal");
    static BfresModel createOctolingModel(const char* name = "RivalOctoling", u32 teamId = 1);
    static BfresModel createCallieModel(const char* name = "Npc_IdolA");
    static BfresModel createMarieModel(const char* name = "Npc_IdolB");
    static BfresModel createSpykeModel(const char* name = "Npc_CustomShop");
    static BfresModel createJuddModel(const char* name = "Npc_Judge");
    static BfresModel createSheldonModel(const char* name = "Npc_WeaponsShop");
    static BfresModel createSplatBombModel(const char* name = "Wsb_Bomb_Throw", u32 teamId = 0);
    static BfresModel createBurstBombModel(const char* name = "Wsb_Bomb_Handy", u32 teamId = 0);
    static BfresModel createSplashWallModel(const char* name = "Wsb_Shield", u32 teamId = 0);
    static BfresModel createSprinklerModel(const char* name = "Wsb_Sprinkler", u32 teamId = 0);
    static BfresModel createInkstrikeModel(const char* name = "Wsp_Tornado", u32 teamId = 0);
    static BfresModel createOctostompModel(const char* name = "Enm_Stamp");
    static BfresModel createOctocopterModel(const char* name = "Enm_Takopter");
    static BfresModel createSqueeGModel(const char* name = "Enm_Cleaner");
    static BfresModel createSparrowModel(const char* name = "Brd_Sparrow00");
    static BfresModel createAerosprayModel(const char* name = "Wmn_Shot_Blaze", u32 teamId = 0);
    static BfresModel createEliter3KModel(const char* name = "Wmn_Charge_Long", u32 teamId = 0);
    static BfresModel createOctobrushModel(const char* name = "Wmn_Roller_BrushNormal", u32 teamId = 0);
    static BfresModel createInkstrikeMonitorModel(const char* name = "Wsp_Tornado_Monitor");
    static BfresModel createHeroTankModel(const char* name = "Tnk_Msn0Lv0", u32 teamId = 0);
    static BfresModel createSkyDayPlazaModel(const char* name = "VR_SkyDayPlaza");
    static BfresModel createSkyNightPlazaModel(const char* name = "VR_SkyNightPlaza");
    static BfresModel createOctoValleySkyWorldModel(const char* name = "VR_DomeMonitorSkyWorld");
    static BfresModel createBananaTreeModel(const char* name = "Obj_TreeBanana");
    static BfresModel createWaterTankModel(const char* name = "Obj_WaterTank");
    static BfresModel createSunkenScrollModel(const char* name = "Obj_AncientDocument");
    static BfresModel createSunkenScrollDummyModel(const char* name = "Obj_AncientDocumentDummy");
    static BfresModel createGreatZapfishModel(const char* name = "Obj_BigNamazu");
    static BfresModel createMiniZapfishModel(const char* name = "Obj_Namazu");
    static BfresModel createZapfishDummyModel(const char* name = "Obj_NamazuDummy");
    static BfresModel createGoalPedestalModel(const char* name = "Obj_Goal");
    static BfresModel createGeyserModel(const char* name = "Obj_Geyser");
    static BfresModel createSpongeModel(const char* name = "Obj_Sponge");
    static BfresModel createDoorKeyModel(const char* name = "Obj_DoorKey00");
    static BfresModel createTreasureBoxModel(const char* name = "Obj_KeyTreasureBox");
    static BfresModel createIkastoneModel(const char* name = "Obj_Ikastone");
    static BfresModel createJumpPointModel(const char* name = "Obj_JumpPoint");
    static BfresModel createOctavioPilotHouseModel(const char* name = "Obj_RailKingPilotHouse");
    static BfresModel createPropellerBasePlateModel(const char* name = "Lft_PropellerBasePlateRailKing");
    static BfresModel createOctUfoBoxModel(const char* name = "Lft_OctUfoBox");
    static BfresModel createOctUfoWallModel(const char* name = "Lft_OctUfoWall");
    static BfresModel createRvlUfoMiniModel(const char* name = "Lft_RvlUfoMini");
    static BfresModel createMissileModel(const char* name = "Obj_Missile");
    static BfresModel createAreaGateModel(const char* name = "Obj_AreaGate");
    static BfresModel createBombFlowerModel(const char* name = "Obj_BombFlower");
    static BfresModel createBossGatewayModel(const char* name = "Obj_BossGateway");
    static BfresModel createGateManholeModel(const char* name = "Obj_GateManhole");
    static BfresModel createGatewayModel(const char* name = "Obj_Gateway");
    static BfresModel createCylinderKingBallModel(const char* name = "Obj_CylinderKingBall");
    static BfresModel createCylinderKingWallModel(const char* name = "Lft_CylinderKingWall");
    static BfresModel createCylinderKingPoisonPondModel(const char* name = "Obj_CylinderKingPoisonPond");
    static BfresModel createWarpPointFlagModel(const char* name = "Obj_Flag");
    static BfresModel createSubFlagModel(const char* name = "Wsb_Flag");
    static BfresModel createAirBallModel(const char* name = "Obj_AirBall");
    static BfresModel createAirBallDuelModel(const char* name = "Obj_AirBallDuel");
    static BfresModel createAirBallMsnModel(const char* name = "Obj_AirBallMsn");
    static BfresModel createGrassModel(const char* name = "Obj_Grass00");
    static BfresModel createGrassRuinsModel(const char* name = "Obj_GrassRuins00");
    static BfresModel createSeaGullModel(const char* name = "Obj_SeaGull");
    static BfresModel createTree00Model(const char* name = "Obj_Tree00");
    static BfresModel createTree01Model(const char* name = "Obj_Tree01");
    static BfresModel createRubberPoleModel(const char* name = "Obj_RubberPole00");
    static BfresModel createWindsockModel(const char* name = "Obj_Windsock");
    static BfresModel createTakodozerModel(const char* name = "Enm_Takodozer");
    static BfresModel createOctoballModel(const char* name = "Enm_Ball");
    static BfresModel createPropellerLift00Model(const char* name = "Lft_Propeller00");
    static BfresModel createPropellerLift01Model(const char* name = "Lft_Propeller01");
    static BfresModel createPropellerFanModel(const char* name = "Obj_Propeller01");
    static BfresModel createHeroArmorModel(const char* name = "Obj_Armor");
    static BfresModel createSwitchPaintModel(const char* name = "Obj_SwitchPaint");
    static BfresModel createColorConeModel(const char* name = "Obj_ColorCone");
    static BfresModel createSquidGuardModel(const char* name = "Obj_SquidGuard");
    static BfresModel createBossWeakPointModel(const char* name = "Enm_BossWeakPoint");
    static BfresModel createOctLegModel(const char* name = "Enm_OctLeg");
    static BfresModel createCommanderModel(const char* name = "Npc_Commander");
    static BfresModel createCommanderBindModel(const char* name = "Npc_CommanderBind");
    static BfresModel createAtarimeHouseModel(const char* name = "Obj_AtarimeHouse");
    static BfresModel createBigNamazuModel(const char* name = "Obj_BigNamazu");
    static BfresModel createRespawnPlatformModel(const char* name = "Obj_RespawnPlatform");
    static BfresModel createStampModel(const char* name = "Enm_Stamp");
    static BfresModel createBox00LModel(const char* name = "Obj_Box00L");
    static BfresModel createBox00SModel(const char* name = "Obj_Box00S");
    static BfresModel createBox01LModel(const char* name = "Obj_Box01L");

    bool loadFromSzsFile(const char* szsFilePath);

private:
    std::string mArchiveName;
    std::vector<BfresModel> mModels;
    bool mIsBigEndian;
    u32 mVersion;
};

} // namespace sead
