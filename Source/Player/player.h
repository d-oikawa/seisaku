#pragma once
#include "Fwk/Graphics.h"
#include "Lib/Math/Vector.h"
#include "Fwk/Collision/Collision.h"

class Player {
public:
	void Init();
	void Update();
	void Render();
	void Term();
private:
	Vector2f mPosition;
	Sprite mSprite;
	Texture mTexture;
};