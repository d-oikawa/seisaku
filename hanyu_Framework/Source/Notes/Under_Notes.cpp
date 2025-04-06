#include "Under_Notes.h"
#include <AppDef.h>

//èâä˙âª
void Under_Notes::Init() {
	Notes::Init();
	{
		mTexture.Load("Images/rhythm/under_notes.png");
		mSprite.SetTexture(mTexture);
		mSprite.SetSize(64.0f, 64.0f);
		mPosition.y = -WINDOW_HEIGHT / 1.5f;
		mSprite.SetPosition(mPosition);
	}
	{
		mCollision.SetTag("Under_Notes");
		mCollision.SetCircle(64.0f, 64.0f, 32.0f);
	}
}

//çXêV
void Under_Notes::Update() {
	Notes::Update();

}