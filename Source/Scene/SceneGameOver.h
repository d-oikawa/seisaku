#pragma once

//Sceneクラスを継承するのでinclude
#include "Scene.h"

//タイトルシーンクラス
class SceneGameOver : public Scene
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
};