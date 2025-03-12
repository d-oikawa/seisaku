#include "player.h"

//プレイヤーの初期化
//テクスチャ・サイズ・位置・サウンド等を設定する
void Player::Init() {

	mTexture.Load("Images/2dAction/player.png");
	mSprite.Init();
	mSprite.SetTexture(mTexture);
	mSprite.SetSize(64.0f, 64.0f);
	mSprite.SetPosition(mPosition);
}

//プレイヤ―の更新
//移動・衝突判定等の設定
void Player::Update() {

}

//プレイヤーの描画
void Player::Render() {

}

//プレイヤーの後片付け
void Player::Term() {
	mTexture.Unload();
	mSprite.Term();
}

//現在位置を取得する
Vector2f Player::GetPosition() {
	return mPosition;
}