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

    if (data[0] != 'F' || data[1] != 'R' || data[2] != 'E' || data[3] != 'S') {
        return false;
    }

    // Byte 8 and 9 are the BOM: 0xFE 0xFF = Big Endian (Wii U), 0xFF 0xFE = Little Endian
    mIsBigEndian = (data[8] == 0xFE && data[9] == 0xFF);

    // Read version
    u32 rawVersion = *reinterpret_cast<const u32*>(data + 4);
    mVersion = Endian::toHostU32(rawVersion, mIsBigEndian);

    auto readU32 = [this, data, size](size_t offset) -> u32 {
        if (offset + 4 > size) return 0;
        u32 raw = *reinterpret_cast<const u32*>(data + offset);
        return Endian::toHostU32(raw, mIsBigEndian);
    };

    auto readU16 = [this, data, size](size_t offset) -> u16 {
        if (offset + 2 > size) return 0;
        u16 raw = *reinterpret_cast<const u16*>(data + offset);
        return Endian::toHostU16(raw, mIsBigEndian);
    };

    auto readF32 = [this, data, size](size_t offset) -> f32 {
        if (offset + 4 > size) return 0.0f;
        u32 raw = *reinterpret_cast<const u32*>(data + offset);
        u32 host = Endian::toHostU32(raw, mIsBigEndian);
        f32 res = 0.0f;
        std::memcpy(&res, &host, 4);
        return res;
    };

    // 1. Collect all FMDL (Model) block offsets throughout the archive
    std::vector<size_t> fmdlOffsets;
    for (size_t offset = 0; offset + 64 <= size; offset += 4) {
        if (std::memcmp(data + offset, "FMDL", 4) == 0) {
            fmdlOffsets.push_back(offset);
        }
    }

    for (size_t mIdx = 0; mIdx < fmdlOffsets.size(); ++mIdx) {
        size_t fmdlOff = fmdlOffsets[mIdx];
        size_t nextModelOff = (mIdx + 1 < fmdlOffsets.size()) ? fmdlOffsets[mIdx + 1] : size;

        BfresModel model;

        // Model name
        u32 nameRel = readU32(fmdlOff + 4);
        size_t nameActual = fmdlOff + 4 + nameRel;
        if (nameActual < size) {
            const char* nameStr = reinterpret_cast<const char*>(data + nameActual);
            model.name = nameStr;
        } else {
            model.name = "FMDL_Model_" + std::to_string(mModels.size());
        }

        u16 numFvtx = readU16(fmdlOff + 0x20);
        u16 numFshp = readU16(fmdlOff + 0x22);

        // Find FVTX vertex buffers associated with this model
        u32 vtxRel = readU32(fmdlOff + 0x10);
        size_t fvtxBase = fmdlOff + 0x10 + vtxRel;

        std::vector<std::vector<Vector3f>> modelVertexPositions;

        // Parse FVTX structures: each FVTX descriptor is 0x20 bytes
        for (u16 vIdx = 0; vIdx < numFvtx; ++vIdx) {
            size_t fvtxOff = fvtxBase + vIdx * 0x20;
            if (fvtxOff + 32 > size) break;
            if (std::memcmp(data + fvtxOff, "FVTX", 4) != 0) {
                continue;
            }

            u32 numVerts = readU32(fvtxOff + 8);
            u32 vbArrRel = readU32(fvtxOff + 0x18);
            size_t vbArr = fvtxOff + 0x18 + vbArrRel;

            std::vector<Vector3f> positions;
            if (vbArr + 24 <= size) {
                u16 stride = readU16(vbArr + 0x0C);
                if (stride == 0) stride = 16;
                u32 vbDataRel = readU32(vbArr + 0x14);
                size_t vbData = vbArr + 0x14 + vbDataRel;

                if (vbData + numVerts * stride <= size) {
                    positions.reserve(numVerts);
                    for (size_t v = 0; v < numVerts; ++v) {
                        size_t p = vbData + v * stride;
                        Vector3f pos(readF32(p), readF32(p + 4), readF32(p + 8));
                        positions.push_back(pos);
                    }
                }
            }
            modelVertexPositions.push_back(std::move(positions));
        }

        // If direct FVTX array had fewer than numFvtx, do range scan fallback
        if (modelVertexPositions.size() < numFvtx) {
            for (size_t scanV = fvtxBase; scanV + 32 <= nextModelOff && modelVertexPositions.size() < numFvtx; scanV += 4) {
                if (std::memcmp(data + scanV, "FVTX", 4) == 0) {
                    u32 numVerts = readU32(scanV + 8);
                    u32 vbArrRel = readU32(scanV + 0x18);
                    size_t vbArr = scanV + 0x18 + vbArrRel;
                    if (vbArr + 24 <= size) {
                        u16 stride = readU16(vbArr + 0x0C);
                        if (stride == 0) stride = 16;
                        u32 vbDataRel = readU32(vbArr + 0x14);
                        size_t vbData = vbArr + 0x14 + vbDataRel;
                        if (vbData + numVerts * stride <= size) {
                            std::vector<Vector3f> positions;
                            positions.reserve(numVerts);
                            for (size_t v = 0; v < numVerts; ++v) {
                                size_t p = vbData + v * stride;
                                positions.emplace_back(readF32(p), readF32(p + 4), readF32(p + 8));
                            }
                            modelVertexPositions.push_back(std::move(positions));
                        }
                    }
                }
            }
        }

        // Parse FSHP shapes within [fmdlOff, nextModelOff)
        for (size_t scanS = fmdlOff; scanS + 64 <= nextModelOff; scanS += 4) {
            if (std::memcmp(data + scanS, "FSHP", 4) != 0) {
                continue;
            }

            size_t fshpOff = scanS;
            BfresMesh mesh;

            u32 sNameRel = readU32(fshpOff + 4);
            size_t sNameActual = fshpOff + 4 + sNameRel;
            if (sNameActual < size) {
                mesh.name = reinterpret_cast<const char*>(data + sNameActual);
            } else {
                mesh.name = "SubMesh_" + std::to_string(model.meshes.size());
            }

            u16 matIdx = readU16(fshpOff + 0x0E);
            mesh.materialIndex = matIdx;

            u16 vtxBufferIdx = readU16(fshpOff + 0x12);
            if (vtxBufferIdx >= modelVertexPositions.size()) {
                vtxBufferIdx = 0;
            }

            u32 totalIndices = readU32(fshpOff + 0x40);
            u32 meshArrRel = readU32(fshpOff + 0x30);
            size_t meshArr = fshpOff + 0x30 + meshArrRel;

            // Search for index buffer pointer inside meshArr structure
            size_t idxData = 0;
            if (totalIndices > 0 && meshArr < size) {
                for (size_t k = 0x18; k < 0x600 && meshArr + k + 4 <= size; k += 4) {
                    u32 offVal = readU32(meshArr + k);
                    size_t target = meshArr + k + offVal;
                    if (target > 0 && target + totalIndices * 2 <= size) {
                        u16 s0 = readU16(target);
                        u16 s1 = readU16(target + 2);
                        u16 s2 = readU16(target + 4);
                        if (s0 < 65000 && s1 < 65000 && s2 < 65000) {
                            idxData = target;
                            break;
                        }
                    }
                }
            }

            // Fallback for single-LOD crates / target dummy
            if (idxData == 0 && totalIndices > 0 && meshArr + 24 <= size) {
                u32 idxBufRel = readU32(meshArr + 0x14);
                size_t idxBuf = meshArr + 0x14 + idxBufRel;
                if (idxBuf + 24 <= size) {
                    u32 idRel = readU32(idxBuf + 0x14);
                    idxData = idxBuf + 0x14 + idRel;
                }
            }

            if (idxData > 0 && totalIndices > 0 && !modelVertexPositions.empty() && vtxBufferIdx < modelVertexPositions.size()) {
                const auto& positions = modelVertexPositions[vtxBufferIdx];
                if (!positions.empty()) {
                    mesh.vertices.resize(positions.size());
                    for (size_t vi = 0; vi < positions.size(); ++vi) {
                        mesh.vertices[vi].position = positions[vi];
                        mesh.vertices[vi].normal = Vector3f(0.0f, 1.0f, 0.0f);
                        mesh.vertices[vi].color = Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
                    }

                    mesh.indices.reserve(totalIndices);
                    for (size_t k = 0; k < totalIndices; ++k) {
                        u16 idx = readU16(idxData + k * 2);
                        if (idx < mesh.vertices.size()) {
                            mesh.indices.push_back(idx);
                        }
                    }

                    // Compute surface normal per triangle
                    for (size_t tri = 0; tri + 2 < mesh.indices.size(); tri += 3) {
                        u32 i0 = mesh.indices[tri];
                        u32 i1 = mesh.indices[tri + 1];
                        u32 i2 = mesh.indices[tri + 2];
                        Vector3f e1 = mesh.vertices[i1].position - mesh.vertices[i0].position;
                        Vector3f e2 = mesh.vertices[i2].position - mesh.vertices[i0].position;
                        Vector3f n = e1.cross(e2).normalized();
                        mesh.vertices[i0].normal = n;
                        mesh.vertices[i1].normal = n;
                        mesh.vertices[i2].normal = n;
                    }

                    model.meshes.push_back(std::move(mesh));
                }
            }
        }

        // Materials
        BfresMaterial defMat;
        defMat.name = model.name + "_Mat";
        defMat.shaderName = "splatoon_standard";
        model.materials.push_back(defMat);

        if (!model.meshes.empty()) {
            mModels.push_back(std::move(model));
        }
    }


    // Fallback if no full models were parsed
    if (mModels.empty()) {
        BfresModel fallbackModel;
        fallbackModel.name = "Fallback_Cube";
        fallbackModel = createProceduralCube("Fallback_Cube", 1.0f);
        mModels.push_back(fallbackModel);
    }

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
    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Obj_GeneralBox.szs")) {
        const BfresModel* realModel = szsParser.getModel(0);
        if (realModel && !realModel->meshes.empty()) {
            BfresModel copy = *realModel;
            if (name) copy.name = name;
            f32 scale = size / 15.0f;
            for (auto& m : copy.meshes) {
                for (auto& v : m.vertices) {
                    v.position = (v.position - Vector3f(0.0f, 7.5f, 0.0f)) * scale;
                }
            }
            return copy;
        }
    }

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
    Vector4f teamColor = (teamId == 0)
        ? Vector4f(1.0f, 0.45f, 0.05f, 1.0f)   // Team Alpha Neon Orange
        : Vector4f(0.05f, 0.85f, 0.95f, 1.0f);  // Team Bravo Neon Cyan

    BfresParser szsParser;
    const char* candFiles[] = { "content/Model/Player00.szs", "content/Model/Player01.szs" };
    for (const char* p : candFiles) {
        if (szsParser.loadFromSzsFile(p)) {
            const BfresModel* realModel = szsParser.getModel(0);
            if (realModel && !realModel->meshes.empty()) {
                BfresModel copy = *realModel;
                if (name) copy.name = name;
                f32 scale = 1.8f / 13.36f;
                for (auto& m : copy.meshes) {
                    for (auto& v : m.vertices) {
                        v.position = (v.position - Vector3f(0.0f, 0.21f, 0.0f)) * scale;
                        if (m.name.find("Hair") != std::string::npos || m.name.find("hair") != std::string::npos ||
                            m.name.find("Tentacle") != std::string::npos || m.name.find("Ink") != std::string::npos) {
                            v.color = teamColor;
                        }
                    }
                }
                return copy;
            }
        }
    }

    BfresModel model;
    model.name = name ? name : "InklingPlayerHuman";

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
    Vector4f teamColor = (teamId == 0)
        ? Vector4f(1.0f, 0.45f, 0.05f, 1.0f)
        : Vector4f(0.05f, 0.85f, 0.95f, 1.0f);

    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Player_Squid.szs")) {
        const BfresModel* realModel = szsParser.getModel(0);
        if (realModel && !realModel->meshes.empty()) {
            BfresModel copy = *realModel;
            if (name) copy.name = name;
            f32 scale = 0.12f;
            for (auto& m : copy.meshes) {
                for (auto& v : m.vertices) {
                    v.position = (v.position - Vector3f(0.0f, -0.39f, 0.0f)) * scale;
                    v.color = teamColor;
                }
            }
            return copy;
        }
    }

    BfresModel model;
    model.name = name ? name : "InklingPlayerSquid";

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
    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Obj_SighterTarget.szs")) {
        const BfresModel* realModel = szsParser.getModel(0);
        if (realModel && !realModel->meshes.empty()) {
            BfresModel copy = *realModel;
            if (name) copy.name = name;
            f32 scale = 2.4f / 21.5f;
            for (auto& m : copy.meshes) {
                for (auto& v : m.vertices) {
                    v.position = v.position * scale;
                }
            }
            return copy;
        }
    }

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

