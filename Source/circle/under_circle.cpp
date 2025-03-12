#include "under_circle.h"
#include <Fwk/Framework.h>
#include <AppDef.h>



//初期化
//テクスチャ・位置・コリジョンの設定
void Under_Circle::Init() {
	Decisioncircle::Init();
	{
		mTexture.Load("Images/rhythm/under.png");
		mSprite.Init();
		mSprite.SetTexture(mTexture);
		mSprite.SetPosition(250.0f, -WINDOW_HEIGHT / 1.5f);
	}
	{
		mCollision.SetTag("Under_circle");
		mCollision.SetCircle(64.0f, 64.0f, 32.0f);
	}
}

//更新
//キー入力中の時のみコリジョンをアクティブにし、サイズを少し大きくする
void Under_Circle::Update() {
	if (Input_I->IsKeyDown('J'))
	{
		mCollision.SetActive(true);
		mSoundSource.Play();
	}
	if (Input_I->IsKeyPressed('J'))
	{
		mSprite.SetSize(76.0f,76.0f);
	}
	else
	{
		mCollision.SetActive(false);
		mSprite.SetSize(64.0f, 64.0f);
	}
	Decisioncircle::Update();
}