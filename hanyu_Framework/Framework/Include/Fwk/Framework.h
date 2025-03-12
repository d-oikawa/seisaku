#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <DirectXMath.h>

#include <cstdint>

#include "Lib/System/Window.h"
#include "Lib/System/Input.h"
#include "Lib/Audio/Audio.h"
#include "Lib/Math/Vector.h"

#include "Fwk/Asset/AssetManager.h"
#include "Fwk/Graphics/RenderManager.h"
#include "Fwk/Time/Time.h"
#include "Fwk/Time/FPSMeasure.h"
#include "Fwk/Collision/CollisionManager.h"

#if defined(DEBUG) || defined(_DEBUG)
#include "Fwk/Debug/DebugLog.h"
#endif

#pragma comment( lib, "d3d12.lib" )
#pragma comment( lib, "dxgi.lib" )

using namespace Lib::Graphics;
using namespace Lib::System;
using namespace Lib::Math;

//Fwk::Collisionはusing namespaceしない。
//実習でCollision構造体を作らせる可能性があり、名前を衝突させたくないので。

#define Framework_I Fwk::Framework::GetInstance()
#define AssetManager_I  Framework_I->GetAssetManager()
#define RenderManager_I  Framework_I->GetRenderManager()
#define CollisionManager_I  Framework_I->GetCollisionManager()
#define Input_I  Framework_I->GetInput()
#define Time_I  Framework_I->GetTime()
#define Window_I  Framework_I->GetWindow()
#define Audio_I  Framework_I->GetAudio()

#define PrintText(_t,...)   Fwk::Framework::GetInstance()->_PrintText(_t,__VA_ARGS__)

#if defined(DEBUG) || defined(_DEBUG)
#define DebugLog(_t,...)    Fwk::Framework::GetInstance()->DebugLogImpl(_t,__VA_ARGS__)
#else
#define DebugLog(_t,...)
#endif

namespace Fwk{

struct FrameworkDesc
{
    uint32_t width;
    uint32_t height;
    const  wchar_t* pClassName;
    const  wchar_t* pWndTitle;
    WNDPROC WndProc;

    const  wchar_t* AssetRootDir;
    uint32_t AssetBufferSize;

    uint32_t RenderRequestBufferSize;

    uint32_t DescriptorSize_Resouce;
    uint32_t DescriptorSize_Sampler;

    const wchar_t* SpriteFontFile;
    const wchar_t* VtxShaderFile;
    const wchar_t* PixShaderFile;

    uint32_t TargetFPS;

    uint32_t CollisionBufferSize;

    DWORD DeltaTimeMsMax;
};

class Framework
{
    Framework(const Framework&) = delete;
    void operator = (const Framework&) = delete;

    Framework();
    ~Framework();

    static Framework* g_Instance;

public:

    static void GenerateInstance();
    static Framework* GetInstance();
    static void ReleaseInstance();

    bool Init(const FrameworkDesc& desc);
    void Term();

    void Update();
    void Render();
    void FrameSync();

    void ShowFPS();
    void HideFPS();
    void SetFPSPosition(float x, float y);

    //ウィンドウの中心点を原点とした時のカーソル位置を戻す
    //ゲーム内座標ではなくウィンドウの内の位置なので注意
    Vector2f GetMousePointInClient()const;

    AssetManager* GetAssetManager();
    RenderManager* GetRenderManager();
    Fwk::Collision::CollisionManager* GetCollisionManager();
    IInput* GetInput();
    Time* GetTime();
    Window* GetWindow();
    Lib::Audio::Audio* GetAudio();

    void _PrintText(const wchar_t* pText, float pos_x, float pos_y);
    void _PrintText(const wchar_t* pText, float pos_x, float pos_y,const float rgb[]);
    void _PrintText(const wchar_t* pText, float pos_x, float pos_y, const float rgb[], float scale);
    void _PrintText(const char* pText, float pos_x, float pos_y);
    void _PrintText(const char* pText, float pos_x, float pos_y, const float rgb[]);
    void _PrintText(const char* pText, float pos_x, float pos_y, const float rgb[], float scale);
    void _PrintText(const string& pText, float pos_x, float pos_y);
    void _PrintText(const string& pText, float pos_x, float pos_y, const float rgb[]);
    void _PrintText(const string& pText, float pos_x, float pos_y, const float rgb[], float scale);

    void DebugLogImpl(const char* format,...);
    void DebugLogImpl(const string& format,... );

private:

    Window m_Window;
    Input m_Input;
    Time m_Time;
    FPSMeasure m_FPS;
    Lib::Audio::Audio m_Audio;

    Fwk::AssetManager m_AssetManager;
    Fwk::RenderManager m_RenderManager;
    Fwk::Collision::CollisionManager m_CollisionManager;

    enum {
        TEXT_BUFFER_MAX = 128
    };
    Font m_Text[TEXT_BUFFER_MAX];
    int m_UseTextNum;

#if defined(DEBUG) || defined(_DEBUG)
    Fwk::DebugLog mDebugLog;
#endif

};

}//namespace Fwk