#include "Pause.h"

#include "SceneInGame.h"

#include "beats/Beats.h"

#include "Fwk/Framework.h"

#include "Mst/PauseMst.h"

#include <iostream>
#include <iomanip>


Beats pBeats2;

//初期化
void Pause::Init() {
	//次のシーンの初期値をなしにしておく
	mNextScene = SceneType::None;
}

//終了
void Pause::Term() {
	//なし
}

//更新
void Pause::Update() {
	mSoundSource.Stop();
	//PrintText("コンティニュー", 420.0f, 50.0f);
	//PrintText("最初からやり直す", 400.0f, 250.0f);


	//下ボタンを押したら
	if (Input_I->IsKeyDown(VK_DOWN)) {
		//次を選択する
		mSelectedIndex += 1;
		//インデックスがオーバーしたら0に戻す
		if (mSelectedIndex >= 3) {
			mSelectedIndex = 0;
		}
	}

	//上ボタンを押したら
	if (Input_I->IsKeyDown(VK_UP)) {
		//ひとつ前を選択する
		mSelectedIndex -= 1;
		//インデックスが0未満になったら一番下を選択する
		if (mSelectedIndex < 0) {
			mSelectedIndex = 3 - 1;
		}
	}

	//Zボタンを押したら
	if (Input_I->IsKeyDown('Z')) {
		if (mSelectedIndex == 0) {

		}
		else if (mSelectedIndex == 1) {
			mNextScene = SceneType::InGame;
		}
		else if (mSelectedIndex == 2) {
			mNextScene = SceneType::Title;
		}
	}
}

//描画
void Pause::Render() {
	PrintText("[ポーズ画面]", 400.0f, 50.0f);


	//選択中の文字色
	float color_selected[3] = { 1.0,1.0f,0.0f };
	//通常の文字色
	float color_normal[3] = { 0.85f,0.0f,0.85f };

	//文字列を描画する
	for (int i = 0; i < 3; ++i) {

		PauseData* pData = GetPauseDataMst().Get(i);

		//文字色選択
		float* pColor = (mSelectedIndex == i) ? color_selected : color_normal;

		//選択名の描画  ※カラー設定の引数を追加するとエラーが起きる。羽生先生に相談するしかない
		PrintText("ありがとね", 400.0f, 120.0f + i * 40.0f);

	}
}