#pragma once

//Sceneクラスを継承するのでinclude
#include "Scene.h"

//音源クラスの定義をinclude
#include "Fwk/Audio/SoundSource.h"
//MP3ファイルを扱うクラスのinclude
#include "Fwk/Audio/MP3Sound.h"

//タイトルシーンクラス
class SceneInGame : public Scene
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

	//時間を返す
	float GetTimer();

private:
	//BGM読み込み用
	MP3Sound mSound;
	//音源
	SoundSource mSoundSource;
	//タイマー
	float mTimer;
};