#ifndef _P2D_POLYGON_H_
#define _P2D_POLYGON_H_

#include "p2dSettings.h"
#include "p2dShape.h"
#include "p2dMath.h"

namespace Physics2D {

class p2dPolygon : public p2dShape {
public:
	p2dVec2 vertices[p2d_maxPolygonVertices];
	p2dVec2 normals[p2d_maxPolygonVertices];
	int count;
	p2dVec2 centroid;
public:
	p2dPolygon()
		: count(0)
	{
		shapeType = e_polygon;
		radius = p2d_polygonRadius;
	}
	
	~p2dPolygon() {}

public:

	int getVertexCount() const {
		return count;
	}
	const p2dVec2& getVertex(int index) const {
		p2dAssert(index < count);
		return vertices[index];
	}
	const p2dVec2* getVertices() const {
		return vertices;
	}
	virtual int getChildCount() const {
		return 1;
	}

	void set(const p2dVec2* vs, int cnt);
	void setAsBox(float hx, float hy, const p2dVec2& center = p2d_vec2Zero, float angle = 0.0f);

	p2dShape* clone(p2dBlockAllocator* allocator) const;
	
	
	virtual bool testPoint(const p2dTransform& xf, const p2dVec2& p) const;
	virtual bool rayCast(const p2dTransform& xf, int childIndex, p2dRayCastOutput* output, const p2dRayCastInput& input) const;
	virtual void conputeAABB(p2dAABB* aabb, const p2dTransform& xf, int childIndex) const;
	virtual void computeMass(p2dMassData* massData, float density) const;

private:
	static int buildConvex(const p2dVec2* vSrc, int cnt, p2dVec2* vDest);
	static p2dVec2 p2dPolygon::computeCentroid(const p2dVec2* vertices, int cnt);
};

}

#endif

