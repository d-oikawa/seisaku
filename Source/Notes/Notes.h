#pragma once

//------------------------
//ノーツの基礎クラス
//------------------------
#include "Fwk/Graphics.h"
#include "Lib/Math/Vector.h"
#include "Fwk/Collision/Collision.h"
#include "Fwk/Audio/WaveSound.h"
#include "Fwk/Audio/SoundSource.h"
using namespace Lib::Math;
using namespace Fwk::Collision;


class Notes
{
public:
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Term();
	Vector2f GetPosition();
	bool IsActive();
	void SetActive(bool IsActive);
	float GetTimer();
	void SetTimer(float timer);
	virtual void OnCreated();


protected:
	void _initAnimation();

protected:
	float mTimer;
	bool mIsActive;
	Texture mTexture;
	Sprite mSprite;
	Vector2f mPosition;
	Collision mCollision;
	WaveSound mSound;
	SoundSource mSoundSource;

};