#ifndef _P2D_SETTING_H_
#define _P2D_SETTING_H_

#include <cstddef>
#include <cfloat>
#include <cstdlib>
#include <assert.h>

#define P2D_DEBUG_DRAW


using int8 = signed char;
using uint8 = unsigned char;
using int16 = short;
using uint16 = unsigned short;
using int32 = int;
using uint32 = unsigned int;
using int64 = long long;
using uint64 = unsigned long long;
using float32 = float;
using float64 = double;

using size_type = size_t;

const float32 p2d_maxFloat = FLT_MAX;
const float32 p2d_epsilon = FLT_EPSILON;
const float p2d_pi = 3.14159265358979323846264f;

inline void* p2dAlloc(uint32 size) {
	return malloc(size);
}
inline void p2dFree(void *p) {
	free(p);
}
void p2dLog(const char*, ...);

#define p2dAssert assert


#define p2d_linearSlop			0.005f

#define p2d_polygonRadius		(2.0f * p2d_linearSlop)

#define p2d_maxPolygonVertices	8
#define p2d_maxContactPoints	2

#endif