BfresModel BfresParser::createKillerWailModel(const char* name, u32 teamId) {
    Vector4f teamColor = (teamId == 0)
        ? Vector4f(1.0f, 0.45f, 0.05f, 1.0f)
        : Vector4f(0.05f, 0.85f, 0.95f, 1.0f);

    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Wsp_BigLaser.szs")) {
        const BfresModel* realModel = szsParser.getModel(0);
        if (realModel && !realModel->meshes.empty()) {
            BfresModel copy = *realModel;
            if (name) copy.name = name;
            f32 scale = 0.15f;
            for (auto& m : copy.meshes) {
                for (auto& v : m.vertices) {
                    v.position = (v.position - Vector3f(0.0f, -2.8f, 0.0f)) * scale;
                    if (m.name.find("Team") != std::string::npos || m.name.find("Color") != std::string::npos) {
                        v.color = teamColor;
                    }
                }
            }
            return copy;
        }
    }

    BfresModel model;
    model.name = name ? name : "Weapon_KillerWail";

    BfresMaterial speakerMat; speakerMat.name = "M_SpeakerBody"; speakerMat.teamColor = Vector4f(0.20f, 0.22f, 0.25f, 1.0f);
    BfresMaterial hornMat;    hornMat.name = "M_SpeakerHorn";    hornMat.teamColor = teamColor;
    BfresMaterial frameMat;   frameMat.name = "M_SpeakerFrame";   frameMat.teamColor = Vector4f(0.65f, 0.65f, 0.70f, 1.0f);

    model.materials.push_back(speakerMat); // 0
    model.materials.push_back(hornMat);    // 1
    model.materials.push_back(frameMat);   // 2

    BfresMesh bodyMesh;  bodyMesh.name = "SubMesh_Body";  bodyMesh.materialIndex = 0;
    BfresMesh hornMesh;  hornMesh.name = "SubMesh_Horn";  hornMesh.materialIndex = 1;
    BfresMesh frameMesh; frameMesh.name = "SubMesh_Frame"; frameMesh.materialIndex = 2;

    auto addBox = [](BfresMesh& m, Vector3f center, Vector3f halfExtents, Vector4f col) {
        Vector3f corners[8] = {
            center + Vector3f(-halfExtents.x, -halfExtents.y, -halfExtents.z),
            center + Vector3f( halfExtents.x, -halfExtents.y, -halfExtents.z),
            center + Vector3f( halfExtents.x,  halfExtents.y, -halfExtents.z),
            center + Vector3f(-halfExtents.x,  halfExtents.y, -halfExtents.z),
            center + Vector3f(-halfExtents.x, -halfExtents.y,  halfExtents.z),
            center + Vector3f( halfExtents.x, -halfExtents.y,  halfExtents.z),
            center + Vector3f( halfExtents.x,  halfExtents.y,  halfExtents.z),
            center + Vector3f(-halfExtents.x,  halfExtents.y,  halfExtents.z)
        };
        u32 faceIndices[6][4] = {
            { 0, 3, 2, 1 }, { 4, 5, 6, 7 },
            { 0, 1, 5, 4 }, { 2, 3, 7, 6 },
            { 0, 4, 7, 3 }, { 1, 2, 6, 5 }
        };
        Vector3f faceNormals[6] = {
            Vector3f(0, 0, -1), Vector3f(0, 0, 1),
            Vector3f(0, -1, 0), Vector3f(0, 1, 0),
            Vector3f(-1, 0, 0), Vector3f(1, 0, 0)
        };
        for (int f = 0; f < 6; ++f) {
            u32 faceBase = static_cast<u32>(m.vertices.size());
            for (int v = 0; v < 4; ++v) {
                BfresVertex vert;
                vert.position = corners[faceIndices[f][v]];
                vert.normal = faceNormals[f];
                vert.color = col;
                vert.uv = (v == 0) ? Vector2f(0, 0) : (v == 1) ? Vector2f(1, 0) : (v == 2) ? Vector2f(1, 1) : Vector2f(0, 1);
                m.vertices.push_back(vert);
            }
            m.indices.push_back(faceBase + 0); m.indices.push_back(faceBase + 1); m.indices.push_back(faceBase + 2);
            m.indices.push_back(faceBase + 0); m.indices.push_back(faceBase + 2); m.indices.push_back(faceBase + 3);
        }
    };

    // Central Megaphone Speaker Housing
    addBox(bodyMesh, Vector3f(0.0f, 0.70f, 0.0f), Vector3f(0.40f, 0.40f, 0.70f), Vector4f(0.18f, 0.18f, 0.22f, 1.0f));
    // Acoustic Flared Horn
    addBox(hornMesh, Vector3f(0.0f, 0.70f, 0.85f), Vector3f(0.65f, 0.65f, 0.20f), teamColor);
    addBox(hornMesh, Vector3f(0.0f, 0.70f, 1.05f), Vector3f(0.85f, 0.85f, 0.06f), teamColor * 1.15f);
    // Tripod Stand Frame
    addBox(frameMesh, Vector3f(-0.35f, 0.18f, -0.35f), Vector3f(0.06f, 0.20f, 0.06f), Vector4f(0.6f, 0.6f, 0.65f, 1.0f));
    addBox(frameMesh, Vector3f( 0.35f, 0.18f, -0.35f), Vector3f(0.06f, 0.20f, 0.06f), Vector4f(0.6f, 0.6f, 0.65f, 1.0f));
    addBox(frameMesh, Vector3f( 0.00f, 0.18f,  0.40f), Vector3f(0.06f, 0.20f, 0.06f), Vector4f(0.6f, 0.6f, 0.65f, 1.0f));

    model.meshes.push_back(bodyMesh);
    model.meshes.push_back(hornMesh);
    model.meshes.push_back(frameMesh);

    return model;
}

