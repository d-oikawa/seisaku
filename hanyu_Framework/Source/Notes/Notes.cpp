#include "Notes.h"
#include "GameDef.h"
#include "Fwk/Framework.h"
#include "Scene/SceneInGame.h"

SceneInGame mInGame;

//初期化
//共通するサイズ・コリジョン・サウンド・アニメーションの設定
void Notes::Init() {
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
		mSound.Load("Sound/jumpC.wav");
		mSoundSource.Init(mSound);
	}
	_initAnimation();
}

//更新
void Notes::Update() {
	mPosition.x -= 105.0f * Time_I->GetDeltaTime();
	mSprite.Update();
}

//描画
void Notes::Render() {
	mSprite.Draw();
}

//解放
//テクスチャ・コリジョン・音楽の解放
void Notes::Term() {
	mTexture.Unload();
	mSprite.Term();
	CollisionManager_I->Unregister(&mCollision);
	mSound.Unload();
	mSoundSource.Term();
}

//現在位置を取得
Vector2f Notes::GetPosition() {
	return mPosition;
}

//ノーツが回転するアニメーションを作成
void Notes::_initAnimation() {
	float rotDeg[] = {
		0.0f,
		360.0f,
	};
	Animation animtrun;
	CreateAnimationRotation(animtrun, "trun", 2, 0.5, true, rotDeg);
	mSprite.AddAnimation(animtrun);
	mSprite.PlayAnimation("trun");
}

//アクティブか否かを返す
bool Notes::IsActive() {
	return mIsActive;
}

//アクティブ状態の設定
void Notes::SetActive(bool isActive) {
	mIsActive = isActive;
	mCollision.SetActive(isActive);
}

//タイマーを設定
void Notes::SetTimer(float timer) {
	mTimer = mInGame.GetTimer();
}

//タイマーを取得
float Notes::GetTimer() {
	return mTimer;
}

void Notes::OnCreated() {
	//なし
}