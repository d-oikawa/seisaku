#pragma once

#include <map>
#include <string>

#include "Lib/Graphics/Graphics.h"
#include "Lib/Graphics/ConstantBuffer.h"
#include "Lib/Math/Vector.h"

#include "Fwk/Shape/ShapePlane2D.h"
#include "Fwk/Animation/Animation.h"
#include "RenderObject.h"
#include "SpriteTypes.h"

//前方宣言
class Texture;

using namespace Lib::Graphics;
using namespace Lib::Math;

//PlayAnimationを呼んだ時に
//既に同じアニメーションを再生していたときの
//振る舞いを指定するための列挙型
enum class AnimationPlayStyle {
    Continue,
    Restart,

    Default = Restart
};

class Sprite : public Fwk::RenderObject
{
    Sprite(const Sprite&) = delete;
    void operator = (const Sprite&) = delete;

public:

    Sprite();
    ~Sprite();

    void Init();

    void SetTexture(Texture& pTexture);
    void Term();
    void Update();
    void Draw();

    //パラメータ設定系
    void SetRotation(float rotDeg);
    void SetRotation(const Vector2f& vDirection);
    void SetPosition(float x, float y);
    void SetPosition(const Vector2f& vPos);
    void SetSize(float width, float height);
    void SetScale(float sx, float sy);
    void SetScale(const Vector2f& vScale);
    void SetTexCoord(float left, float top, float width, float height);

    //ピボット中心軸の設定
    void SetPivot(Pivot pivot);
    void SetPivot(float x, float y);
    void SetPivot(const Vector2f& pivot);

    //スプライトセル構造体を使ってTexcoordとPivotを同時に設定
    void SetSpriteCell(const SpriteCell& cell);

    //パラメータ取得系
    float GetRotation()const;
    float GetPositionX()const;
    float GetPositionY()const;
    Vector2f GetPosition()const;
    float GetWidth()const;
    float GetHeight()const;
    float GetScaleX()const;
    float GetScaleY()const;
    float GetScaledWidth()const;
    float GetScaledHeight()const;

    //implements interface methods.
    void OnDraw(IGraphics* g,Camera* pCamera) override;
    //implements RenderObject
    RENDER_PASS GetRenderPass()const override;
    RENDER_MODE GetRenderMode()const override;

    //ブレンド設定
    void SetBlend(BLEND_MODE blendMode);
    void SetColor(float r, float g, float b, float a);
    void SetColor(float r, float g, float b);
    void SetAlpha(float a);
    void SetAdditionalColor(float r, float g, float b);

    //アニメーション関連(本来なら別クラスに分けたいところだが今はよしとする)
    //アニメーションを追加する(ID自動採番）
    int AddAnimation(const Animation& animation);
    //アニメーションを追加する(ID指定）
    int AddAnimation(int id,const Animation& animation);

    //アニメーションを削除する(ID指定）
    void RemoveAnimation(int id);
    //アニメーションを削除する(アニメーション名指定）
    void RemoveAnimation(const char* name);
    void RemoveAnimation(const string& name);

    //アニメーションを再生する(ID指定）
    void PlayAnimation(int id, AnimationPlayStyle playStyle = AnimationPlayStyle::Default);
    //アニメーションを再生する(アニメーション名指定）
    void PlayAnimation(const char* name, AnimationPlayStyle playStyle = AnimationPlayStyle::Default);
    void PlayAnimation(const string& name, AnimationPlayStyle playStyle = AnimationPlayStyle::Default);

    //アニメーションを一時停止する
    void PauseAnimation(bool bPause);

    //アニメーションを停止する
    void StopAnimation();
    
    //アニメーションのループ設定(現在再生中のアニメ）
    void SetLoopAnimation(bool bLoop);
    //アニメーションのループ設定(ID指定）
    void SetLoopAnimation(int id, bool bLoop);
    //アニメーションのループ設定(アニメーション名指定）
    void SetLoopAnimation(const char* name, bool bLoop);
    void SetLoopAnimation(const string& name, bool bLoop);
    //すべてのアニメーションのループ設定
    void SetLoopAnimationAll(bool bLoop);
    
    //アニメーションの再生速度設定(現在再生中のアニメ）
    void SetAnimationSpeed(float AnimSpeed);
    //アニメーションの再生速度設定(ID指定）
    void SetAnimationSpeed(int id, float AnimSpeed);
    //アニメーションの再生速度設定(アニメーション名指定）
    void SetAnimationSpeed(const char* name, float AnimSpeed);
    void SetAnimationSpeed(const string& name, float AnimSpeed);
    //すべてのアニメーションの再生速度設定
    void SetAnimationSpeedAll(float AnimSpeed);

    //現在再生中のアニメの再生スピード取得
    float GetAnimationSpeed()const;

    //現在再生中のアニメのIDを取得(未再生だった場合0が戻る）
    const int GetAnimationId()const;
    //現在再生中のアニメのアニメ名取得
    const char* GetAnimationName()const;

    //アニメーションを再生しているか
    bool IsAnimationPlaying()const;
    //アニメーションが一時停止しているか
    bool IsAnimationPausing()const;

    //X軸フリップ
    void SetFlipX(bool isFlip);
    bool IsFlipX()const;

    //Y軸フリップ
    void SetFlipY(bool isFlip);
    bool IsFlipY()const;

private:

    int _findFreeId()const;
    bool _findIdByName(const char* name,int* retVal)const;
    void _applyAnimation();

    Texture* m_pTexture;

    float m_RotDeg;
    
    DirectX::XMMATRIX m_RotMtx;
    DirectX::XMMATRIX m_TransMtx;
    DirectX::XMMATRIX m_SizeMtx;
    DirectX::XMMATRIX m_ScaleMtx;
    DirectX::XMMATRIX m_TransOffsetMtx;
    DirectX::XMMATRIX m_PivotMtx;    

    ConstantBuffer m_ConstantBuffer[Graphics::FrameCount];

    Fwk::ShapePlane2D m_plane;

    std::map<int, Animation> m_Animations;
    Animation* m_pPlayingAnimation;
    int m_PlayingAnimationId;

    RENDER_MODE m_RenderMode;

    bool m_isFlipX;
    bool m_isFlipY;

    bool m_IsInitialized;
};