BfresModel BfresParser::createInkzookaModel(const char* name, u32 teamId) {
    BfresModel model;
    model.name = name ? name : "Weapon_Inkzooka";

    Vector4f teamColor = (teamId == 0)
        ? Vector4f(1.0f, 0.45f, 0.05f, 1.0f)
        : Vector4f(0.05f, 0.85f, 0.95f, 1.0f);

    BfresMaterial barrelMat; barrelMat.name = "M_ZookaBarrel"; barrelMat.teamColor = Vector4f(0.15f, 0.15f, 0.18f, 1.0f);
    BfresMaterial drumMat;   drumMat.name = "M_ZookaDrum";     drumMat.teamColor = teamColor;
    BfresMaterial gripMat;   gripMat.name = "M_ZookaGrip";     gripMat.teamColor = Vector4f(0.85f, 0.85f, 0.15f, 1.0f);

    model.materials.push_back(barrelMat);
    model.materials.push_back(drumMat);
    model.materials.push_back(gripMat);

    BfresMesh barrelMesh; barrelMesh.name = "SubMesh_Barrel"; barrelMesh.materialIndex = 0;
    BfresMesh drumMesh;   drumMesh.name = "SubMesh_Drum";     drumMesh.materialIndex = 1;
    BfresMesh gripMesh;   gripMesh.name = "SubMesh_Grip";     gripMesh.materialIndex = 2;

    auto addBox = [](BfresMesh& m, Vector3f center, Vector3f halfExtents, Vector4f col) {
        Vector3f corners[8] = {
            center + Vector3f(-halfExtents.x, -halfExtents.y, -halfExtents.z),
            center + Vector3f( halfExtents.x, -halfExtents.y, -halfExtents.z),
            center + Vector3f( halfExtents.x,  halfExtents.y, -halfExtents.z),
            center + Vector3f(-halfExtents.x,  halfExtents.y, -halfExtents.z),
            center + Vector3f(-halfExtents.x, -halfExtents.y,  halfExtents.z),
            center + Vector3f( halfExtents.x, -halfExtents.y,  halfExtents.z),
            center + Vector3f( halfExtents.x,  halfExtents.y,  halfExtents.z),
            center + Vector3f(-halfExtents.x,  halfExtents.y,  halfExtents.z)
        };
        u32 faceIndices[6][4] = {
            { 0, 3, 2, 1 }, { 4, 5, 6, 7 },
            { 0, 1, 5, 4 }, { 2, 3, 7, 6 },
            { 0, 4, 7, 3 }, { 1, 2, 6, 5 }
        };
        Vector3f faceNormals[6] = {
            Vector3f(0, 0, -1), Vector3f(0, 0, 1),
            Vector3f(0, -1, 0), Vector3f(0, 1, 0),
            Vector3f(-1, 0, 0), Vector3f(1, 0, 0)
        };
        for (int f = 0; f < 6; ++f) {
            u32 faceBase = static_cast<u32>(m.vertices.size());
            for (int v = 0; v < 4; ++v) {
                BfresVertex vert;
                vert.position = corners[faceIndices[f][v]];
                vert.normal = faceNormals[f];
                vert.color = col;
                vert.uv = (v == 0) ? Vector2f(0, 0) : (v == 1) ? Vector2f(1, 0) : (v == 2) ? Vector2f(1, 1) : Vector2f(0, 1);
                m.vertices.push_back(vert);
            }
            m.indices.push_back(faceBase + 0); m.indices.push_back(faceBase + 1); m.indices.push_back(faceBase + 2);
            m.indices.push_back(faceBase + 0); m.indices.push_back(faceBase + 2); m.indices.push_back(faceBase + 3);
        }
    };

    // Bazooka Heavy Barrel (forward tube)
    addBox(barrelMesh, Vector3f(0.0f, 0.0f, 0.25f), Vector3f(0.18f, 0.18f, 0.65f), Vector4f(0.20f, 0.20f, 0.22f, 1.0f));
    // Muzzle Opening Rim
    addBox(barrelMesh, Vector3f(0.0f, 0.0f, 0.90f), Vector3f(0.22f, 0.22f, 0.05f), Vector4f(0.15f, 0.15f, 0.15f, 1.0f));
    // Ink Rotary Drum Magazine
    addBox(drumMesh, Vector3f(0.0f, 0.08f, -0.15f), Vector3f(0.25f, 0.25f, 0.22f), teamColor);
    // Grip Handle & Shoulder Rest
    addBox(gripMesh, Vector3f(0.0f, -0.22f, 0.05f), Vector3f(0.06f, 0.15f, 0.08f), Vector4f(0.85f, 0.85f, 0.15f, 1.0f));
    addBox(gripMesh, Vector3f(0.0f, -0.05f, -0.45f), Vector3f(0.12f, 0.12f, 0.16f), Vector4f(0.35f, 0.35f, 0.38f, 1.0f));

    model.meshes.push_back(barrelMesh);
    model.meshes.push_back(drumMesh);
    model.meshes.push_back(gripMesh);

    return model;
}

