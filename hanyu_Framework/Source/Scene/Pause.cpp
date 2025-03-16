#include "Pause.h"

#include "SceneInGame.h"

#include "beats/Beats.h"

#include "Fwk/Framework.h"

#include <iostream>
#include <iomanip>


Beats pBeats2;

//初期化
void Pause::Init() {
	//次のシーンの初期値をなしにしておく
	mNextScene = SceneType::None;
	//テクスチャの読み込み
	conTexture.Load("Images/2dAction/con.png");
	yariTexture.Load("Images/2dAction/yari.png");

	//スプライトの初期化
	conSprite.Init();
	yariSprite.Init();
	//テクスチャの設定
	conSprite.SetTexture(conTexture);
	yariSprite.SetTexture(yariTexture);
	//スプライトのサイズ設定
	conSprite.SetSize(240.0f, 200.0f);
	yariSprite.SetSize(240.0f, 200.0f);
	//YajirushiSprite.SetPivot(Pivot::TopLeft);
	conSprite.SetPosition(480.0f, -250.0f);
	yariSprite.SetPosition(480.0f, -450.0f);
}

//終了
void Pause::Term() {
	//なし
	conSprite.Term();
	yariSprite.Term();
	conTexture.Unload();
	yariTexture.Unload();
}

//更新
void Pause::Update() {
	mSoundSource.Stop();
	//PrintText("コンティニュー", 420.0f, 50.0f);
	//PrintText("最初からやり直す", 400.0f, 250.0f);
		if (i == false) {
			conSprite.SetAdditionalColor(10.0f, 0.0f, 0.0f);
			yariSprite.SetAdditionalColor(0.0f, 0.0f, 0.0f);
			i = true;
		}
		if (Input_I->IsKeyDown('2')) {
			yariSprite.SetAdditionalColor(1.0f, 0.0f, 0.0f);
			conSprite.SetAdditionalColor(0.0f, 0.0f, 0.0f);
		}
		if (Input_I->IsKeyDown('8')) {
			conSprite.SetAdditionalColor(1.0f, 7.0f, 0.0f);
			yariSprite.SetAdditionalColor(0.0f, 0.0f, 0.0f);
		}
		else  {
			if (Input_I->IsKeyDown('5')) {
				mNextScene = SceneType::Title;
			}
		}
}

//描画
void Pause::Render() {
	conSprite.Draw();
	yariSprite.Draw();
}