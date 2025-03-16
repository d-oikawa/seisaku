#pragma once

//Sceneクラスを継承するのでinclude
#include "Scene.h"

//音源クラスの定義をinclude
#include "Fwk/Audio/SoundSource.h"
//MP3ファイルを扱うクラスのinclude
#include "Fwk/Audio/MP3Sound.h"

//SpriteやTexture,Animationを使うためのヘッダファイル
#include "Fwk/Graphics.h"

//Vector2fクラスを使うためのヘッダファイル
#include "Lib/Math/Vector.h"

//タイトルシーンクラス
class Pause : public Scene
{
public:
	//初期化
	void Init();
	//後片付け
	void Term();
	//更新
	void Update();
	//描画
	void Render();

private:
	//BGM読み込み用
	MP3Sound mSound;
	//音源
	SoundSource mSoundSource;

	Texture yariTexture;
	Texture conTexture;
	Sprite yariSprite;
	Sprite conSprite;
	Vector2f mPosition;

	float X;
	bool i = false;
};