BfresModel BfresParser::createSplattershotModel(const char* name, u32 teamId) {
    Vector4f teamColor = (teamId == 0)
        ? Vector4f(1.0f, 0.45f, 0.05f, 1.0f)
        : Vector4f(0.05f, 0.85f, 0.95f, 1.0f);

    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Wmn_Shot_Normal.szs")) {
        const BfresModel* realModel = szsParser.getModel(0);
        if (realModel && !realModel->meshes.empty()) {
            BfresModel copy = *realModel;
            if (name) copy.name = name;
            f32 scale = 0.18f;
            for (auto& m : copy.meshes) {
                for (auto& v : m.vertices) {
                    v.position = v.position * scale;
                    if (m.name.find("Ink") != std::string::npos || m.name.find("Tank") != std::string::npos) {
                        v.color = teamColor;
                    }
                }
            }
            return copy;
        }
    }
    return createInkzookaModel(name, teamId);
}

BfresModel BfresParser::createSplatRollerModel(const char* name, u32 teamId) {
    Vector4f teamColor = (teamId == 0)
        ? Vector4f(1.0f, 0.45f, 0.05f, 1.0f)
        : Vector4f(0.05f, 0.85f, 0.95f, 1.0f);

    BfresParser szsParser;
    const char* paths[] = { "content/Model/Wmn_Roller_Normal.szs", "content/Model/Wmn_Roller_Heavy.szs" };
    for (const char* p : paths) {
        if (szsParser.loadFromSzsFile(p)) {
            const BfresModel* realModel = szsParser.getModel(0);
            if (realModel && !realModel->meshes.empty()) {
                BfresModel copy = *realModel;
                if (name) copy.name = name;
                f32 scale = 0.16f;
                for (auto& m : copy.meshes) {
                    for (auto& v : m.vertices) {
                        v.position = v.position * scale;
                        if (m.name.find("Ink") != std::string::npos || m.name.find("Roll") != std::string::npos) {
                            v.color = teamColor;
                        }
                    }
                }
                return copy;
            }
        }
    }
    return createInkzookaModel(name, teamId);
}

