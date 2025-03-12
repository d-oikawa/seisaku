#include "Lib/Math/Vector.h"

namespace Lib{
namespace Math {

template <>
bool operator==<float>(const TVector2<float>& v0, const TVector2<float>& v1) {
	return (
		fabs(v0.x - v1.x) < FLT_EPSILON
		&& fabs(v0.y - v1.y) < FLT_EPSILON
		);
}

template <>
bool operator==<double>(const TVector2<double>& v0, const TVector2<double>& v1) {
	return (
		fabs(v0.x - v1.x) < DBL_EPSILON
		&& fabs(v0.y - v1.y) < DBL_EPSILON
		);
}

template <>
bool operator==<float>(const TVector3<float>& v0, const TVector3<float>& v1) {
	return (
		fabs(v0.x - v1.x) < FLT_EPSILON
		&& fabs(v0.y - v1.y) < FLT_EPSILON
		&& fabs(v0.z - v1.z) < FLT_EPSILON
		);
}

//doubleで特殊化
template <>
bool operator==<double>(const TVector3<double>& v0, const TVector3<double>& v1) {
	return (
		fabs(v0.x - v1.x) < DBL_EPSILON
		&& fabs(v0.y - v1.y) < DBL_EPSILON
		&& fabs(v0.z - v1.z) < DBL_EPSILON
		);
}

}
}//namespace Fwk