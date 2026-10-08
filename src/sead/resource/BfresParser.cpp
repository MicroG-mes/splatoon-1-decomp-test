#include "sead/resource/BfresParser.h"
#include <cstring>
#include <cstdio>
#include <cmath>

namespace sead {

BfresParser::BfresParser()
    : mArchiveName("")
    , mIsBigEndian(true)
    , mVersion(0)
{
}

BfresParser::~BfresParser() {
    clear();
}

void BfresParser::clear() {
    mModels.clear();
    mArchiveName.clear();
}

bool BfresParser::load(const u8* data, size_t size) {
    clear();
    if (!data || size < 16) {
        return false;
    }

    u32 magic = *reinterpret_cast<const u32*>(data);
    if (magic == cMagic) {
        mIsBigEndian = true;
    } else if (magic == cMagicLE) {
        mIsBigEndian = false;
    } else {
        // Not a BFRES magic header
        return false;
    }

    // Read version and byte order
    u32 rawVersion = *reinterpret_cast<const u32*>(data + 4);
    mVersion = Endian::toHostU32(rawVersion, mIsBigEndian);

    u16 rawBom = *reinterpret_cast<const u16*>(data + 8);
    u16 bom = Endian::toHostU16(rawBom, mIsBigEndian);
    if (bom == 0xFEFF) {
        mIsBigEndian = true;
    } else if (bom == 0xFFFE) {
        mIsBigEndian = false;
    }

    u32 rawFileSize = *reinterpret_cast<const u32*>(data + 12);
    u32 fileSize = Endian::toHostU32(rawFileSize, mIsBigEndian);
    (void)fileSize;

    // Minimum mock/test model representation if parsed successfully
    BfresModel parsedModel;
    parsedModel.name = "FRES_Model_0";

    BfresMaterial defMat;
    defMat.name = "DefMat";
    defMat.shaderName = "splatoon_standard";
    parsedModel.materials.push_back(defMat);

    BfresMesh mesh;
    mesh.name = "Mesh_0";
    mesh.materialIndex = 0;

    // Basic triangle placeholder for binary header validation
    BfresVertex v0, v1, v2;
    v0.position = Vector3f(0.0f, 1.0f, 0.0f);
    v1.position = Vector3f(-1.0f, -1.0f, 0.0f);
    v2.position = Vector3f(1.0f, -1.0f, 0.0f);
    mesh.vertices.push_back(v0);
    mesh.vertices.push_back(v1);
    mesh.vertices.push_back(v2);
    mesh.indices.push_back(0);
    mesh.indices.push_back(1);
    mesh.indices.push_back(2);

    parsedModel.meshes.push_back(mesh);
    mModels.push_back(parsedModel);

    return true;
}

bool BfresParser::loadFromSarc(const SarcArchive& sarc, const char* path) {
    size_t fileSize = 0;
    const u8* fileData = sarc.getFile(path, &fileSize);
    if (!fileData || fileSize == 0) {
        return false;
    }
    return load(fileData, fileSize);
}

const BfresModel* BfresParser::getModel(size_t index) const {
    if (index >= mModels.size()) {
        return nullptr;
    }
    return &mModels[index];
}

const BfresModel* BfresParser::findModel(const char* name) const {
    if (!name) return nullptr;
    for (const auto& model : mModels) {
        if (model.name == name) {
            return &model;
        }
    }
    return nullptr;
}

BfresModel BfresParser::createProceduralCube(const char* name, f32 size) {
    BfresModel model;
    model.name = name ? name : "ProceduralCube";

    BfresMaterial mat;
    mat.name = "CubeMat";
    mat.shaderName = "splatoon_mesh";
    model.materials.push_back(mat);

    BfresMesh mesh;
    mesh.name = "CubeMesh";
    mesh.materialIndex = 0;

    f32 h = size * 0.5f;

    struct FaceDef {
        Vector3f normal;
        Vector3f p[4];
        Vector2f uv[4];
    };

    FaceDef faces[6] = {
        // Front (+Z)
        { Vector3f(0, 0, 1), { Vector3f(-h, -h,  h), Vector3f( h, -h,  h), Vector3f( h,  h,  h), Vector3f(-h,  h,  h) },
          { Vector2f(0, 1), Vector2f(1, 1), Vector2f(1, 0), Vector2f(0, 0) } },
        // Back (-Z)
        { Vector3f(0, 0, -1), { Vector3f( h, -h, -h), Vector3f(-h, -h, -h), Vector3f(-h,  h, -h), Vector3f( h,  h, -h) },
          { Vector2f(0, 1), Vector2f(1, 1), Vector2f(1, 0), Vector2f(0, 0) } },
        // Top (+Y)
        { Vector3f(0, 1, 0), { Vector3f(-h,  h,  h), Vector3f( h,  h,  h), Vector3f( h,  h, -h), Vector3f(-h,  h, -h) },
          { Vector2f(0, 1), Vector2f(1, 1), Vector2f(1, 0), Vector2f(0, 0) } },
        // Bottom (-Y)
        { Vector3f(0, -1, 0), { Vector3f(-h, -h, -h), Vector3f( h, -h, -h), Vector3f( h, -h,  h), Vector3f(-h, -h,  h) },
          { Vector2f(0, 1), Vector2f(1, 1), Vector2f(1, 0), Vector2f(0, 0) } },
        // Right (+X)
        { Vector3f(1, 0, 0), { Vector3f( h, -h,  h), Vector3f( h, -h, -h), Vector3f( h,  h, -h), Vector3f( h,  h,  h) },
          { Vector2f(0, 1), Vector2f(1, 1), Vector2f(1, 0), Vector2f(0, 0) } },
        // Left (-X)
        { Vector3f(-1, 0, 0), { Vector3f(-h, -h, -h), Vector3f(-h, -h,  h), Vector3f(-h,  h,  h), Vector3f(-h,  h, -h) },
          { Vector2f(0, 1), Vector2f(1, 1), Vector2f(1, 0), Vector2f(0, 0) } }
    };

    for (int f = 0; f < 6; ++f) {
        u32 baseIdx = static_cast<u32>(mesh.vertices.size());
        for (int v = 0; v < 4; ++v) {
            BfresVertex vert;
            vert.position = faces[f].p[v];
            vert.normal = faces[f].normal;
            vert.uv = faces[f].uv[v];
            mesh.vertices.push_back(vert);
        }
        mesh.indices.push_back(baseIdx + 0);
        mesh.indices.push_back(baseIdx + 1);
        mesh.indices.push_back(baseIdx + 2);
        mesh.indices.push_back(baseIdx + 0);
        mesh.indices.push_back(baseIdx + 2);
        mesh.indices.push_back(baseIdx + 3);
    }

    model.meshes.push_back(mesh);
    return model;
}

BfresModel BfresParser::createProceduralGroundPlane(const char* name, f32 width, f32 depth) {
    BfresModel model;
    model.name = name ? name : "ProceduralGround";

    BfresMaterial mat;
    mat.name = "GroundMat";
    mat.shaderName = "splatoon_stage";
    model.materials.push_back(mat);

    BfresMesh mesh;
    mesh.name = "GroundMesh";
    mesh.materialIndex = 0;

    f32 hw = width * 0.5f;
    f32 hd = depth * 0.5f;

    BfresVertex v[4];
    v[0].position = Vector3f(-hw, 0.0f, -hd); v[0].normal = Vector3f(0, 1, 0); v[0].uv = Vector2f(0, 0);
    v[1].position = Vector3f( hw, 0.0f, -hd); v[1].normal = Vector3f(0, 1, 0); v[1].uv = Vector2f(1, 0);
    v[2].position = Vector3f( hw, 0.0f,  hd); v[2].normal = Vector3f(0, 1, 0); v[2].uv = Vector2f(1, 1);
    v[3].position = Vector3f(-hw, 0.0f,  hd); v[3].normal = Vector3f(0, 1, 0); v[3].uv = Vector2f(0, 1);

    for (int i = 0; i < 4; ++i) {
        mesh.vertices.push_back(v[i]);
    }

    mesh.indices.push_back(0);
    mesh.indices.push_back(1);
    mesh.indices.push_back(2);
    mesh.indices.push_back(0);
    mesh.indices.push_back(2);
    mesh.indices.push_back(3);

    model.meshes.push_back(mesh);
    return model;
}

BfresModel BfresParser::createSplatoonCrateModel(const char* name, f32 size) {
    BfresModel model;
    model.name = name ? name : "Obj_GeneralBox";

    // Material 0: GeneralBox Wooden Body
    BfresMaterial bodyMat;
    bodyMat.name = "M_GeneralBoxBody";
    bodyMat.shaderName = "splatoon_wood_box";
    bodyMat.teamColor = Vector4f(0.88f, 0.68f, 0.42f, 1.0f); // Warm natural pine wood
    model.materials.push_back(bodyMat);

    // Material 1: GeneralBox Steel Corner Reinforcement
    BfresMaterial edgeMat;
    edgeMat.name = "M_GeneralBoxTopEdge";
    edgeMat.shaderName = "splatoon_steel_trim";
    edgeMat.teamColor = Vector4f(0.32f, 0.36f, 0.40f, 1.0f); // Galvanized dark steel
    model.materials.push_back(edgeMat);

    // SubMesh 1: Wooden Body (pCube213__M_GeneralBoxBody)
    BfresMesh bodyMesh;
    bodyMesh.name = "pCube213__M_GeneralBoxBody";
    bodyMesh.materialIndex = 0;

    f32 h = size * 0.5f;

    struct FaceDef {
        Vector3f normal;
        Vector3f p[4];
        Vector2f uv[4];
    };

    FaceDef faces[6] = {
        // Front (+Z)
        { Vector3f(0, 0, 1), { Vector3f(-h, -h,  h), Vector3f( h, -h,  h), Vector3f( h,  h,  h), Vector3f(-h,  h,  h) },
          { Vector2f(0, 1), Vector2f(1, 1), Vector2f(1, 0), Vector2f(0, 0) } },
        // Back (-Z)
        { Vector3f(0, 0, -1), { Vector3f( h, -h, -h), Vector3f(-h, -h, -h), Vector3f(-h,  h, -h), Vector3f( h,  h, -h) },
          { Vector2f(0, 1), Vector2f(1, 1), Vector2f(1, 0), Vector2f(0, 0) } },
        // Top (+Y)
        { Vector3f(0, 1, 0), { Vector3f(-h,  h,  h), Vector3f( h,  h,  h), Vector3f( h,  h, -h), Vector3f(-h,  h, -h) },
          { Vector2f(0, 1), Vector2f(1, 1), Vector2f(1, 0), Vector2f(0, 0) } },
        // Bottom (-Y)
        { Vector3f(0, -1, 0), { Vector3f(-h, -h, -h), Vector3f( h, -h, -h), Vector3f( h, -h,  h), Vector3f(-h, -h,  h) },
          { Vector2f(0, 1), Vector2f(1, 1), Vector2f(1, 0), Vector2f(0, 0) } },
        // Right (+X)
        { Vector3f(1, 0, 0), { Vector3f( h, -h,  h), Vector3f( h, -h, -h), Vector3f( h,  h, -h), Vector3f( h,  h,  h) },
          { Vector2f(0, 1), Vector2f(1, 1), Vector2f(1, 0), Vector2f(0, 0) } },
        // Left (-X)
        { Vector3f(-1, 0, 0), { Vector3f(-h, -h, -h), Vector3f(-h, -h,  h), Vector3f(-h,  h,  h), Vector3f(-h,  h, -h) },
          { Vector2f(0, 1), Vector2f(1, 1), Vector2f(1, 0), Vector2f(0, 0) } }
    };

    for (int f = 0; f < 6; ++f) {
        u32 baseIdx = static_cast<u32>(bodyMesh.vertices.size());
        for (int v = 0; v < 4; ++v) {
            BfresVertex vert;
            vert.position = faces[f].p[v];
            vert.normal = faces[f].normal;
            vert.uv = faces[f].uv[v];
            vert.color = Vector4f(0.92f, 0.76f, 0.52f, 1.0f);
            bodyMesh.vertices.push_back(vert);
        }
        bodyMesh.indices.push_back(baseIdx + 0);
        bodyMesh.indices.push_back(baseIdx + 1);
        bodyMesh.indices.push_back(baseIdx + 2);
        bodyMesh.indices.push_back(baseIdx + 0);
        bodyMesh.indices.push_back(baseIdx + 2);
        bodyMesh.indices.push_back(baseIdx + 3);
    }
    model.meshes.push_back(bodyMesh);

    // SubMesh 2: Steel Top Corner Braces (pCube216__M_GeneralBoxTopEdge)
    BfresMesh edgeMesh;
    edgeMesh.name = "pCube216__M_GeneralBoxTopEdge";
    edgeMesh.materialIndex = 1;

    f32 eh = h * 1.025f;
    f32 bh = h * 0.90f;

    // Corner bracket rims
    FaceDef rimFaces[4] = {
        { Vector3f(0, 0, 1), { Vector3f(-eh, bh,  eh), Vector3f( eh, bh,  eh), Vector3f( eh,  eh,  eh), Vector3f(-eh,  eh,  eh) },
          { Vector2f(0, 1), Vector2f(1, 1), Vector2f(1, 0), Vector2f(0, 0) } },
        { Vector3f(0, 0, -1), { Vector3f( eh, bh, -eh), Vector3f(-eh, bh, -eh), Vector3f(-eh,  eh, -eh), Vector3f( eh,  eh, -eh) },
          { Vector2f(0, 1), Vector2f(1, 1), Vector2f(1, 0), Vector2f(0, 0) } },
        { Vector3f(1, 0, 0), { Vector3f( eh, bh,  eh), Vector3f( eh, bh, -eh), Vector3f( eh,  eh, -eh), Vector3f( eh,  eh,  eh) },
          { Vector2f(0, 1), Vector2f(1, 1), Vector2f(1, 0), Vector2f(0, 0) } },
        { Vector3f(-1, 0, 0), { Vector3f(-eh, bh, -eh), Vector3f(-eh, bh,  eh), Vector3f(-eh,  eh,  eh), Vector3f(-eh,  eh, -eh) },
          { Vector2f(0, 1), Vector2f(1, 1), Vector2f(1, 0), Vector2f(0, 0) } }
    };

    for (int f = 0; f < 4; ++f) {
        u32 baseIdx = static_cast<u32>(edgeMesh.vertices.size());
        for (int v = 0; v < 4; ++v) {
            BfresVertex vert;
            vert.position = rimFaces[f].p[v];
            vert.normal = rimFaces[f].normal;
            vert.uv = rimFaces[f].uv[v];
            vert.color = Vector4f(0.45f, 0.48f, 0.52f, 1.0f);
            edgeMesh.vertices.push_back(vert);
        }
        edgeMesh.indices.push_back(baseIdx + 0);
        edgeMesh.indices.push_back(baseIdx + 1);
        edgeMesh.indices.push_back(baseIdx + 2);
        edgeMesh.indices.push_back(baseIdx + 0);
        edgeMesh.indices.push_back(baseIdx + 2);
        edgeMesh.indices.push_back(baseIdx + 3);
    }
    model.meshes.push_back(edgeMesh);

    return model;
}

BfresModel BfresParser::createInklingHumanModel(const char* name, u32 teamId) {
    BfresModel model;
    model.name = name ? name : "InklingPlayerHuman";

    Vector4f teamColor = (teamId == 0)
        ? Vector4f(1.0f, 0.45f, 0.05f, 1.0f)   // Team Alpha Neon Orange
        : Vector4f(0.05f, 0.85f, 0.95f, 1.0f);  // Team Bravo Neon Cyan

    // Materials
    BfresMaterial skinMat; skinMat.name = "M_InklingSkin"; skinMat.teamColor = Vector4f(0.96f, 0.82f, 0.72f, 1.0f);
    BfresMaterial hairMat; hairMat.name = "M_InklingHair"; hairMat.teamColor = teamColor;
    BfresMaterial clothMat; clothMat.name = "M_InklingCloth"; clothMat.teamColor = Vector4f(0.18f, 0.20f, 0.22f, 1.0f);
    BfresMaterial gearMat; gearMat.name = "M_InklingGear"; gearMat.teamColor = Vector4f(0.85f, 0.88f, 0.20f, 1.0f); // Splattershot neon yellow
    model.materials.push_back(skinMat);  // 0
    model.materials.push_back(hairMat);  // 1
    model.materials.push_back(clothMat); // 2
    model.materials.push_back(gearMat);  // 3

    auto addBox = [](BfresMesh& mesh, Vector3f center, Vector3f size, Vector4f col) {
        f32 hx = size.x * 0.5f, hy = size.y * 0.5f, hz = size.z * 0.5f;
        Vector3f corners[8] = {
            center + Vector3f(-hx, -hy, -hz), center + Vector3f( hx, -hy, -hz),
            center + Vector3f( hx,  hy, -hz), center + Vector3f(-hx,  hy, -hz),
            center + Vector3f(-hx, -hy,  hz), center + Vector3f( hx, -hy,  hz),
            center + Vector3f( hx,  hy,  hz), center + Vector3f(-hx,  hy,  hz)
        };
        int faces[6][4] = {
            { 0, 1, 2, 3 }, // Back
            { 5, 4, 7, 6 }, // Front
            { 4, 0, 3, 7 }, // Left
            { 1, 5, 6, 2 }, // Right
            { 3, 2, 6, 7 }, // Top
            { 4, 5, 1, 0 }  // Bottom
        };
        Vector3f norms[6] = {
            Vector3f(0, 0, -1), Vector3f(0, 0, 1),
            Vector3f(-1, 0, 0), Vector3f(1, 0, 0),
            Vector3f(0, 1, 0),  Vector3f(0, -1, 0)
        };
        for (int f = 0; f < 6; ++f) {
            u32 base = static_cast<u32>(mesh.vertices.size());
            for (int v = 0; v < 4; ++v) {
                BfresVertex vert;
                vert.position = corners[faces[f][v]];
                vert.normal = norms[f];
                vert.uv = Vector2f((v == 1 || v == 2) ? 1.0f : 0.0f, (v >= 2) ? 1.0f : 0.0f);
                vert.color = col;
                mesh.vertices.push_back(vert);
            }
            mesh.indices.push_back(base + 0); mesh.indices.push_back(base + 1); mesh.indices.push_back(base + 2);
            mesh.indices.push_back(base + 0); mesh.indices.push_back(base + 2); mesh.indices.push_back(base + 3);
        }
    };

    // 1. Head & Face (Material 0)
    BfresMesh headMesh; headMesh.name = "SubMesh_Head"; headMesh.materialIndex = 0;
    addBox(headMesh, Vector3f(0.0f, 1.45f, 0.0f), Vector3f(0.38f, 0.38f, 0.36f), Vector4f(0.96f, 0.82f, 0.72f, 1.0f));
    model.meshes.push_back(headMesh);

    // 2. Tentacle Hair (Material 1)
    BfresMesh hairMesh; hairMesh.name = "SubMesh_Hair"; hairMesh.materialIndex = 1;
    // Left & Right front tentacles
    addBox(hairMesh, Vector3f(-0.24f, 1.15f, 0.05f), Vector3f(0.12f, 0.65f, 0.16f), teamColor);
    addBox(hairMesh, Vector3f( 0.24f, 1.15f, 0.05f), Vector3f(0.12f, 0.65f, 0.16f), teamColor);
    // Back hair crest
    addBox(hairMesh, Vector3f( 0.00f, 1.55f, -0.15f), Vector3f(0.36f, 0.22f, 0.20f), teamColor);
    model.meshes.push_back(hairMesh);

    // 3. Body & Clothing (Material 2)
    BfresMesh bodyMesh; bodyMesh.name = "SubMesh_Body"; bodyMesh.materialIndex = 2;
    // Torso (Shirt)
    addBox(bodyMesh, Vector3f(0.0f, 0.95f, 0.0f), Vector3f(0.42f, 0.55f, 0.28f), Vector4f(0.20f, 0.22f, 0.25f, 1.0f));
    // Legs & Shoes
    addBox(bodyMesh, Vector3f(-0.13f, 0.35f, 0.0f), Vector3f(0.14f, 0.65f, 0.16f), Vector4f(0.12f, 0.12f, 0.15f, 1.0f));
    addBox(bodyMesh, Vector3f( 0.13f, 0.35f, 0.0f), Vector3f(0.14f, 0.65f, 0.16f), Vector4f(0.12f, 0.12f, 0.15f, 1.0f));
    model.meshes.push_back(bodyMesh);

    // 4. Ink Tank & Weapon (Material 3)
    BfresMesh gearMesh; gearMesh.name = "SubMesh_Gear"; gearMesh.materialIndex = 3;
    // Ink Tank on back
    addBox(gearMesh, Vector3f(0.0f, 0.95f, -0.22f), Vector3f(0.22f, 0.55f, 0.20f), teamColor * 0.85f);
    // Splattershot Blaster held forward
    addBox(gearMesh, Vector3f(0.25f, 0.90f, 0.35f), Vector3f(0.15f, 0.18f, 0.55f), Vector4f(0.90f, 0.85f, 0.15f, 1.0f));
    addBox(gearMesh, Vector3f(0.25f, 0.96f, 0.60f), Vector3f(0.08f, 0.08f, 0.22f), Vector4f(0.20f, 0.20f, 0.20f, 1.0f)); // Nozzle
    model.meshes.push_back(gearMesh);

    return model;
}

BfresModel BfresParser::createInklingSquidModel(const char* name, u32 teamId) {
    BfresModel model;
    model.name = name ? name : "InklingPlayerSquid";

    Vector4f teamColor = (teamId == 0)
        ? Vector4f(1.0f, 0.45f, 0.05f, 1.0f)
        : Vector4f(0.05f, 0.85f, 0.95f, 1.0f);

    BfresMaterial mantleMat; mantleMat.name = "M_SquidMantle"; mantleMat.teamColor = teamColor;
    BfresMaterial eyesMat;   eyesMat.name = "M_SquidEyes";     eyesMat.teamColor = Vector4f(0.95f, 0.90f, 0.10f, 1.0f);
    model.materials.push_back(mantleMat); // 0
    model.materials.push_back(eyesMat);   // 1

    BfresMesh mantleMesh; mantleMesh.name = "SubMesh_SquidMantle"; mantleMesh.materialIndex = 0;
    // Aerodynamic arrowhead squid mantle
    Vector3f pFront( 0.0f,  0.15f,  0.55f);
    Vector3f pBackL(-0.35f, 0.12f, -0.35f);
    Vector3f pBackR( 0.35f, 0.12f, -0.35f);
    Vector3f pTop(   0.0f,  0.30f, -0.05f);
    Vector3f pBot(   0.0f,  0.02f, -0.05f);

    auto addTri = [&](BfresMesh& m, Vector3f a, Vector3f b, Vector3f c, Vector4f col) {
        Vector3f ab = b - a;
        Vector3f ac = c - a;
        Vector3f n = ab.cross(ac).normalized();
        u32 idx = static_cast<u32>(m.vertices.size());
        BfresVertex va, vb, vc;
        va.position = a; va.normal = n; va.uv = Vector2f(0.5f, 1.0f); va.color = col;
        vb.position = b; vb.normal = n; vb.uv = Vector2f(0.0f, 0.0f); vb.color = col;
        vc.position = c; vc.normal = n; vc.uv = Vector2f(1.0f, 0.0f); vc.color = col;
        m.vertices.push_back(va); m.vertices.push_back(vb); m.vertices.push_back(vc);
        m.indices.push_back(idx); m.indices.push_back(idx + 1); m.indices.push_back(idx + 2);
    };

    // Upper pyramid
    addTri(mantleMesh, pFront, pBackR, pTop, teamColor);
    addTri(mantleMesh, pFront, pTop, pBackL, teamColor);
    addTri(mantleMesh, pTop, pBackR, pBackL, teamColor);
    // Lower pyramid
    addTri(mantleMesh, pFront, pBot, pBackR, teamColor);
    addTri(mantleMesh, pFront, pBackL, pBot, teamColor);
    addTri(mantleMesh, pBot, pBackL, pBackR, teamColor);

    // Twin tentacles trailing behind
    addTri(mantleMesh, pBackL, Vector3f(-0.45f, 0.08f, -0.75f), Vector3f(-0.25f, 0.08f, -0.75f), teamColor);
    addTri(mantleMesh, pBackR, Vector3f( 0.25f, 0.08f, -0.75f), Vector3f( 0.45f, 0.08f, -0.75f), teamColor);
    model.meshes.push_back(mantleMesh);

    // Squid Eyes
    BfresMesh eyeMesh; eyeMesh.name = "SubMesh_SquidEyes"; eyeMesh.materialIndex = 1;
    addTri(eyeMesh, Vector3f(-0.16f, 0.22f, 0.15f), Vector3f(-0.10f, 0.26f, 0.12f), Vector3f(-0.12f, 0.18f, 0.10f), Vector4f(1, 1, 0.2f, 1));
    addTri(eyeMesh, Vector3f( 0.16f, 0.22f, 0.15f), Vector3f( 0.12f, 0.18f, 0.10f), Vector3f( 0.10f, 0.26f, 0.12f), Vector4f(1, 1, 0.2f, 1));
    model.meshes.push_back(eyeMesh);

    return model;
}

BfresModel BfresParser::createInkBulletModel(const char* name, f32 radius) {
    BfresModel model;
    model.name = name ? name : "InkBullet";

    BfresMaterial bulletMat;
    bulletMat.name = "M_InkBullet";
    bulletMat.shaderName = "splatoon_wet_ink";
    bulletMat.teamColor = Vector4f(1.0f, 0.5f, 0.0f, 1.0f);
    model.materials.push_back(bulletMat);

    BfresMesh mesh;
    mesh.name = "SubMesh_BulletBlob";
    mesh.materialIndex = 0;

    f32 r = radius;
    // Octahedron teardrop blob
    Vector3f top(0, r * 1.4f, 0), bot(0, -r * 0.8f, 0);
    Vector3f fwd(0, 0, r * 1.2f), bck(0, 0, -r * 0.8f);
    Vector3f lft(-r, 0, 0), rgt(r, 0, 0);

    auto addBlobTri = [&](Vector3f a, Vector3f b, Vector3f c) {
        Vector3f n = (b - a).cross(c - a).normalized();
        u32 idx = static_cast<u32>(mesh.vertices.size());
        BfresVertex va, vb, vc;
        va.position = a; va.normal = n; va.uv = Vector2f(0.5f, 1.0f); va.color = Vector4f(1, 1, 1, 1);
        vb.position = b; vb.normal = n; vb.uv = Vector2f(0.0f, 0.0f); vb.color = Vector4f(1, 1, 1, 1);
        vc.position = c; vc.normal = n; vc.uv = Vector2f(1.0f, 0.0f); vc.color = Vector4f(1, 1, 1, 1);
        mesh.vertices.push_back(va); mesh.vertices.push_back(vb); mesh.vertices.push_back(vc);
        mesh.indices.push_back(idx); mesh.indices.push_back(idx + 1); mesh.indices.push_back(idx + 2);
    };

    addBlobTri(top, rgt, fwd); addBlobTri(top, fwd, lft);
    addBlobTri(top, lft, bck); addBlobTri(top, bck, rgt);
    addBlobTri(bot, fwd, rgt); addBlobTri(bot, lft, fwd);
    addBlobTri(bot, bck, lft); addBlobTri(bot, rgt, bck);

    model.meshes.push_back(mesh);
    return model;
}

BfresModel BfresParser::createSighterTargetModel(const char* name) {
    BfresModel model;
    model.name = name ? name : "SighterTarget";

    // Material 0: Metal Base & Spring Rod
    BfresMaterial baseMat;
    baseMat.name = "M_TargetStand";
    baseMat.shaderName = "splatoon_standard";
    baseMat.teamColor = Vector4f(0.35f, 0.38f, 0.42f, 1.0f);
    model.materials.push_back(baseMat);

    // Material 1: Dummy Body Board
    BfresMaterial boardMat;
    boardMat.name = "M_TargetBoard";
    boardMat.shaderName = "splatoon_standard";
    boardMat.teamColor = Vector4f(0.92f, 0.85f, 0.35f, 1.0f);
    model.materials.push_back(boardMat);

    // Material 2: Bullseye Ring
    BfresMaterial ringMat;
    ringMat.name = "M_TargetBullseye";
    ringMat.shaderName = "splatoon_standard";
    ringMat.teamColor = Vector4f(0.85f, 0.18f, 0.18f, 1.0f);
    model.materials.push_back(ringMat);

    BfresMesh standMesh;
    standMesh.name = "Mesh_Stand";
    standMesh.materialIndex = 0;

    BfresMesh boardMesh;
    boardMesh.name = "Mesh_Board";
    boardMesh.materialIndex = 1;

    BfresMesh ringMesh;
    ringMesh.name = "Mesh_Ring";
    ringMesh.materialIndex = 2;

    auto addBox = [](BfresMesh& m, const Vector3f& center, const Vector3f& extents, const Vector4f& col) {
        Vector3f corners[8] = {
            center + Vector3f(-extents.x, -extents.y, -extents.z),
            center + Vector3f( extents.x, -extents.y, -extents.z),
            center + Vector3f( extents.x,  extents.y, -extents.z),
            center + Vector3f(-extents.x,  extents.y, -extents.z),
            center + Vector3f(-extents.x, -extents.y,  extents.z),
            center + Vector3f( extents.x, -extents.y,  extents.z),
            center + Vector3f( extents.x,  extents.y,  extents.z),
            center + Vector3f(-extents.x,  extents.y,  extents.z)
        };
        Vector3f normals[6] = {
            Vector3f(0, 0, -1), Vector3f(0, 0, 1),
            Vector3f(-1, 0, 0), Vector3f(1, 0, 0),
            Vector3f(0, 1, 0),  Vector3f(0, -1, 0)
        };
        u32 faceIndices[6][4] = {
            {0, 3, 2, 1}, {4, 5, 6, 7},
            {0, 4, 7, 3}, {1, 2, 6, 5},
            {3, 7, 6, 2}, {0, 1, 5, 4}
        };
        for (int f = 0; f < 6; ++f) {
            u32 faceBase = static_cast<u32>(m.vertices.size());
            for (int v = 0; v < 4; ++v) {
                BfresVertex vert;
                vert.position = corners[faceIndices[f][v]];
                vert.normal = normals[f];
                vert.uv = Vector2f((v == 1 || v == 2) ? 1.0f : 0.0f, (v >= 2) ? 1.0f : 0.0f);
                vert.color = col;
                m.vertices.push_back(vert);
            }
            m.indices.push_back(faceBase + 0);
            m.indices.push_back(faceBase + 1);
            m.indices.push_back(faceBase + 2);
            m.indices.push_back(faceBase + 0);
            m.indices.push_back(faceBase + 2);
            m.indices.push_back(faceBase + 3);
        }
    };

    // Stand
    addBox(standMesh, Vector3f(0.0f, 0.08f, 0.0f), Vector3f(0.70f, 0.08f, 0.70f), Vector4f(0.28f, 0.30f, 0.32f, 1.0f));
    addBox(standMesh, Vector3f(0.0f, 0.65f, 0.0f), Vector3f(0.10f, 0.50f, 0.10f), Vector4f(0.40f, 0.42f, 0.45f, 1.0f));

    // Torso Board
    addBox(boardMesh, Vector3f(0.0f, 1.80f, 0.0f), Vector3f(0.55f, 0.65f, 0.08f), Vector4f(0.95f, 0.88f, 0.35f, 1.0f));

    // Bullseye Ring
    addBox(ringMesh, Vector3f(0.0f, 1.80f, 0.09f), Vector3f(0.30f, 0.30f, 0.02f), Vector4f(0.88f, 0.18f, 0.18f, 1.0f));
    addBox(ringMesh, Vector3f(0.0f, 1.80f, -0.09f), Vector3f(0.30f, 0.30f, 0.02f), Vector4f(0.88f, 0.18f, 0.18f, 1.0f));

    model.meshes.push_back(standMesh);
    model.meshes.push_back(boardMesh);
    model.meshes.push_back(ringMesh);

    return model;
}

bool BfresParser::loadFromSzsFile(const char* szsFilePath) {
    if (!szsFilePath) return false;
    FILE* fp = fopen(szsFilePath, "rb");
    if (!fp) return false;

    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (fsize <= 0) {
        fclose(fp);
        return false;
    }

    std::vector<u8> buffer(static_cast<size_t>(fsize));
    size_t readBytes = fread(buffer.data(), 1, buffer.size(), fp);
    fclose(fp);

    if (readBytes != buffer.size()) return false;

    SarcArchive sarc;
    if (!sarc.load(buffer.data(), buffer.size())) return false;

    for (size_t i = 0; i < sarc.getFileCount(); ++i) {
        const auto* info = sarc.getFileInfo(i);
        if (info && info->name.find(".bfres") != std::string::npos) {
            return load(info->data, info->size);
        }
    }

    return false;
}

} // namespace sead