BfresModel BfresParser::createSplatChargerModel(const char* name, u32 teamId) {
    Vector4f teamColor = (teamId == 0)
        ? Vector4f(1.0f, 0.45f, 0.05f, 1.0f)
        : Vector4f(0.05f, 0.85f, 0.95f, 1.0f);

    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Wmn_Charge_Light.szs")) {
        const BfresModel* realModel = szsParser.getModel(0);
        if (realModel && !realModel->meshes.empty()) {
            BfresModel copy = *realModel;
            if (name) copy.name = name;
            f32 scale = 0.18f;
            for (auto& m : copy.meshes) {
                for (auto& v : m.vertices) {
                    v.position = v.position * scale;
                    if (m.name.find("Ink") != std::string::npos || m.name.find("Tank") != std::string::npos) {
                        v.color = teamColor;
                    }
                }
            }
            return copy;
        }
    }
    return createSplattershotModel(name, teamId);
}

BfresModel BfresParser::createKrakenModel(const char* name, u32 teamId) {
    Vector4f teamColor = (teamId == 0)
        ? Vector4f(1.0f, 0.45f, 0.05f, 1.0f)
        : Vector4f(0.05f, 0.85f, 0.95f, 1.0f);

    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Wsp_KingSquid.szs")) {
        const BfresModel* realModel = szsParser.getModel(0);
        if (realModel && !realModel->meshes.empty()) {
            BfresModel copy = *realModel;
            if (name) copy.name = name;
            f32 scale = 0.22f;
            for (auto& m : copy.meshes) {
                for (auto& v : m.vertices) {
                    v.position = v.position * scale;
                    v.color = teamColor;
                }
            }
            return copy;
        }
    }
    return createInklingSquidModel(name, teamId);
}

