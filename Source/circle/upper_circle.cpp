#include "upper_circle.h"
#include <Fwk/Framework.h>
#include <AppDef.h>



//初期化
//テクスチャ・位置・コリジョンの設定
void Upper_Circle::Init() {
	Decisioncircle::Init();
	{
		mTexture.Load("Images/rhythm/upper.png");
		mSprite.Init();
		mSprite.SetTexture(mTexture);
		mSprite.SetPosition(250.0f, -WINDOW_HEIGHT / 3.0f);
	}
	{
		mCollision.SetTag("Upper_circle");
		mCollision.SetCircle(0.0f, 0.0f, 32.0f);
	}
}

//更新
//キー入力中の時のみコリジョンをアクティブにし、サイズを少し大きくする
void Upper_Circle::Update() {
	if (Input_I->IsKeyDown('F'))
	{
		mCollision.SetActive(true);
		mSoundSource.Play();
	}
	if (Input_I->IsKeyPressed('F'))
	{
		mSprite.SetSize(76.0f, 76.0f);
	}
	else
	{
		mSprite.SetSize(64.0f, 64.0f);
		mCollision.SetActive(false);
	}
	Decisioncircle::Update();
}