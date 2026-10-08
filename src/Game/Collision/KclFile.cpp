#include "Game/Collision/KclFile.h"
#include "sead/resource/seadResource.h"
#include <cmath>
#include <algorithm>
#include <cstring>

namespace Game {

KclFile::KclFile()
    : mMinBounds(0.0f, 0.0f, 0.0f)
    , mMaxBounds(0.0f, 0.0f, 0.0f)
    , mThickness(0.0f)
    , mIsBigEndian(true)
{
}

KclFile::~KclFile() {
    clear();
}

void KclFile::clear() {
    mPositions.clear();
    mNormals.clear();
    mPrisms.clear();
    mOctree.clear();
}

bool KclFile::load(const u8* data, size_t size) {
    clear();
    if (!data || size < sizeof(KclHeaderRaw)) {
        printf("[-] KclFile::load failed: data=%p, size=%zu < sizeof(KclHeaderRaw)=%zu\n", data, size, sizeof(KclHeaderRaw));
        return false;
    }

    // Splatoon KCL container detection:
    // If the file begins with the model collision wrapper (0x02020000),
    // offset 0x58 points to the inner KCL collision data (typically 0x5C).
    if (size >= 0x60 && data[0] == 0x02 && data[1] == 0x02) {
        u32 subOff = sead::Endian::toHostU32(*reinterpret_cast<const u32*>(data + 0x58), true);
        if (subOff >= 0x38 && subOff < size) {
            data += subOff;
            size -= subOff;
        } else {
            data += 0x5C;
            size -= 0x5C;
        }
    }

    // Detect endianness: KCL headers on Wii U are Big Endian
    // Position offset is typically 0x38 or 0x3C or 0x40
    u32 testPosOff = *reinterpret_cast<const u32*>(data);
    if (testPosOff > 0x00010000) {
        mIsBigEndian = true;
    } else {
        mIsBigEndian = false;
    }

    const KclHeaderRaw* hdr = reinterpret_cast<const KclHeaderRaw*>(data);

    u32 posOff = sead::Endian::toHostU32(hdr->posTableOffset, mIsBigEndian);
    u32 nrmOff = sead::Endian::toHostU32(hdr->nrmTableOffset, mIsBigEndian);
    u32 prismOff = sead::Endian::toHostU32(hdr->prismTableOffset, mIsBigEndian);
    u32 octreeOff = sead::Endian::toHostU32(hdr->octreeTableOffset, mIsBigEndian);

    if (posOff >= size || nrmOff >= size || prismOff >= size) {
        return false;
    }

    mThickness = sead::Endian::toHostF32(hdr->thickness, mIsBigEndian);
    mMinBounds.x = sead::Endian::toHostF32(hdr->minCoord.x, mIsBigEndian);
    mMinBounds.y = sead::Endian::toHostF32(hdr->minCoord.y, mIsBigEndian);
    mMinBounds.z = sead::Endian::toHostF32(hdr->minCoord.z, mIsBigEndian);

    // 1. Load Positions
    size_t posBytes = (nrmOff > posOff) ? (nrmOff - posOff) : 0;
    size_t numPositions = posBytes / sizeof(sead::Vector3f);
    mPositions.reserve(numPositions);
    const sead::Vector3f* posSrc = reinterpret_cast<const sead::Vector3f*>(data + posOff);
    for (size_t i = 0; i < numPositions; ++i) {
        sead::Vector3f v;
        v.x = sead::Endian::toHostF32(posSrc[i].x, mIsBigEndian);
        v.y = sead::Endian::toHostF32(posSrc[i].y, mIsBigEndian);
        v.z = sead::Endian::toHostF32(posSrc[i].z, mIsBigEndian);
        mPositions.push_back(v);
    }

    if (!mPositions.empty()) {
        mMinBounds = mPositions[0];
        mMaxBounds = mPositions[0];
        for (const auto& p : mPositions) {
            mMinBounds.x = (std::min)(mMinBounds.x, p.x);
            mMinBounds.y = (std::min)(mMinBounds.y, p.y);
            mMinBounds.z = (std::min)(mMinBounds.z, p.z);
            mMaxBounds.x = (std::max)(mMaxBounds.x, p.x);
            mMaxBounds.y = (std::max)(mMaxBounds.y, p.y);
            mMaxBounds.z = (std::max)(mMaxBounds.z, p.z);
        }
    }

    // 2. Load Normals
    size_t nrmBytes = (prismOff > nrmOff) ? (prismOff - nrmOff) : 0;
    size_t numNormals = nrmBytes / sizeof(sead::Vector3f);
    mNormals.reserve(numNormals);
    const sead::Vector3f* nrmSrc = reinterpret_cast<const sead::Vector3f*>(data + nrmOff);
    for (size_t i = 0; i < numNormals; ++i) {
        sead::Vector3f n;
        n.x = sead::Endian::toHostF32(nrmSrc[i].x, mIsBigEndian);
        n.y = sead::Endian::toHostF32(nrmSrc[i].y, mIsBigEndian);
        n.z = sead::Endian::toHostF32(nrmSrc[i].z, mIsBigEndian);
        mNormals.push_back(n);
    }

    // 3. Load Prisms
    size_t prismBytes = (octreeOff > prismOff) ? (octreeOff - prismOff) : (size - prismOff);
    bool is20Byte = (prismBytes % 20 == 0);
    size_t prismStride = is20Byte ? 20 : 16;
    size_t numPrisms = prismBytes / prismStride;
    mPrisms.reserve(numPrisms);

    for (size_t i = 0; i < numPrisms; ++i) {
        const u8* src = data + prismOff + i * prismStride;
        KclPrismRaw p;
        p.length    = sead::Endian::toHostF32(*reinterpret_cast<const f32*>(src), mIsBigEndian);
        p.posIndex  = sead::Endian::toHostU16(*reinterpret_cast<const u16*>(src + 4), mIsBigEndian);
        p.dirIndex  = sead::Endian::toHostU16(*reinterpret_cast<const u16*>(src + 6), mIsBigEndian);
        p.nrmAIndex = sead::Endian::toHostU16(*reinterpret_cast<const u16*>(src + 8), mIsBigEndian);
        p.nrmBIndex = sead::Endian::toHostU16(*reinterpret_cast<const u16*>(src + 10), mIsBigEndian);
        p.nrmCIndex = sead::Endian::toHostU16(*reinterpret_cast<const u16*>(src + 12), mIsBigEndian);
        p.attribute = sead::Endian::toHostU16(*reinterpret_cast<const u16*>(src + 14), mIsBigEndian);
        p.flags     = is20Byte ? sead::Endian::toHostU32(*reinterpret_cast<const u32*>(src + 16), mIsBigEndian) : 0;
        mPrisms.push_back(p);
    }

    return true;
}

// Ray-triangle intersection test (Möller-Trumbore)
static bool IntersectRayTriangle(
    const sead::Vector3f& orig, const sead::Vector3f& dir,
    const sead::Vector3f& v0, const sead::Vector3f& v1, const sead::Vector3f& v2,
    f32& t, f32& u, f32& v
) {
    const f32 EPSILON = 0.000001f;
    sead::Vector3f edge1 = v1 - v0;
    sead::Vector3f edge2 = v2 - v0;

    // Cross product dir x edge2
    sead::Vector3f pvec(
        dir.y * edge2.z - dir.z * edge2.y,
        dir.z * edge2.x - dir.x * edge2.z,
        dir.x * edge2.y - dir.y * edge2.x
    );

    // Dot product edge1 . pvec
    f32 det = edge1.x * pvec.x + edge1.y * pvec.y + edge1.z * pvec.z;
    if (det > -EPSILON && det < EPSILON) return false;
    f32 invDet = 1.0f / det;

    sead::Vector3f tvec = orig - v0;
    u = (tvec.x * pvec.x + tvec.y * pvec.y + tvec.z * pvec.z) * invDet;
    if (u < 0.0f || u > 1.0f) return false;

    // Cross product tvec x edge1
    sead::Vector3f qvec(
        tvec.y * edge1.z - tvec.z * edge1.y,
        tvec.z * edge1.x - tvec.x * edge1.z,
        tvec.x * edge1.y - tvec.y * edge1.x
    );

    v = (dir.x * qvec.x + dir.y * qvec.y + dir.z * qvec.z) * invDet;
    if (v < 0.0f || u + v > 1.0f) return false;

    t = (edge2.x * qvec.x + edge2.y * qvec.y + edge2.z * qvec.z) * invDet;
    return (t > EPSILON);
}

bool KclFile::raycast(const sead::Vector3f& origin, const sead::Vector3f& direction, f32 maxDist, KclHitResult& outHit) const {
    outHit.hit = false;
    outHit.distance = maxDist;

    if (mPrisms.empty() || mPositions.empty()) return false;

    f32 closestDist = maxDist;
    size_t closestIdx = 0;
    sead::Vector3f bestHitPoint;
    sead::Vector3f bestHitNormal;

    for (size_t i = 0; i < mPrisms.size(); ++i) {
        const auto& p = mPrisms[i];
        if (p.posIndex >= mPositions.size() || p.dirIndex >= mNormals.size()) continue;

        const sead::Vector3f& v0 = mPositions[p.posIndex];
        const sead::Vector3f& nrm = mNormals[p.dirIndex];

        // Reconstruct triangle points from prism normal components
        if (p.nrmAIndex >= mNormals.size() || p.nrmBIndex >= mNormals.size()) continue;
        const sead::Vector3f& nA = mNormals[p.nrmAIndex];
        const sead::Vector3f& nB = mNormals[p.nrmBIndex];

        // Cross product nA x nB gives triangle base
        sead::Vector3f cross(
            nA.y * nB.z - nA.z * nB.y,
            nA.z * nB.x - nA.x * nB.z,
            nA.x * nB.y - nA.y * nB.x
        );

        sead::Vector3f v1 = v0 + cross * (p.length > 0.0f ? p.length : 1.0f);
        sead::Vector3f v2 = v0 + nrm * (p.length > 0.0f ? p.length : 1.0f);

        f32 t = 0.0f, u = 0.0f, v = 0.0f;
        if (IntersectRayTriangle(origin, direction, v0, v1, v2, t, u, v)) {
            if (t < closestDist) {
                closestDist = t;
                closestIdx = i;
                bestHitPoint = origin + direction * t;
                bestHitNormal = nrm;
                outHit.hit = true;
            }
        }
    }

    if (outHit.hit) {
        outHit.distance = closestDist;
        outHit.hitPoint = bestHitPoint;
        outHit.hitNormal = bestHitNormal;
        outHit.attribute = mPrisms[closestIdx].attribute;
        return true;
    }
    return false;
}

bool KclFile::checkSphere(const sead::Vector3f& center, f32 radius, KclHitResult& outHit) const {
    outHit.hit = false;
    f32 minD = radius;

    for (const auto& p : mPrisms) {
        if (p.posIndex >= mPositions.size()) continue;
        const sead::Vector3f& v = mPositions[p.posIndex];

        f32 dx = center.x - v.x;
        f32 dy = center.y - v.y;
        f32 dz = center.z - v.z;
        f32 dSq = dx * dx + dy * dy + dz * dz;

        if (dSq < radius * radius) {
            f32 d = std::sqrt(dSq);
            if (d < minD) {
                minD = d;
                outHit.hit = true;
                outHit.distance = d;
                outHit.hitPoint = v;
                if (p.dirIndex < mNormals.size()) {
                    outHit.hitNormal = mNormals[p.dirIndex];
                } else {
                    outHit.hitNormal.set(0.0f, 1.0f, 0.0f);
                }
                outHit.attribute = p.attribute;
            }
        }
    }
    return outHit.hit;
}

sead::BfresModel KclFile::toBfresModel(const char* modelName) const {
    sead::BfresModel model;
    model.name = modelName ? modelName : "StageCollisionMesh";

    // Material 0: Floor / Ground (Paintable concrete)
    sead::BfresMaterial floorMat;
    floorMat.name = "M_StageFloor";
    floorMat.shaderName = "splatoon_stage";
    floorMat.teamColor = sead::Vector4f(0.55f, 0.58f, 0.62f, 1.0f);
    model.materials.push_back(floorMat);

    // Material 1: Wall (Paintable wall)
    sead::BfresMaterial wallMat;
    wallMat.name = "M_StageWall";
    wallMat.shaderName = "splatoon_stage";
    wallMat.teamColor = sead::Vector4f(0.42f, 0.45f, 0.48f, 1.0f);
    model.materials.push_back(wallMat);

    // Material 2: Grate / Metal mesh (Uninkable catwalk)
    sead::BfresMaterial grateMat;
    grateMat.name = "M_StageGrate";
    grateMat.shaderName = "splatoon_stage";
    grateMat.teamColor = sead::Vector4f(0.75f, 0.70f, 0.35f, 1.0f);
    model.materials.push_back(grateMat);

    sead::BfresMesh floorMesh;
    floorMesh.name = "SubMesh_StageFloor";
    floorMesh.materialIndex = 0;

    sead::BfresMesh wallMesh;
    wallMesh.name = "SubMesh_StageWall";
    wallMesh.materialIndex = 1;

    sead::BfresMesh grateMesh;
    grateMesh.name = "SubMesh_StageGrate";
    grateMesh.materialIndex = 2;

    for (size_t i = 0; i < mPrisms.size(); ++i) {
        const auto& p = mPrisms[i];
        if (p.posIndex >= mPositions.size() || p.dirIndex >= mNormals.size()) continue;
        if (p.nrmAIndex >= mNormals.size() || p.nrmBIndex >= mNormals.size() || p.nrmCIndex >= mNormals.size()) continue;

        const sead::Vector3f& v0 = mPositions[p.posIndex];
        const sead::Vector3f& fn = mNormals[p.dirIndex];
        const sead::Vector3f& na = mNormals[p.nrmAIndex];
        const sead::Vector3f& nb = mNormals[p.nrmBIndex];
        const sead::Vector3f& nc = mNormals[p.nrmCIndex];

        sead::Vector3f crossA = fn.cross(na);
        sead::Vector3f crossB = fn.cross(nb);
        f32 denomA = nc.dot(crossA);
        f32 denomB = nc.dot(crossB);
        if (std::abs(denomA) < 1e-6f || std::abs(denomB) < 1e-6f) continue;

        sead::Vector3f v1 = v0 + crossB * (p.length / denomB);
        sead::Vector3f v2 = v0 + crossA * (p.length / denomA);

        sead::BfresMesh* targetMesh = &floorMesh;
        sead::Vector4f vertColor(0.85f, 0.85f, 0.85f, 1.0f);

        if ((p.attribute & 0x0020) != 0 || (p.attribute & 0x0040) != 0) { // Grate / Uninkable
            targetMesh = &grateMesh;
            vertColor = sead::Vector4f(0.95f, 0.85f, 0.35f, 1.0f);
        } else if ((p.attribute & 0x0003) == cKclAttr_Wall || (p.attribute & 0x0003) == cKclAttr_SteepSlope) { // Wall
            targetMesh = &wallMesh;
            vertColor = sead::Vector4f(0.55f, 0.60f, 0.65f, 1.0f);
        } else { // Floor
            targetMesh = &floorMesh;
            vertColor = sead::Vector4f(0.85f, 0.88f, 0.90f, 1.0f);
        }

        u32 baseIdx = static_cast<u32>(targetMesh->vertices.size());

        f32 rangeX = (mMaxBounds.x > mMinBounds.x) ? (mMaxBounds.x - mMinBounds.x) : 100.0f;
        f32 rangeZ = (mMaxBounds.z > mMinBounds.z) ? (mMaxBounds.z - mMinBounds.z) : 100.0f;
        if (rangeX < 1.0f) rangeX = 100.0f;
        if (rangeZ < 1.0f) rangeZ = 100.0f;

        auto calcUv = [&](const sead::Vector3f& pos) -> sead::Vector2f {
            return sead::Vector2f((pos.x - mMinBounds.x) / rangeX, (pos.z - mMinBounds.z) / rangeZ);
        };

        sead::BfresVertex vert0, vert1, vert2;
        vert0.position = v0; vert0.normal = fn; vert0.uv = calcUv(v0); vert0.color = vertColor;
        vert1.position = v1; vert1.normal = fn; vert1.uv = calcUv(v1); vert1.color = vertColor;
        vert2.position = v2; vert2.normal = fn; vert2.uv = calcUv(v2); vert2.color = vertColor;

        targetMesh->vertices.push_back(vert0);
        targetMesh->vertices.push_back(vert1);
        targetMesh->vertices.push_back(vert2);

        targetMesh->indices.push_back(baseIdx + 0);
        targetMesh->indices.push_back(baseIdx + 1);
        targetMesh->indices.push_back(baseIdx + 2);
    }

    if (!floorMesh.vertices.empty()) model.meshes.push_back(floorMesh);
    if (!wallMesh.vertices.empty())  model.meshes.push_back(wallMesh);
    if (!grateMesh.vertices.empty()) model.meshes.push_back(grateMesh);

    return model;
}

bool KclFile::loadFromSzsFile(const char* szsFilePath) {
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

    sead::SarcArchive sarc;
    if (!sarc.load(buffer.data(), buffer.size())) return false;

    std::string baseName;
    const char* slash = strrchr(szsFilePath, '/');
    if (!slash) slash = strrchr(szsFilePath, '\\');
    const char* start = slash ? slash + 1 : szsFilePath;
    const char* dot = strrchr(start, '.');
    if (dot) baseName.assign(start, dot);
    else baseName = start;

    std::string targetKcl = baseName + ".kcl";
    const sead::SarcFileInfo* bestKcl = nullptr;
    size_t bestSize = 0;

    for (size_t i = 0; i < sarc.getFileCount(); ++i) {
        const auto* info = sarc.getFileInfo(i);
        if (!info) continue;
        if (info->name == targetKcl) {
            bestKcl = info;
            break;
        }
        if (info->name.find(".kcl") != std::string::npos && info->size > bestSize) {
            bestKcl = info;
            bestSize = info->size;
        }
    }

    if (bestKcl) {
        return load(bestKcl->data, bestKcl->size);
    }
    return false;
}

} // namespace Game
