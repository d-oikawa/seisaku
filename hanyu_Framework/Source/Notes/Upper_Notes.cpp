#include "Upper_Notes.h"

//初期化
void Upper_Notes::Init() {
	Notes::Init();
	{
		mTexture.Load("Images/rhythm/upper_notes.png");
		mSprite.SetTexture(mTexture);
		mSprite.SetSize(64.0f, 64.0f);
	}
	{
		mCollision.SetTag("Upper_Notes");
		mCollision.SetCircle(64.0f, 64.0f, 32.0f);
	}
}

//更新
void Upper_Notes::Update() {
	Notes::Update();
}