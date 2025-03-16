#pragma once

#include "circle/under_circle.h"
#include "circle/upper_circle.h"

//音楽を再生させる
#include "beats/Beats.h"

//ノーツの情報を取得する
#include "Notes/NotesMng.h"

//全オブジェクト初期化
void GameObjectInit();

//全オブジェクト更新
void GameObjectUpdate();

//全オブジェクト描画
void GameObjectRender();

//全オブジェクト解放
void GameObjectTerm();


//Beatsクラスのポインタを返す
Beats* GetBeats();