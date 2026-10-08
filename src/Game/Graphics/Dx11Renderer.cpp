#include "Game/Graphics/Dx11Renderer.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <cstdio>
#include <cstring>
#include <algorithm>

namespace Game {

static const char* s_ShaderSource = R"(
cbuffer FrameConstants : register(b0) {
    row_major matrix gViewProj;
    float4 gCameraPos;
    float4 gLightDir;
    float  gTime;
    float3 gPadding0;
};

cbuffer ObjectConstants : register(b1) {
    row_major matrix gWorld;
    float4 gTeamColor;
    uint   gTeamId;
    uint   gFlags;
    float2 gPadding1;
};

struct VS_INPUT {
    float3 position : POSITION;
    float3 normal   : NORMAL;
    float2 uv       : TEXCOORD0;
    float3 tangent  : TANGENT;
    float4 color    : COLOR;
};

struct PS_INPUT {
    float4 posH     : SV_POSITION;
    float3 worldPos : POSITION;
    float3 normalW  : NORMAL;
    float2 uv       : TEXCOORD0;
    float4 color    : COLOR;
};

PS_INPUT VSMain(VS_INPUT input) {
    PS_INPUT output;
    float4 pos = float4(input.position, 1.0f);
    float4 worldPos;
    worldPos.x = dot(gWorld[0], pos);
    worldPos.y = dot(gWorld[1], pos);
    worldPos.z = dot(gWorld[2], pos);
    worldPos.w = dot(gWorld[3], pos);

    output.worldPos = worldPos.xyz;

    output.posH.x = dot(gViewProj[0], worldPos);
    output.posH.y = dot(gViewProj[1], worldPos);
    output.posH.z = dot(gViewProj[2], worldPos);
    output.posH.w = dot(gViewProj[3], worldPos);

    float3 norm;
    norm.x = dot(gWorld[0].xyz, input.normal);
    norm.y = dot(gWorld[1].xyz, input.normal);
    norm.z = dot(gWorld[2].xyz, input.normal);
    output.normalW = normalize(norm);

    output.uv = input.uv;
    output.color = input.color;
    return output;
}

Texture2D gInkTexture : register(t1);
SamplerState gSampler : register(s0);

float4 PSMain(PS_INPUT input) : SV_TARGET {
    float3 lightDir = normalize(gLightDir.xyz);
    float3 norm = normalize(input.normalW);
    float diff = max(dot(norm, lightDir), 0.20f);

    float4 inkSample = gInkTexture.Sample(gSampler, input.uv);
    float3 baseColor = gTeamColor.rgb * input.color.rgb;

    // Projective dynamic ink surface blending
    if (inkSample.a > 0.05f) {
        float3 inkColor = inkSample.rgb;
        baseColor = lerp(baseColor, inkColor, inkSample.a);

        // Dynamic wet ink specular highlight
        float3 viewDir = normalize(gCameraPos.xyz - input.worldPos);
        float3 halfVec = normalize(lightDir + viewDir);
        float spec = pow(max(dot(norm, halfVec), 0.0f), 32.0f) * 0.8f;
        return float4(baseColor * diff + spec, 1.0f);
    }

    // Default stage diffuse
    return float4(baseColor * diff, 1.0f);
}
)";

Dx11Renderer::Dx11Renderer()
    : mInitialized(false)
    , mHeadless(true)
    , mHwnd(nullptr)
    , mWidth(1280)
    , mHeight(720)
    , mInFrame(false)
    , mDevice(nullptr)
    , mImmediateContext(nullptr)
    , mSwapChain(nullptr)
    , mRenderTargetView(nullptr)
    , mDepthStencilView(nullptr)
    , mDepthStencilState(nullptr)
    , mRasterizerState(nullptr)
    , mVertexShader(nullptr)
    , mPixelShader(nullptr)
    , mInputLayout(nullptr)
    , mCbFrame(nullptr)
    , mCbObject(nullptr)
    , mDynamicVertexBuffer(nullptr)
    , mDynamicIndexBuffer(nullptr)
    , mMaxVertexBufferSize(0)
    , mMaxIndexBufferSize(0)
    , mPaintTexture(nullptr)
    , mPaintSrv(nullptr)
    , mSamplerState(nullptr)
{
    mClearColor[0] = 0.08f;
    mClearColor[1] = 0.12f;
    mClearColor[2] = 0.18f;
    mClearColor[3] = 1.0f;
    mStats.reset();
}

