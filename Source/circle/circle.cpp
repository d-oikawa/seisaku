#include "circle.h"
#include "GameDef.h"
#include "Fwk/Framework.h"

//初期化
//共通するサイズ・コリジョン・サウンド・アニメーションの設定
void Decisioncircle::Init() {
	{
		mSprite.SetSize(64.0f, 64.0f);
	}
	{
		mCollision.SetGroup((int)CollisionGroup::decisioncircle);
		mCollision.AddHitGroup((int)CollisionGroup::notes);
		mCollision.SetOwner(this);
		mCollision.SetActive(true);
		CollisionManager_I->Register(&mCollision);
	}
	{
		mSound.Load("Sound/hit.wav");
		mSoundSource.Init(mSound);
	}
	_initAnimation();
}

//更新
void Decisioncircle::Update() {
	mSprite.Update();
}

//描画
void Decisioncircle::Render() {
	mSprite.Draw();
}

//解放
//テクスチャ・コリジョン・音楽の解放
void Decisioncircle::Term() {
	mTexture.Unload();
	mSprite.Term();
	CollisionManager_I->Unregister(&mCollision);
	mSound.Unload();
	mSoundSource.Term();
}

//現在位置を取得
Vector2f Decisioncircle::GetPosition() {
	return mPosition;
}

//判定サークルが回転するアニメーションを作成
void Decisioncircle::_initAnimation() {
	float rotDeg[] = {
		0.0f,
		360.0f,
	};
	Animation animtrun;
	CreateAnimationRotation(animtrun, "trun", 2, 0.5, true, rotDeg);
	mSprite.AddAnimation(animtrun);
	mSprite.PlayAnimation("trun");
}