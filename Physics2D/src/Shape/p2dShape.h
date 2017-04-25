#pragma once

#include "p2dSettings.h"
#include "p2dMath.h"
#include "p2dBlockAllocator.h"

namespace Physics2D {

struct p2dRayCastInput;
struct p2dRayCastOutput;
class p2dAABB;

struct p2dMassData {
	p2dVec2 center;
	float mass;
	float I;
};


class p2dShape {
public:
	enum Type {
		e_edge = 0,
		e_chain,
		e_circle,
		e_polygon
	};
protected:
	Type shapeType;
	float radius;
public:
	p2dShape() {
	}
	virtual ~p2dShape() {

	}
public:
	Type getType() const {
		return shapeType;
	}
	float getRadius() const {
		return radius;
	}
public:
	virtual bool validate() const {
		return true;
	}
	virtual p2dShape* clone(p2dBlockAllocator* allocator) const = 0;
	virtual int getChildCount() const = 0;
	virtual bool testPoint(const p2dTransform& xf, const p2dVec2& p) const = 0;
	virtual bool rayCast(const p2dTransform& xf, int childIndex, p2dRayCastOutput* output, const p2dRayCastInput& input) const = 0;
	virtual void conputeAABB(p2dAABB* aabb, const p2dTransform& xf, int childIndex) const = 0;
	virtual void computeMass(p2dMassData* massData, float density) const = 0;
};

}