Dx11Renderer::~Dx11Renderer() {
    shutdown();
}

bool Dx11Renderer::initPipeline(void* hWnd, u32 width, u32 height, bool headless) {
    shutdown();

    mHwnd = hWnd;
    mWidth = (width > 0) ? width : 1280;
    mHeight = (height > 0) ? height : 720;
    mHeadless = headless || (hWnd == nullptr);

    mViewport.x = 0.0f;
    mViewport.y = 0.0f;
    mViewport.width = static_cast<f32>(mWidth);
    mViewport.height = static_cast<f32>(mHeight);
    mViewport.minDepth = 0.0f;
    mViewport.maxDepth = 1.0f;

    mStats.reset();
    mInFrame = false;

    if (!mHeadless && mHwnd) {
        if (!initD3D11()) {
            printf("[-] Dx11Renderer: Failed to initialize hardware D3D11 device, falling back to headless.\n");
            mHeadless = true;
        }
    }

    mInitialized = true;
    return true;
}

bool Dx11Renderer::initD3D11() {
    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(scd));
    scd.BufferCount = 1;
    scd.BufferDesc.Width = mWidth;
    scd.BufferDesc.Height = mHeight;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = static_cast<HWND>(mHwnd);
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };
    D3D_FEATURE_LEVEL featureLevel;

    UINT createDeviceFlags = 0;
#if defined(_DEBUG)
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createDeviceFlags,
        featureLevels,
        ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION,
        &scd,
        &mSwapChain,
        &mDevice,
        &featureLevel,
        &mImmediateContext
    );

    if (FAILED(hr)) {
        // Fallback to WARP software rasterizer if hardware adapter unavailable
        hr = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_WARP,
            nullptr,
            createDeviceFlags,
            featureLevels,
            ARRAYSIZE(featureLevels),
            D3D11_SDK_VERSION,
            &scd,
            &mSwapChain,
            &mDevice,
            &featureLevel,
            &mImmediateContext
        );
    }

    if (FAILED(hr)) {
        return false;
    }

    // Create RenderTargetView
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
    if (FAILED(hr)) return false;

    hr = mDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr)) return false;

    // Create Depth Stencil Buffer & View
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory(&descDepth, sizeof(descDepth));
    descDepth.Width = mWidth;
    descDepth.Height = mHeight;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ID3D11Texture2D* pDepthBuffer = nullptr;
    hr = mDevice->CreateTexture2D(&descDepth, nullptr, &pDepthBuffer);
    if (FAILED(hr)) return false;

    hr = mDevice->CreateDepthStencilView(pDepthBuffer, nullptr, &mDepthStencilView);
    pDepthBuffer->Release();
    if (FAILED(hr)) return false;

    // Depth Stencil State
    D3D11_DEPTH_STENCIL_DESC dsDesc;
    ZeroMemory(&dsDesc, sizeof(dsDesc));
    dsDesc.DepthEnable = TRUE;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
    hr = mDevice->CreateDepthStencilState(&dsDesc, &mDepthStencilState);
    if (FAILED(hr)) return false;

    // Rasterizer State
    D3D11_RASTERIZER_DESC rasterDesc;
    ZeroMemory(&rasterDesc, sizeof(rasterDesc));
    rasterDesc.CullMode = D3D11_CULL_NONE; // Cull none so both sides of Splatoon meshes render cleanly
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.DepthClipEnable = TRUE;
    hr = mDevice->CreateRasterizerState(&rasterDesc, &mRasterizerState);
    if (FAILED(hr)) return false;

    if (!initShaders()) return false;
    if (!initDynamicBuffers()) return false;

    return true;
}

