#include "SceneInGame.h"

#include "Fwk/Framework.h"

#include "GameObjectMng/GameObjectMng.h"

#include "beats/Beats.h"

Beats pBeats;

//初期化
void SceneInGame::Init() {
	mNextScene = SceneType::None;
	GameObjectInit();
}

//終了
void SceneInGame::Term() {
	//ゲームオブジェクトの解放
	GameObjectTerm();

}

//更新
//音楽が再生されているか判定
//止まっていたら次のシーンへ遷移
void SceneInGame::Update() {
//ゲームオブジェクトの更新
	GameObjectUpdate();
	if (GetBeats()->IsPlaying())
	{
		mNextScene = SceneType::GameClear;
	}
	//ポーズ画面へ遷移
	if (Input_I->IsKeyDown('P')) {
		pBeats.StopBeats();
		mNextScene = SceneType::Pause;
	}
}

//描画
void SceneInGame::Render() {
	//ゲームオブジェクトの描画
	GameObjectRender();
}

float SceneInGame::GetTimer() {
	return mTimer;
}