#include "Fwk/Collision/CollisionShape.h"

#include <math.h>

namespace Fwk {
namespace Collision {

//---------------------
//矩形と矩形の衝突を判定する
//---------------------
bool IsRectsOverlapped(Rect rectA, Rect rectB)
{
	//矩形Aのパラメータ
	float x1 = rectA.position.x;
	float y1 = rectA.position.y;
	float w1 = rectA.width;
	float h1 = rectA.height;

	//矩形Bのパラメータ
	float x2 = rectB.position.x;
	float y2 = rectB.position.y;
	float w2 = rectB.width;
	float h2 = rectB.height;

	//二つの矩形の中心点の、X軸上の距離dxとY軸上の距離dyを算出
	//距離dxはx2-x1の絶対値、距離dyはy2-y1の絶対値
	float dx = (float)fabs(x2 - x1);
	float dy = (float)fabs(y2 - y1);

	//各軸の距離と矩形の幅・高さから衝突を調べる

	//x軸上の距離(dx)が、(矩形Aの衝突矩形の幅の半分＋矩形Bの衝突矩形の幅の半分) 
	//以下で、かつ
	//y軸上の距離(dy)が、(矩形Aの衝突矩形の高さの半分＋矩形Bの衝突矩形の高さの半分)
	//であれば衝突している
	return (dx <= (w1 / 2) + (w2 / 2) && dy <= (h1 / 2) + (h2 / 2));
}

//---------------------
//円とと矩形の衝突を判定する
//---------------------
bool IsCircleAndRectOverlapped(Circle circle, Rect rect)
{
	//円の中心点から最も近い矩形内の点の座標を調べる

	//矩形の右端のX座標
	float right = rect.position.x + (rect.width / 2.0f);
	//矩形の左端のX座標
	float left = rect.position.x - (rect.width / 2.0f);
	//矩形の上端のY座標
	float top = rect.position.y + (rect.height / 2.0f);
	//矩形の下端のY座標
	float bottom = rect.position.y - (rect.height / 2.0f);

	//一旦、円の中心点の座標を最短点としておく
	float x = circle.position.x;
	float y = circle.position.y;

	//X,Yそれぞれについて、矩形の範囲外となっていた場合、範囲内(端の座標)に収める
	if (x < left) {
		x = left;
	}
	else if (x > right) {
		x = right;
	}

	if (y < bottom) {
		y = bottom;
	}
	else if (y > top) {
		y = top;
	}

	//座標x,yは円の中心点に最も近い矩形内の点となっているので、
	//この点と円の中心点との距離が円の半径以内に収まっていたら
	//円と矩形が衝突していることになる

	//二点間の距離を算出
	float d = sqrtf(
		(circle.position.x - x) * (circle.position.x - x)
		+ (circle.position.y - y) * (circle.position.y - y)
	);

	//二点間の距離が円の半径よりも短ければ衝突している
	return (d <= (circle.radius));
}

//---------------------
//円と円の衝突を判定する
//---------------------
bool IsCirclesOverlapped(Circle c1, Circle c2)
{
	//二点間の距離を算出
	float d = sqrtf(
		(c2.position.x - c1.position.x) * (c2.position.x - c1.position.x)
		+ (c2.position.y - c1.position.y) * (c2.position.y - c1.position.y)
	);

	//二点間の距離が半径１＋半径２よりも短ければ衝突している
	return (d <= (c1.radius + c2.radius));
}

}
}