#include "Scene.h"

#include "Fwk/Framework.h"

//初期化
void Scene::Init() {
	//次のシーンの初期値をなしにしておく
	mNextScene = SceneType::None;
}

//終了
void Scene::Term() {
	//なし
}

//更新
void Scene::Update() {
	//なし
}

//描画
void Scene::Render() {
	//なし
}

//次のシーンを取得する
SceneType Scene::GetNextScene() {
	return mNextScene;
}