BfresModel BfresParser::createOctotrooperModel(const char* name, u32 teamId) {
    Vector4f teamColor = (teamId == 0)
        ? Vector4f(1.0f, 0.45f, 0.05f, 1.0f)
        : Vector4f(0.95f, 0.12f, 0.45f, 1.0f);

    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Enm_Hohei.szs")) {
        const BfresModel* realModel = szsParser.getModel(0);
        if (realModel && !realModel->meshes.empty()) {
            BfresModel copy = *realModel;
            if (name) copy.name = name;
            f32 scale = 0.16f;
            for (auto& m : copy.meshes) {
                for (auto& v : m.vertices) {
                    v.position = v.position * scale;
                    v.color = teamColor;
                }
            }
            return copy;
        }
    }
    return createProceduralCube(name ? name : "Enemy_Octotrooper", 1.2f);
}

BfresModel BfresParser::createRainmakerPedestalModel(const char* name) {
    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Obj_ShrBasketGoal.szs")) {
        const BfresModel* realModel = szsParser.getModel(0);
        if (realModel && !realModel->meshes.empty()) {
            BfresModel copy = *realModel;
            if (name) copy.name = name;
            f32 scale = 0.12f;
            for (auto& m : copy.meshes) {
                for (auto& v : m.vertices) {
                    v.position = v.position * scale;
                }
            }
            return copy;
        }
    }
    return createSplatoonCrateModel(name ? name : "Obj_ShrBasketGoal", 3.0f);
}

BfresModel BfresParser::createOctolingModel(const char* name, u32 teamId) {
    Vector4f teamColor = (teamId == 0)
        ? Vector4f(1.0f, 0.45f, 0.05f, 1.0f)
        : Vector4f(0.95f, 0.12f, 0.45f, 1.0f); // Octarian Magenta

    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Rival00.szs")) {
        const BfresModel* realModel = szsParser.getModel(0);
        if (realModel && !realModel->meshes.empty()) {
            BfresModel copy = *realModel;
            if (name) copy.name = name;
            f32 scale = 1.8f / 15.17f;
            for (auto& m : copy.meshes) {
                for (auto& v : m.vertices) {
                    v.position = (v.position - Vector3f(0.0f, -2.0f, 0.0f)) * scale;
                    if (m.name.find("Hair") != std::string::npos || m.name.find("hair") != std::string::npos ||
                        m.name.find("Oct") != std::string::npos) {
                        v.color = teamColor;
                    }
                }
            }
            return copy;
        }
    }
    return createInklingHumanModel(name, teamId);
}

BfresModel BfresParser::createCallieModel(const char* name) {
    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Npc_IdolA.szs")) {
        const BfresModel* realModel = szsParser.getModel(0);
        if (realModel && !realModel->meshes.empty()) {
            BfresModel copy = *realModel;
            if (name) copy.name = name;
            f32 scale = 1.8f / 14.8f;
            for (auto& m : copy.meshes) {
                for (auto& v : m.vertices) {
                    v.position = (v.position - Vector3f(0.0f, -0.5f, 0.0f)) * scale;
                }
            }
            return copy;
        }
    }
    return createInklingHumanModel(name ? name : "Npc_IdolA", 0);
}

BfresModel BfresParser::createMarieModel(const char* name) {
    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Npc_IdolB.szs")) {
        const BfresModel* realModel = szsParser.getModel(0);
        if (realModel && !realModel->meshes.empty()) {
            BfresModel copy = *realModel;
            if (name) copy.name = name;
            f32 scale = 1.8f / 14.8f;
            for (auto& m : copy.meshes) {
                for (auto& v : m.vertices) {
                    v.position = (v.position - Vector3f(0.0f, -0.5f, 0.0f)) * scale;
                }
            }
            return copy;
        }
    }
    return createInklingHumanModel(name ? name : "Npc_IdolB", 1);
}

BfresModel BfresParser::createSpykeModel(const char* name) {
    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Npc_CustomShop.szs")) {
        const BfresModel* realModel = szsParser.getModel(0);
        if (realModel && !realModel->meshes.empty()) {
            BfresModel copy = *realModel;
            if (name) copy.name = name;
            f32 scale = 0.12f;
            for (auto& m : copy.meshes) {
                for (auto& v : m.vertices) {
                    v.position = v.position * scale;
                }
            }
            return copy;
        }
    }
    return createProceduralCube(name ? name : "Npc_CustomShop", 1.5f);
}