bool Dx11Renderer::initShaders() {
    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;

    HRESULT hr = D3DCompile(
        s_ShaderSource,
        strlen(s_ShaderSource),
        "SplatoonShaders.hlsl",
        nullptr,
        nullptr,
        "VSMain",
        "vs_4_0",
        D3DCOMPILE_ENABLE_STRICTNESS,
        0,
        &vsBlob,
        &errorBlob
    );

    if (FAILED(hr)) {
        if (errorBlob) {
            printf("[-] Vertex Shader Error: %s\n", static_cast<const char*>(errorBlob->GetBufferPointer()));
            errorBlob->Release();
        }
        return false;
    }

    hr = mDevice->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &mVertexShader);
    if (FAILED(hr)) {
        vsBlob->Release();
        return false;
    }

    // Input Layout
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    hr = mDevice->CreateInputLayout(layout, ARRAYSIZE(layout), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &mInputLayout);
    vsBlob->Release();
    if (FAILED(hr)) return false;

    // Pixel Shader
    ID3DBlob* psBlob = nullptr;
    hr = D3DCompile(
        s_ShaderSource,
        strlen(s_ShaderSource),
        "SplatoonShaders.hlsl",
        nullptr,
        nullptr,
        "PSMain",
        "ps_4_0",
        D3DCOMPILE_ENABLE_STRICTNESS,
        0,
        &psBlob,
        &errorBlob
    );

    if (FAILED(hr)) {
        if (errorBlob) {
            printf("[-] Pixel Shader Error: %s\n", static_cast<const char*>(errorBlob->GetBufferPointer()));
            errorBlob->Release();
        }
        return false;
    }

    hr = mDevice->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &mPixelShader);
    psBlob->Release();
    if (FAILED(hr)) return false;

    return true;
}

bool Dx11Renderer::initDynamicBuffers() {
    // Constant Buffers
    D3D11_BUFFER_DESC cbDesc;
    ZeroMemory(&cbDesc, sizeof(cbDesc));
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    cbDesc.ByteWidth = sizeof(Dx11FrameConstants);
    HRESULT hr = mDevice->CreateBuffer(&cbDesc, nullptr, &mCbFrame);
    if (FAILED(hr)) return false;

    cbDesc.ByteWidth = sizeof(Dx11ObjectConstants);
    hr = mDevice->CreateBuffer(&cbDesc, nullptr, &mCbObject);
    if (FAILED(hr)) return false;

    // Sampler State for Ink Texture
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = mDevice->CreateSamplerState(&sampDesc, &mSamplerState);
    if (FAILED(hr)) return false;

    return true;
}

void Dx11Renderer::releaseD3D11() {
    if (mSamplerState)       { mSamplerState->Release(); mSamplerState = nullptr; }
    if (mPaintSrv)           { mPaintSrv->Release(); mPaintSrv = nullptr; }
    if (mPaintTexture)       { mPaintTexture->Release(); mPaintTexture = nullptr; }
    if (mDynamicIndexBuffer) { mDynamicIndexBuffer->Release(); mDynamicIndexBuffer = nullptr; }
    if (mDynamicVertexBuffer){ mDynamicVertexBuffer->Release(); mDynamicVertexBuffer = nullptr; }
    if (mCbObject)           { mCbObject->Release(); mCbObject = nullptr; }
    if (mCbFrame)            { mCbFrame->Release(); mCbFrame = nullptr; }
    if (mInputLayout)        { mInputLayout->Release(); mInputLayout = nullptr; }
    if (mPixelShader)        { mPixelShader->Release(); mPixelShader = nullptr; }
    if (mVertexShader)       { mVertexShader->Release(); mVertexShader = nullptr; }
    if (mRasterizerState)    { mRasterizerState->Release(); mRasterizerState = nullptr; }
    if (mDepthStencilState)  { mDepthStencilState->Release(); mDepthStencilState = nullptr; }
    if (mDepthStencilView)   { mDepthStencilView->Release(); mDepthStencilView = nullptr; }
    if (mRenderTargetView)   { mRenderTargetView->Release(); mRenderTargetView = nullptr; }
    if (mSwapChain)          { mSwapChain->Release(); mSwapChain = nullptr; }
    if (mImmediateContext)   { mImmediateContext->Release(); mImmediateContext = nullptr; }
    if (mDevice)             { mDevice->Release(); mDevice = nullptr; }
}

void Dx11Renderer::shutdown() {
    releaseD3D11();
    mInitialized = false;
    mInFrame = false;
    mStats.reset();
}

