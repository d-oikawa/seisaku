#pragma once

#include "Lib/Math/Vector.h"

using namespace Lib::Math;

namespace Fwk {
namespace Collision {

enum class ShapeType
{
	None = -1,
	Circle,
	Rect,
};

struct Circle
{
	Lib::Math::Vector2f position;
	float radius;
};

struct Rect
{
	Lib::Math::Vector2f position;
	float width;
	float height;
};

struct Shape {
	ShapeType shapeType;
	Circle circle;
	Rect rect;
};

//---------------------
//矩形と矩形の衝突を判定する
//---------------------
bool IsRectsOverlapped(Rect rectA, Rect rectB);

//---------------------
//円とと矩形の衝突を判定する
//---------------------
bool IsCircleAndRectOverlapped(Circle circle, Rect rect);

//---------------------
//円と円の衝突を判定する
//---------------------
bool IsCirclesOverlapped(Circle c1, Circle c2);

}
}