BfresModel BfresParser::createJuddModel(const char* name) {
    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Npc_Judge.szs")) {
        const BfresModel* realModel = szsParser.getModel(0);
        if (realModel && !realModel->meshes.empty()) {
            BfresModel copy = *realModel;
            if (name) copy.name = name;
            f32 scale = 0.14f;
            for (auto& m : copy.meshes) {
                for (auto& v : m.vertices) {
                    v.position = v.position * scale;
                }
            }
            return copy;
        }
    }
    return createProceduralCube(name ? name : "Npc_Judge", 1.2f);
}

BfresModel BfresParser::createSheldonModel(const char* name) {
    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Npc_WeaponsShop.szs")) {
        const BfresModel* realModel = szsParser.getModel(0);
        if (realModel && !realModel->meshes.empty()) {
            BfresModel copy = *realModel;
            if (name) copy.name = name;
            f32 scale = 0.14f;
            for (auto& m : copy.meshes) {
                for (auto& v : m.vertices) {
                    v.position = v.position * scale;
                }
            }
            return copy;
        }
    }
    return createProceduralCube(name ? name : "Npc_WeaponsShop", 1.2f);
}

BfresModel BfresParser::createSplatBombModel(const char* name, u32 teamId) {
    Vector4f teamColor = (teamId == 0)
        ? Vector4f(1.0f, 0.45f, 0.05f, 1.0f)
        : Vector4f(0.05f, 0.85f, 0.95f, 1.0f);

    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Wsb_Bomb_Throw.szs")) {
        const BfresModel* realModel = szsParser.getModel(0);
        if (realModel && !realModel->meshes.empty()) {
            BfresModel copy = *realModel;
            if (name) copy.name = name;
            f32 scale = 0.15f;
            for (auto& m : copy.meshes) {
                for (auto& v : m.vertices) {
                    v.position = v.position * scale;
                    v.color = teamColor;
                }
            }
            return copy;
        }
    }
    return createInkBulletModel(name ? name : "Wsb_Bomb_Throw", 0.35f);
}

BfresModel BfresParser::createBurstBombModel(const char* name, u32 teamId) {
    Vector4f teamColor = (teamId == 0)
        ? Vector4f(1.0f, 0.45f, 0.05f, 1.0f)
        : Vector4f(0.05f, 0.85f, 0.95f, 1.0f);

    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Wsb_Bomb_Handy.szs")) {
        const BfresModel* realModel = szsParser.getModel(0);
        if (realModel && !realModel->meshes.empty()) {
            BfresModel copy = *realModel;
            if (name) copy.name = name;
            f32 scale = 0.15f;
            for (auto& m : copy.meshes) {
                for (auto& v : m.vertices) {
                    v.position = v.position * scale;
                    v.color = teamColor;
                }
            }
            return copy;
        }
    }
    return createInkBulletModel(name ? name : "Wsb_Bomb_Handy", 0.30f);
}

BfresModel BfresParser::createSplashWallModel(const char* name, u32 teamId) {
    Vector4f teamColor = (teamId == 0)
        ? Vector4f(1.0f, 0.45f, 0.05f, 1.0f)
        : Vector4f(0.05f, 0.85f, 0.95f, 1.0f);

    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Wsb_Shield.szs")) {
        const BfresModel* realModel = szsParser.getModel(0);
        if (realModel && !realModel->meshes.empty()) {
            BfresModel copy = *realModel;
            if (name) copy.name = name;
            f32 scale = 0.18f;
            for (auto& m : copy.meshes) {
                for (auto& v : m.vertices) {
                    v.position = v.position * scale;
                    v.color = teamColor;
                }
            }
            return copy;
        }
    }
    return createProceduralCube(name ? name : "Wsb_Shield", 0.8f);
}

BfresModel BfresParser::createSprinklerModel(const char* name, u32 teamId) {
    Vector4f teamColor = (teamId == 0)
        ? Vector4f(1.0f, 0.45f, 0.05f, 1.0f)
        : Vector4f(0.05f, 0.85f, 0.95f, 1.0f);

    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Wsb_Sprinkler.szs")) {
        const BfresModel* realModel = szsParser.getModel(0);
        if (realModel && !realModel->meshes.empty()) {
            BfresModel copy = *realModel;
            if (name) copy.name = name;
            f32 scale = 0.18f;
            for (auto& m : copy.meshes) {
                for (auto& v : m.vertices) {
                    v.position = v.position * scale;
                    v.color = teamColor;
                }
            }
            return copy;
        }
    }
    return createProceduralCube(name ? name : "Wsb_Sprinkler", 0.6f);
}

BfresModel BfresParser::createInkstrikeModel(const char* name, u32 teamId) {
    Vector4f teamColor = (teamId == 0)
        ? Vector4f(1.0f, 0.45f, 0.05f, 1.0f)
        : Vector4f(0.05f, 0.85f, 0.95f, 1.0f);

    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Wsp_Tornado.szs")) {
        const BfresModel* realModel = szsParser.getModel(0);
        if (realModel && !realModel->meshes.empty()) {
            BfresModel copy = *realModel;
            if (name) copy.name = name;
            f32 scale = 0.20f;
            for (auto& m : copy.meshes) {
                for (auto& v : m.vertices) {
                    v.position = v.position * scale;
                    v.color = teamColor;
                }
            }
            return copy;
        }
    }
    return createProceduralCube(name ? name : "Wsp_Tornado", 1.5f);
}

