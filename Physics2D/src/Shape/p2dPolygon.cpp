#include "p2dPolygon.h"
#include "p2dMath.h"
#include "p2dCollision.h"
#include <new>

namespace Physics2D {

int p2dPolygon::buildConvex(const p2dVec2* vSrc, int cnt, p2dVec2* vDest) {
	int hull[p2d_maxPolygonVertices];
	int destCnt = 0;
	//step1: find the most right vertex
	int v0 = 0;
	for (int i = 1; i < cnt; ++i) {
		if (vSrc[i].x > vSrc[v0].x) {
			v0 = i;
		} else if (vSrc[i].x == vSrc[v0].x
			&& vSrc[i].y < vSrc[v0].y) {
			v0 = i;
		}
	}
	//step2: wrapping
	hull[destCnt++] = v0;
	for (;;) {
		int candidate = 0;

		for (int i = 0; i < cnt; ++i) {
			p2dVec2 e1 = vSrc[i]-vSrc[hull[destCnt-1]];
			p2dVec2 e2 = vSrc[candidate]-vSrc[hull[destCnt-1]];
			float crossVal = cross(e1, e2);
			if (crossVal > 0.0f) {
				candidate = i;
			} else if (crossVal == 0.0f && e1.squaredLength() > e2.squaredLength()) {
				candidate = i;
			}
		}
		if (candidate == v0)
			break;
		else
			hull[destCnt++] = candidate;
	}

	for (int i = 0; i < destCnt; ++i) {
		vDest[i] = vSrc[hull[i]];
	}
	return destCnt;
}


void p2dPolygon::set(const p2dVec2* vSrc, int cnt) {
	// check and eliminate duplicated points
	p2dVec2 ps[p2d_maxPolygonVertices];
	int uniqCnt = 0;
	for (int i = 0; i < cnt; ++i) {
		bool uniq = true;
		for (int j = 0; j < uniqCnt; ++j) {
			if (squaredDistance(vSrc[i], ps[j]) < ((0.5f * p2d_linearSlop) * (0.5f * p2d_linearSlop))) {
				uniq = false;
				break;
			}
		}
		if (uniq)
			ps[uniqCnt++] = vSrc[i];
	}
	// compute convex hull
	count = buildConvex(ps, uniqCnt, vertices);
	
	// compute normals
	for (int i = 0; i < count; ++i) {
		int j = (i+1 == count) ? 0 : i+1;
		normals[i] = cross(vertices[j] - vertices[i], 1.0f);
		normals[i].normalize();
	}
}

void p2dPolygon::setAsBox(float hx, float hy, const p2dVec2& center, float angle) {
	p2dTransform xf(center, angle);

	count = 4;
	vertices[0].set(-hx, -hy);
	vertices[1].set( hx, -hy);
	vertices[2].set( hx,  hy);
	vertices[3].set(-hx,  hy);
	normals[0].set( 0.0f, -1.0f);
	normals[1].set( 1.0f,  0.0f);
	normals[2].set( 0.0f,  1.0f);
	normals[3].set(-1.0f,  0.0f);

	for (int i = 0; i < 4; ++i) {
		vertices[i] = mul(xf, vertices[i]);
		normals[i] = mul(xf.q, normals[i]);
	}
}

p2dShape* p2dPolygon::clone(p2dBlockAllocator* allocator) const {
	void* mem = allocator->Allocate(sizeof(p2dPolygon));
	p2dPolygon* copy = new (mem) p2dPolygon;

	for (int i = 0; i < count; ++i) {
		copy->vertices[i] = vertices[i];
		copy->normals[i] = normals[i];
	}
	copy->count = count;
	copy->centroid = centroid;
	return copy;
}

bool p2dPolygon::testPoint(const p2dTransform& xf, const p2dVec2& p) const {
	if (count < 3) return false;

	p2dVec2 localP = mulT(xf, p);
	for (int i = 0; i < count; ++i) {
		p2dVec2 d = localP - vertices[i];
		if (dot(d, normals[i]) > 0.0f)
			return false;
	}
	return true;
}

void p2dPolygon::conputeAABB(p2dAABB* aabb, const p2dTransform& xf, int childIndex) const {
	p2dAssert(childIndex == 0);
	p2dVec2 lower = vertices[0], upper = vertices[0];
	for (int i = 1; i < count; ++i) {
		p2dVec2 wp = mul(xf, vertices[i]);
		upper = p2dMax(upper, wp);
		lower = p2dMin(lower, wp);
	}
	p2dVec2 r(radius, radius);	// add radius
	aabb->lowerBound = lower - r;
	aabb->upperBound = upper + r;
}

bool p2dPolygon::rayCast(const p2dTransform& xf, int childIndex, p2dRayCastOutput* output, const p2dRayCastInput& input) const {
	//TODO
	return false;
}

void p2dPolygon::computeMass(p2dMassData* massData, float density) const {
	const float inv3 = 1.0f / 3.0f;
	float area = 0.0f, totalArea = 0.0f;
	p2dVec2 centroid(0.0f, 0.0f);
	float I = 0.0f;

	const p2dVec2& v0 = getVertex(0);
	for (int i = 1; i < count-1; ++i) {
		const p2dVec2& e1 = getVertex(i) - v0;
		const p2dVec2& e2 = getVertex(i+1) - v0;
		float D = cross(e1, e2);
		area = 0.5f * D;
		totalArea += area;
		centroid += area * (e1 + e2) * inv3;

		float ex1 = e1.x, ey1 = e1.y;
		float ex2 = e2.x, ey2 = e2.y;

		float intx2 = ex1*ex1 + ex2*ex1 + ex2*ex2;
		float inty2 = ey1*ey1 + ey2*ey1 + ey2*ey2;

		I += (0.25f * inv3 * D) * (intx2 + inty2);
	}
	assert(totalArea > p2d_epsilon);
	centroid *= 1.0f / totalArea;
	massData->center = centroid + v0;
	massData->mass = totalArea * density;

	// Inertia tensor relative to the local origin (point s).
	massData->I = density * I;
	// Shift to center of mass then to original body origin.
	massData->I += massData->mass * (dot(massData->center, massData->center) - dot(centroid, centroid));
}

p2dVec2 p2dPolygon::computeCentroid(const p2dVec2* vertices, int cnt) {
	p2dVec2 c; c.set(0.0f, 0.0f);
	float area = 0.0f;

	p2dVec2 pRef(0.0f, 0.0f);

	const float inv3 = 1.0f / 3.0f;

	for (int i = 0; i < cnt; ++i) {
		// Triangle vertices.
		p2dVec2 p1 = pRef;
		p2dVec2 p2 = vertices[i];
		p2dVec2 p3 = i + 1 < cnt ? vertices[i+1] : vertices[0];

		p2dVec2 e1 = p2 - p1;
		p2dVec2 e2 = p3 - p1;

		float D = cross(e1, e2);

		float triangleArea = 0.5f * D;
		area += triangleArea;

		// Area weighted centroid
		c += triangleArea * inv3 * (p1 + p2 + p3);
	}
	assert(area > p2d_epsilon);
	c *= 1.0f / area;
	return c;
}

}
