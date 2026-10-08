#pragma once

#include "types.h"
#include "sead/math/seadVector.h"
#include "sead/math/seadMatrix.h"
#include "sead/resource/BfresParser.h"
#include "Game/Paint/PaintMap3D.h"
#include <vector>
#include <string>

// Forward declarations for DirectX 11 COM interfaces
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11DepthStencilState;
struct ID3D11RasterizerState;
struct ID3D11Buffer;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11SamplerState;

namespace Game {

struct Dx11Viewport {
    f32 x;
    f32 y;
    f32 width;
    f32 height;
    f32 minDepth;
    f32 maxDepth;

    Dx11Viewport()
        : x(0.0f), y(0.0f), width(1280.0f), height(720.0f), minDepth(0.0f), maxDepth(1.0f)
    {}
};

struct alignas(16) Dx11FrameConstants {
    sead::Matrix44f viewProjMatrix;
    sead::Vector4f cameraPos;
    sead::Vector4f lightDir;
    f32 gameTime;
    f32 padding[3];

    Dx11FrameConstants()
        : cameraPos(0, 5, -10, 1)
        , lightDir(0.577f, 0.577f, -0.577f, 0)
        , gameTime(0.0f)
    {
        padding[0] = padding[1] = padding[2] = 0.0f;
    }
};

struct alignas(16) Dx11ObjectConstants {
    sead::Matrix44f worldMatrix;
    sead::Vector4f teamColor;
    u32 teamId;
    u32 flags;
    f32 padding[2];

    Dx11ObjectConstants()
        : teamColor(1.0f, 0.5f, 0.0f, 1.0f)
        , teamId(0)
        , flags(0)
    {
        padding[0] = padding[1] = 0.0f;
    }
};

struct Dx11DrawStats {
    u32 drawCalls;
    u32 verticesDrawn;
    u32 indicesDrawn;
    u32 paintTextureUpdates;

    void reset() {
        drawCalls = 0;
        verticesDrawn = 0;
        indicesDrawn = 0;
        paintTextureUpdates = 0;
    }
};

class Dx11Renderer {
public:
    Dx11Renderer();
    ~Dx11Renderer();

    // Initializes DirectX 11 pipeline (supports native Win32 HWND or headless test mode)
    bool initPipeline(void* hWnd, u32 width, u32 height, bool headless = false);
    void shutdown();

    // Frame lifecycle
    void beginFrame(f32 r = 0.08f, f32 g = 0.12f, f32 b = 0.18f, f32 a = 1.0f);
    void setFrameConstants(const sead::Matrix44f& viewProj, const sead::Vector3f& camPos, f32 time);
    void submitMesh(const sead::BfresMesh& mesh, const sead::Matrix44f& world, const sead::Vector4f& teamColor, u32 teamId);
    void submitModel(const sead::BfresModel& model, const sead::Matrix44f& world, u32 teamId);
    void bindPaintTexture(const PaintMap3D& paintMap);
    void endFrame();
    void present();

    // Pipeline query methods
    bool isInitialized() const { return mInitialized; }
    bool isHeadless() const { return mHeadless; }
    u32 getWidth() const { return mWidth; }
    u32 getHeight() const { return mHeight; }
    const Dx11Viewport& getViewport() const { return mViewport; }
    const Dx11DrawStats& getStats() const { return mStats; }
    const Dx11FrameConstants& getFrameConstants() const { return mFrameConstants; }

private:
    bool initD3D11();
    bool initShaders();
    bool initDynamicBuffers();
    void releaseD3D11();

    bool mInitialized;
    bool mHeadless;
    void* mHwnd;
    u32 mWidth;
    u32 mHeight;
    Dx11Viewport mViewport;
    Dx11DrawStats mStats;
    Dx11FrameConstants mFrameConstants;
    Dx11ObjectConstants mObjectConstants;
    bool mInFrame;

    // Backbuffer clear color
    f32 mClearColor[4];

    // Direct3D 11 hardware pipeline objects
    ID3D11Device*           mDevice;
    ID3D11DeviceContext*    mImmediateContext;
    IDXGISwapChain*         mSwapChain;
    ID3D11RenderTargetView* mRenderTargetView;
    ID3D11DepthStencilView* mDepthStencilView;
    ID3D11DepthStencilState* mDepthStencilState;
    ID3D11RasterizerState*  mRasterizerState;

    ID3D11VertexShader*     mVertexShader;
    ID3D11PixelShader*      mPixelShader;
    ID3D11InputLayout*      mInputLayout;

    ID3D11Buffer*           mCbFrame;
    ID3D11Buffer*           mCbObject;
    ID3D11Buffer*           mDynamicVertexBuffer;
    ID3D11Buffer*           mDynamicIndexBuffer;
    size_t                  mMaxVertexBufferSize;
    size_t                  mMaxIndexBufferSize;

    ID3D11Texture2D*          mPaintTexture;
    ID3D11ShaderResourceView* mPaintSrv;
    ID3D11SamplerState*       mSamplerState;
};

} // namespace Game
