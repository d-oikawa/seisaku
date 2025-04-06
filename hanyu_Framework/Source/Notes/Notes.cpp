#include "Notes.h"
#include "GameDef.h"
#include "Fwk/Framework.h"
#include "Scene/SceneInGame.h"
#include "GameObjectMng/GameObjectMng.h"
#include "AppDef.h"

SceneInGame mInGame;

//初期化
//共通するサイズ・テクスチャ・コリジョン・サウンド・アニメーションの設定
void Notes::Init() {
	{
		mSprite.SetTexture(mTexture);
		mSprite.SetSize(64.0f, 64.0f);
		mPosition.x = WINDOW_WIDTH + 64.0f;
		mSprite.SetVisible(true);
		mIsActive = true;
	}
	{
		mCollision.SetGroup((int)CollisionGroup::decisioncircle);
		mCollision.AddHitGroup((int)CollisionGroup::notes);
		mCollision.SetOwner(this);
		mCollision.SetActive(true);
		//CollisionManager_I->Register(&mCollision);
	}
	{
		mSound.Load("Sound/jumpC.wav");
		mSoundSource.Init(mSound);
	}
	_initAnimation();

	mSprite.Init();
}

//更新
//生成を受け取ったらノーツを常に移動させる
//左端に移動したら見えなくする
void Notes::Update() {
	if (!mIsActive)
	{
		return;
	}
	mSprite.SetVisible(true);
	float speed = 600.0f * Time_I->GetDeltaTime();
	mPosition.x -= speed;
	if (mPosition.x < 0.0f)
	{
		mIsActive = false;
		mSprite.SetVisible(false);
	}

	mSprite.SetPosition(mPosition);
	mSprite.Update();
}

//描画
void Notes::Render() {
	if (!mIsActive)
	{
		return;
	}
	mSprite.Draw();
}

//解放
//テクスチャ・コリジョン・音楽の解放
void Notes::Term() {
	mTexture.Unload();
	mSprite.Term();
	//CollisionManager_I->Unregister(&mCollision);
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

//拍数を設定
void Notes::SetBeat(int beat) {
	mBeat = mInGame.GetBeat();
}

//拍数を取得
float Notes::GetBeat() {
	return mBeat;
}

//生成されたときupdateを更新する
void Notes::OnCreated() {
}