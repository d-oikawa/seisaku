#include "SceneTitle.h"

#include "Fwk/Framework.h"

//初期化
void SceneTitle::Init() {
	//次のシーンの初期値をなしにしておく
	mNextScene = SceneType::None;
}

//終了
void SceneTitle::Term() {
	//なし
}

//更新
void SceneTitle::Update() {
	//Aボタンを押したら次のシーンにInGameを設定する
	if (Input_I->IsKeyDown('Z')||Input_I->IsButtonDown(0,GAMEPAD_BUTTON::A))
	{
		mNextScene = SceneType::InGame;
	}
}

//描画
void SceneTitle::Render() {
	//文字列を描画する
	PrintText("aaaaa", 380.0f, 250.0f);
}
