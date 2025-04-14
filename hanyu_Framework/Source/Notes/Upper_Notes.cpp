#include "Upper_Notes.h"

#include <AppDef.h>
//初期化
void Upper_Notes::Init() {
	Notes::Init();
	{
		mTexture.Load("Images/rhythm/upper_notes.png");
		mSprite.SetTexture(mTexture);
		mSprite.SetSize(64.0f, 64.0f);
		mPosition.y=(-WINDOW_HEIGHT / 3.0f);
		mSprite.SetPosition(mPosition);
	}
	{
		mCollision.SetTag("Upper_Notes");
		mCollision.SetCircle(64.0f, 64.0f, 32.0f);
	}
}

//更新
//生成のラインが同じでは無ければ更新しない
void Upper_Notes::Update() {
	if (Notes::GetNotesLine())
	{
		return;
	}
	Notes::Update();
}