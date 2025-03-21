#include "Under_Notes.h"

//初期化
void Under_Notes::Init() {
	Notes::Init();
	{
		mTexture.Load("Images/rhythm/under_notes.png");
		mSprite.SetTexture(mTexture);
		mSprite.SetSize(64.0f, 64.0f);
	}
	{
		mCollision.SetTag("Under_Notes");
		mCollision.SetCircle(64.0f, 64.0f, 32.0f);
	}
}

//更新
void Under_Notes::Update() {
	Notes::Update();
}