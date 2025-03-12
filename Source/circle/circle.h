#pragma once

//------------------------
//”»’ècircle‚ÌŠî‘bƒNƒ‰ƒX
//------------------------
#include "Fwk/Graphics.h"
#include "Lib/Math/Vector.h"
#include "Fwk/Collision/Collision.h"
#include "Fwk/Audio/WaveSound.h"
#include "Fwk/Audio/SoundSource.h"

using namespace Lib::Math;
using namespace Fwk::Collision;


class Decisioncircle
{
public:
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Term();
	Vector2f GetPosition();

protected:
	void _initAnimation();

protected:
	Texture mTexture;
	Sprite mSprite;
	Vector2f mPosition;
	Collision mCollision;
	WaveSound mSound;
	SoundSource mSoundSource;

};
