#pragma once

#include <math.h>
#include <float.h>
#include <directxmath.h>

using namespace DirectX;

namespace Lib{
namespace Math {

template <class _T>
class TVector3
{
public:
	_T x;
	_T y;
	_T z;

public:

	TVector3() :x((_T)0), y((_T)0), z((_T)0) {}
	TVector3(_T _x, _T _y, _T _z) :x(_x), y(_y), z(_z) {}

	TVector3<_T>& operator=(const TVector3<_T>& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	TVector3<_T>& operator+=(const TVector3<_T>& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	TVector3<_T>& operator-=(const TVector3<_T>& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	TVector3<_T>& operator*=(_T s) {
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	TVector3<_T>& operator/=(_T s) {
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}

	_T GetLength()const {
		FXMVECTOR v = { (float)x,(float)y,(float)z };
		float len = 0.0f;
		XMStoreFloat(&len, DirectX::XMVector3Length(v));
		return (_T)len;
	}

	//自分を正規化します
	void Normalize() {
		FXMVECTOR v = { (float)x,(float)y,(float)z };
		DirectX::XMFLOAT3 rv{};
		XMStoreFloat3(&rv, DirectX::XMVector3Normalize(v));
		x = (_T)rv.x;
		y = (_T)rv.y;
		z = (_T)rv.z;
	}

	//単位ベクトル（大きさ1のベクトル）を返します。
	TVector3<_T> GetUnitVector()const
	{
		FXMVECTOR v = { (float)x,(float)y,(float)z };
		DirectX::XMFLOAT3 rv{};
		XMStoreFloat3(&rv, DirectX::XMVector3Normalize(v));
		TVector3<_T> result((_T)rv.x, (_T)rv.y, (_T)rv.z);
		return result;
	}

	static _T Dot(const TVector3<_T>& v0, const TVector3<_T>& v1) {
		return (v0.x * v1.x) + (v0.y * v1.y) + (v0.z * v1.z);
	}
	static TVector3<_T> Cross(const TVector3<_T>& v0, const TVector3<_T>& v1) {
		TVector3<_T> result(
			(v0.y * v1.z) - (v0.z * v1.y),
			(v0.z * v1.x) - (v0.x * v1.z),
			(v0.x * v1.y) - (v0.y * v1.x)
		);
		return result;
	}
};

//  +演算子のオーバーロード
template <typename _T>
TVector3<_T> operator+(const TVector3<_T>& v0, const TVector3<_T>& v1) {
	TVector3<_T> vr(v0.x + v1.x, v0.y + v1.y, v0.z + v1.z);
	return vr;
}

//  -演算子のオーバーロード
template <typename _T>
TVector3<_T> operator-(const TVector3<_T>& v0, const TVector3<_T>& v1) {
	TVector3<_T> vr(v0.x - v1.x, v0.y - v1.y, v0.z - v1.z);
	return vr;
}

//  *演算子のオーバーロード
template <typename _T>
TVector3<_T> operator*(const _T s, const TVector3<_T>& v) {
	TVector3<_T> vr(v.x * s, v.y * s, v.z * s);
	return vr;
}

//  *演算子のオーバーロード
template <typename _T>
TVector3<_T> operator*(const TVector3<_T>& v, const _T s) {
	TVector3<_T> vr(v.x * s, v.y * s, v.z * s);
	return vr;
}

//  /演算子のオーバーロード
template <typename _T>
TVector3<_T> operator/(const TVector3<_T>& v, const _T s) {
	TVector3<_T> vr(v.x / s, v.y / s, v.z / s);
	return vr;
}

//  ==演算子のオーバーロード
template <typename _T>
bool operator==(const TVector3<_T>& v0, const TVector3<_T>& v1) {
	return (v0 == v1);
}

//floatで特殊化
template <>
bool operator==<float>(const TVector3<float>& v0, const TVector3<float>& v1);

//doubleで特殊化
template <>
bool operator==<double>(const TVector3<double>& v0, const TVector3<double>& v1);

//  !=演算子のオーバーロード
template <typename _T>
bool operator!=(const TVector3<_T>& v0, const TVector3<_T>& v1) {
	return !(v0 == v1);
}

//汎用ベクトル3次元ベクトル型定義
typedef TVector3<float> Vector3f;
typedef TVector3<int> Vector3i;
typedef TVector3<double> Vector3d;

}//namespace Math
}//namespace Lib