void Dx11Renderer::beginFrame(f32 r, f32 g, f32 b, f32 a) {
    if (!mInitialized) return;

    mClearColor[0] = r;
    mClearColor[1] = g;
    mClearColor[2] = b;
    mClearColor[3] = a;

    mStats.reset();
    mInFrame = true;

    if (!mHeadless && mImmediateContext && mRenderTargetView && mDepthStencilView) {
        mImmediateContext->ClearRenderTargetView(mRenderTargetView, mClearColor);
        mImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

        mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
        mImmediateContext->OMSetDepthStencilState(mDepthStencilState, 1);
        mImmediateContext->RSSetState(mRasterizerState);

        D3D11_VIEWPORT vp;
        vp.Width = static_cast<FLOAT>(mWidth);
        vp.Height = static_cast<FLOAT>(mHeight);
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        mImmediateContext->RSSetViewports(1, &vp);

        mImmediateContext->IASetInputLayout(mInputLayout);
        mImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        mImmediateContext->VSSetShader(mVertexShader, nullptr, 0);
        mImmediateContext->PSSetShader(mPixelShader, nullptr, 0);
        mImmediateContext->PSSetSamplers(0, 1, &mSamplerState);
    }
}

void Dx11Renderer::setFrameConstants(const sead::Matrix44f& viewProj, const sead::Vector3f& camPos, f32 time) {
    mFrameConstants.viewProjMatrix = viewProj;
    mFrameConstants.cameraPos = sead::Vector4f(camPos.x, camPos.y, camPos.z, 1.0f);
    mFrameConstants.gameTime = time;

    if (!mHeadless && mImmediateContext && mCbFrame) {
        D3D11_MAPPED_SUBRESOURCE mapped;
        HRESULT hr = mImmediateContext->Map(mCbFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
        if (SUCCEEDED(hr)) {
            memcpy(mapped.pData, &mFrameConstants, sizeof(Dx11FrameConstants));
            mImmediateContext->Unmap(mCbFrame, 0);
        }
        mImmediateContext->VSSetConstantBuffers(0, 1, &mCbFrame);
        mImmediateContext->PSSetConstantBuffers(0, 1, &mCbFrame);
    }
}

void Dx11Renderer::submitMesh(const sead::BfresMesh& mesh, const sead::Matrix44f& world, const sead::Vector4f& teamColor, u32 teamId) {
    if (!mInitialized || !mInFrame || mesh.vertices.empty()) return;

    mObjectConstants.worldMatrix = world;
    mObjectConstants.teamColor = teamColor;
    mObjectConstants.teamId = teamId;

    mStats.drawCalls++;
    mStats.verticesDrawn += static_cast<u32>(mesh.vertices.size());
    mStats.indicesDrawn += static_cast<u32>(mesh.indices.size());

    if (!mHeadless && mImmediateContext && mDevice) {
        // Update Object Constant Buffer
        if (mCbObject) {
            D3D11_MAPPED_SUBRESOURCE mapped;
            HRESULT hr = mImmediateContext->Map(mCbObject, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
            if (SUCCEEDED(hr)) {
                memcpy(mapped.pData, &mObjectConstants, sizeof(Dx11ObjectConstants));
                mImmediateContext->Unmap(mCbObject, 0);
            }
            mImmediateContext->VSSetConstantBuffers(1, 1, &mCbObject);
            mImmediateContext->PSSetConstantBuffers(1, 1, &mCbObject);
        }

        // Upload or re-allocate vertex buffer
        size_t requiredVbSize = mesh.vertices.size() * sizeof(sead::BfresVertex);
        if (!mDynamicVertexBuffer || mMaxVertexBufferSize < requiredVbSize) {
            if (mDynamicVertexBuffer) mDynamicVertexBuffer->Release();
            mMaxVertexBufferSize = requiredVbSize * 2;
            D3D11_BUFFER_DESC vbd;
            ZeroMemory(&vbd, sizeof(vbd));
            vbd.Usage = D3D11_USAGE_DYNAMIC;
            vbd.ByteWidth = static_cast<UINT>(mMaxVertexBufferSize);
            vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            mDevice->CreateBuffer(&vbd, nullptr, &mDynamicVertexBuffer);
        }

        if (mDynamicVertexBuffer) {
            D3D11_MAPPED_SUBRESOURCE mappedVb;
            HRESULT hr = mImmediateContext->Map(mDynamicVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedVb);
            if (SUCCEEDED(hr)) {
                memcpy(mappedVb.pData, mesh.vertices.data(), requiredVbSize);
                mImmediateContext->Unmap(mDynamicVertexBuffer, 0);
            }
            UINT stride = sizeof(sead::BfresVertex);
            UINT offset = 0;
            mImmediateContext->IASetVertexBuffers(0, 1, &mDynamicVertexBuffer, &stride, &offset);
        }

        // Upload or re-allocate index buffer
        if (!mesh.indices.empty()) {
            size_t requiredIbSize = mesh.indices.size() * sizeof(u32);
            if (!mDynamicIndexBuffer || mMaxIndexBufferSize < requiredIbSize) {
                if (mDynamicIndexBuffer) mDynamicIndexBuffer->Release();
                mMaxIndexBufferSize = requiredIbSize * 2;
                D3D11_BUFFER_DESC ibd;
                ZeroMemory(&ibd, sizeof(ibd));
                ibd.Usage = D3D11_USAGE_DYNAMIC;
                ibd.ByteWidth = static_cast<UINT>(mMaxIndexBufferSize);
                ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
                ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                mDevice->CreateBuffer(&ibd, nullptr, &mDynamicIndexBuffer);
            }

            if (mDynamicIndexBuffer) {
                D3D11_MAPPED_SUBRESOURCE mappedIb;
                HRESULT hr = mImmediateContext->Map(mDynamicIndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedIb);
                if (SUCCEEDED(hr)) {
                    memcpy(mappedIb.pData, mesh.indices.data(), requiredIbSize);
                    mImmediateContext->Unmap(mDynamicIndexBuffer, 0);
                }
                mImmediateContext->IASetIndexBuffer(mDynamicIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
                mImmediateContext->DrawIndexed(static_cast<UINT>(mesh.indices.size()), 0, 0);
            }
        } else {
            mImmediateContext->Draw(static_cast<UINT>(mesh.vertices.size()), 0);
        }
    }
}

void Dx11Renderer::submitModel(const sead::BfresModel& model, const sead::Matrix44f& world, u32 teamId) {
    if (!mInitialized || !mInFrame) return;

    for (const auto& mesh : model.meshes) {
        sead::Vector4f teamColor(1.0f, 0.5f, 0.0f, 1.0f);
        if (mesh.materialIndex < model.materials.size()) {
            teamColor = model.materials[mesh.materialIndex].teamColor;
        }
        submitMesh(mesh, world, teamColor, teamId);
    }
}

void Dx11Renderer::bindPaintTexture(const PaintMap3D& paintMap) {
    if (!mInitialized || !mInFrame) return;

    mStats.paintTextureUpdates++;

    if (!mHeadless && mDevice && mImmediateContext && paintMap.getWidth() > 0 && paintMap.getHeight() > 0) {
        u32 texW = paintMap.getWidth();
        u32 texH = paintMap.getHeight();

        // Create or update dynamic texture
        if (!mPaintTexture) {
            D3D11_TEXTURE2D_DESC td;
            ZeroMemory(&td, sizeof(td));
            td.Width = texW;
            td.Height = texH;
            td.MipLevels = 1;
            td.ArraySize = 1;
            td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            td.SampleDesc.Count = 1;
            td.SampleDesc.Quality = 0;
            td.Usage = D3D11_USAGE_DYNAMIC;
            td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            td.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

            HRESULT hr = mDevice->CreateTexture2D(&td, nullptr, &mPaintTexture);
            if (SUCCEEDED(hr)) {
                D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
                ZeroMemory(&srvd, sizeof(srvd));
                srvd.Format = td.Format;
                srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                srvd.Texture2D.MipLevels = 1;
                mDevice->CreateShaderResourceView(mPaintTexture, &srvd, &mPaintSrv);
            }
        }

        if (mPaintTexture && mPaintSrv) {
            D3D11_MAPPED_SUBRESOURCE mapped;
            HRESULT hr = mImmediateContext->Map(mPaintTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
            if (SUCCEEDED(hr)) {
                const u8* src = paintMap.getRgbaBuffer();
                u8* dst = static_cast<u8*>(mapped.pData);
                for (u32 row = 0; row < texH; ++row) {
                    memcpy(dst + row * mapped.RowPitch, src + row * texW * 4, texW * 4);
                }
                mImmediateContext->Unmap(mPaintTexture, 0);
            }
            mImmediateContext->PSSetShaderResources(1, 1, &mPaintSrv);
        }
    }
}

void Dx11Renderer::endFrame() {
    mInFrame = false;
}

void Dx11Renderer::present() {
    if (!mHeadless && mSwapChain) {
        mSwapChain->Present(1, 0); // VSync 60 FPS
    }
}

} // namespace Game
