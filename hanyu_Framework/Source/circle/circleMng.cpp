//circleの管理クラス
#include "circleMng.h"

Under_Circle mUnderCircle;
Upper_Circle mUpperCircle;

//初期化
void CircleMng::Init()
{
	mUnderCircle.Init();
	mUpperCircle.Init();
};

//更新
void CircleMng::Update()
{
	mUnderCircle.Update();
	mUpperCircle.Update();
};

//描画
void CircleMng::Render()
{
	mUnderCircle.Render();
	mUpperCircle.Render();
};

//解放
void CircleMng::Term()
{
	mUnderCircle.Term();
	mUpperCircle.Term();
};
