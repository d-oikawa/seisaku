#pragma once

//------------------------
//ノーツの基礎クラス
//------------------------
#include "Fwk/Graphics.h"
#include "Lib/Math/Vector.h"
#include "Fwk/Collision/Collider.h"
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
	float GetBeat();
	void SetBeat(int beat);
	virtual void OnCreated();
	int GetNotesLine();
	void SetNotesLine(int notesline);


protected:
	void _initAnimation();

protected:
	float mBeat;
	bool mIsActive;
	bool mIsOnCreated;
	int mNotesLine;
	Texture mTexture;
	Sprite mSprite;
	Vector2f mPosition;
	Collider mCollision;
	WaveSound mSound;
	SoundSource mSoundSource;

};