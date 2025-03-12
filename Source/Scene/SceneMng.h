#pragma once

//Sceneクラスを使うのでinclude
#include "Scene.h"

//シーン管理クラス
class SceneMng
{
public:
	//初期化
	void Init(SceneType startScene);
	//後片付け
	void Term();
	//更新
	void Update();
	//描画
	void Render();

private:
	//新しいシーンを開始する
	void _beginScene(SceneType newScene);
	//現在のシーンを終了する
	void _endCurrentScene();

private:
	//現在のシーン
	SceneType mCurrentScene;
	//次のシーン
	SceneType mNextScene;
	//各シーンクラスのポインタ
	Scene* mpScene;
};
