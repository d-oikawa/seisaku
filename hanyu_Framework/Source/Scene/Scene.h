#pragma once

//シーン種別の列挙型を使うのでinclude
#include "GameDef.h"

//シーンクラス
class Scene
{
public:
	//初期化
	virtual void Init();
	//後片付け
	virtual void Term();
	//更新
	virtual void Update();
	//描画
	virtual void Render();
	//次のシーンを取得する
	SceneType GetNextScene();
protected:
	//次のシーン
	SceneType mNextScene;

};