BfresModel BfresParser::createOctostompModel(const char* name) {
    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Enm_Stamp.szs")) {
        BfresModel combined;
        combined.name = name ? name : "Enm_Stamp";
        for (size_t i = 0; i < szsParser.getModelCount(); ++i) {
            const BfresModel* part = szsParser.getModel(i);
            if (part) {
                for (const auto& mesh : part->meshes) {
                    combined.meshes.push_back(mesh);
                }
            }
        }
        if (!combined.meshes.empty()) {
            return combined;
        }
    }
    return createProceduralCube(name ? name : "Enm_Stamp", 3.0f);
}

BfresModel BfresParser::createOctocopterModel(const char* name) {
    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Enm_Takopter.szs")) {
        const BfresModel* realModel = szsParser.getModel(0);
        if (realModel && !realModel->meshes.empty()) {
            BfresModel copy = *realModel;
            if (name) copy.name = name;
            return copy;
        }
    }
    return createProceduralCube(name ? name : "Enm_Takopter", 1.2f);
}

BfresModel BfresParser::createSqueeGModel(const char* name) {
    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Enm_Cleaner.szs")) {
        const BfresModel* realModel = szsParser.getModel(0);
        if (realModel && !realModel->meshes.empty()) {
            BfresModel copy = *realModel;
            if (name) copy.name = name;
            return copy;
        }
    }
    return createProceduralCube(name ? name : "Enm_Cleaner", 1.0f);
}

BfresModel BfresParser::createSparrowModel(const char* name) {
    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Brd_Sparrow00.szs")) {
        const BfresModel* realModel = szsParser.getModel(0);
        if (realModel && !realModel->meshes.empty()) {
            BfresModel copy = *realModel;
            if (name) copy.name = name;
            return copy;
        }
    }
    return createProceduralCube(name ? name : "Brd_Sparrow00", 0.4f);
}

BfresModel BfresParser::createAerosprayModel(const char* name, u32 teamId) {
    (void)teamId;
    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Wmn_Shot_Blaze.szs")) {
        BfresModel combined;
        combined.name = name ? name : "Wmn_Shot_Blaze";
        for (size_t i = 0; i < szsParser.getModelCount(); ++i) {
            const BfresModel* part = szsParser.getModel(i);
            if (part) {
                for (const auto& mesh : part->meshes) {
                    combined.meshes.push_back(mesh);
                }
            }
        }
        if (!combined.meshes.empty()) return combined;
    }
    return createSplattershotModel(name ? name : "Wmn_Shot_Blaze", teamId);
}

BfresModel BfresParser::createEliter3KModel(const char* name, u32 teamId) {
    (void)teamId;
    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Wmn_Charge_Long.szs")) {
        BfresModel combined;
        combined.name = name ? name : "Wmn_Charge_Long";
        for (size_t i = 0; i < szsParser.getModelCount(); ++i) {
            const BfresModel* part = szsParser.getModel(i);
            if (part) {
                for (const auto& mesh : part->meshes) {
                    combined.meshes.push_back(mesh);
                }
            }
        }
        if (!combined.meshes.empty()) return combined;
    }
    return createSplatChargerModel(name ? name : "Wmn_Charge_Long", teamId);
}

BfresModel BfresParser::createOctobrushModel(const char* name, u32 teamId) {
    (void)teamId;
    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Wmn_Roller_BrushNormal.szs")) {
        BfresModel combined;
        combined.name = name ? name : "Wmn_Roller_BrushNormal";
        for (size_t i = 0; i < szsParser.getModelCount(); ++i) {
            const BfresModel* part = szsParser.getModel(i);
            if (part) {
                for (const auto& mesh : part->meshes) {
                    combined.meshes.push_back(mesh);
                }
            }
        }
        if (!combined.meshes.empty()) return combined;
    }
    return createSplatRollerModel(name ? name : "Wmn_Roller_BrushNormal", teamId);
}

BfresModel BfresParser::createInkstrikeMonitorModel(const char* name) {
    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Wsp_Tornado_Monitor.szs")) {
        BfresModel combined;
        combined.name = name ? name : "Wsp_Tornado_Monitor";
        for (size_t i = 0; i < szsParser.getModelCount(); ++i) {
            const BfresModel* part = szsParser.getModel(i);
            if (part) {
                for (const auto& mesh : part->meshes) {
                    combined.meshes.push_back(mesh);
                }
            }
        }
        if (!combined.meshes.empty()) return combined;
    }
    return createProceduralCube(name ? name : "Wsp_Tornado_Monitor", 0.5f);
}

BfresModel BfresParser::createHeroTankModel(const char* name, u32 teamId) {
    (void)teamId;
    BfresParser szsParser;
    if (szsParser.loadFromSzsFile("content/Model/Tnk_Msn0Lv0.szs")) {
        BfresModel combined;
        combined.name = name ? name : "Tnk_Msn0Lv0";
        for (size_t i = 0; i < szsParser.getModelCount(); ++i) {
            const BfresModel* part = szsParser.getModel(i);
            if (part) {
                for (const auto& mesh : part->meshes) {
                    combined.meshes.push_back(mesh);
                }
            }
        }
        if (!combined.meshes.empty()) return combined;
    }
    return createProceduralCube(name ? name : "Tnk_Msn0Lv0", 0.8f);
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
