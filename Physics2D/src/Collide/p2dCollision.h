#ifndef _P2D_COLLISION_H_
#define _P2D_COLLISION_H_

#include "p2dMath.h"

namespace Physics2D {

class p2dPolygon;

struct p2dRayCastInput {
	p2dVec2 p1, p2;
	float maxFraction;
};

struct p2dRayCastOutput {
	p2dVec2 normal;
	float fraction;
};

class p2dAABB {
public:
	p2dVec2 lowerBound, upperBound;
public:
	p2dAABB() {}
	~p2dAABB() {}
public:
	p2dVec2 getCenter() const {
		return (upperBound + lowerBound) * 0.5f;
	}
	p2dVec2 getExtends() const {
		return (upperBound - lowerBound) * 0.5f;
	}
	float getPerimeter() const {
		return 2.0f * (upperBound.x - lowerBound.x + upperBound.y - lowerBound.y);
	}
	void combine(const p2dAABB& aabb) {
		lowerBound = p2dMin(lowerBound, aabb.lowerBound);
		upperBound = p2dMin(upperBound, aabb.upperBound);
	}
	void combine(const p2dAABB& aabb1, const p2dAABB& aabb2) {
		lowerBound = p2dMin(aabb1.lowerBound, aabb2.lowerBound);
		upperBound = p2dMin(aabb1.upperBound, aabb2.upperBound);
	}
	bool contains(const p2dAABB& aabb) const {
		bool ret = true;
		ret = ret && aabb.upperBound.x <= upperBound.x && aabb.upperBound.y <= upperBound.y;
		ret = ret && aabb.lowerBound.x >= lowerBound.x && aabb.lowerBound.y >= lowerBound.y;
		return ret;
	}
	bool rayCast(p2dRayCastOutput* output, const p2dRayCastInput& input) const;
};

struct p2dMTD {
	p2dVec2 pushVec;
	float depth;
};

struct p2dManifoldPoint {
	p2dVec2 point;
};

struct p2dManifold {
	p2dManifoldPoint points[p2d_maxContactPoints];
	int pointCount;
	p2dVec2 normal;
};

struct p2dClipVertex {
	p2dVec2 v;
};

void p2dCollidePolygons(p2dManifold* manifold, const p2dPolygon* polygonA, const p2dTransform& xfA,
							   const p2dPolygon* polygonB, const p2dTransform& xfB);

}

#endif
