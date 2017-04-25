#include "p2dCollision.h"
#include "p2dMath.h"
#include "p2dPolygon.h"

namespace Physics2D {

float p2dFindMaxSeparation(int* edgeIndex, const p2dPolygon* poly1, const p2dTransform& xf1,
									   const p2dPolygon* poly2, const p2dTransform& xf2) {
	p2dTransform xf = mulT(xf2, xf1);	//xfA向xfB变换
	int countA = poly1->count;
	int countB = poly2->count;

	const p2dVec2* nsA = poly1->normals;
	const p2dVec2* vsA = poly1->vertices;
	const p2dVec2* vsB = poly2->vertices;

	float sep = -p2d_maxFloat;
	int bestIndex = 0;
	for (int i = 0; i < countA; ++i) {
		// transform coordinate in the outer loop, instead of the inner one
		p2dVec2 vA = mul(xf, vsA[i]);
		p2dVec2 n = mul(xf.q, nsA[i]);

		float minS = dot(n, (vsB[0] - vA));
		
		for (int j = 1; j < countB; ++j) {
			float s = dot(n, (vsB[j] - vA));
			if (minS > s) {
				minS = s;	// find gap between polygons (from each edge of poly1)
			}
		}
		// find the bigest gap
		if (sep < minS) {
			sep = minS;
			bestIndex = i;
		}
	}
	*edgeIndex = bestIndex;
	return sep;
}

void p2dFindIncidentEdge(p2dClipVertex e[2], int refEdge, 
						const p2dPolygon* poly1, const p2dTransform& xf1,
						const p2dPolygon* poly2, const p2dTransform& xf2) {

	int count2 = poly2->count;
	const p2dVec2& normal = poly1->normals[refEdge];

	int index = 0;
	float minDot = dot(normal, poly2->normals[0]);
	for (int i = 1; i < count2; ++i) {
		float dotVal = dot(normal, poly2->normals[i]);
		if (dotVal < minDot) {
			minDot = dotVal;
			index = i;
		}
	}
	int i1 = index;
	int i2 = (i1+1 == count2) ? 0 : i1+1;

	e[0].v = mul(xf2, poly2->vertices[i1]);
	e[1].v = mul(xf2, poly2->vertices[i2]);
}

int p2dClipSegment(p2dClipVertex vOut[2], p2dClipVertex vIn[2], const p2dVec2& normal, float offset) {
	int clipPointsNum = 0;

	float d1 = dot(vIn[0].v, normal) - offset;
	float d2 = dot(vIn[1].v, normal) - offset;

	if (d1 >= 0.0f) vOut[clipPointsNum++].v = vIn[0].v;
	if (d2 >= 0.0f) vOut[clipPointsNum++].v = vIn[1].v;

	if (d1 * d2 < 0.0f) {
		// means real clipping
		float d = d1 / (d1 - d2);
		vOut[clipPointsNum++].v = vIn[0].v + (vIn[1].v - vIn[0].v) * d;
	}
	return clipPointsNum;
}

void p2dCollidePolygons(p2dManifold* manifold, const p2dPolygon* polygonA, const p2dTransform& xfA,
									 const p2dPolygon* polygonB, const p2dTransform& xfB) {

	float totalRadius = polygonA->getRadius() + polygonB->getRadius();

	int edgeA, edgeB;
	float sepA = p2dFindMaxSeparation(&edgeA, polygonA, xfA, polygonB, xfB);
	if (sepA > totalRadius)
		return;	// no collision

	float sepB = p2dFindMaxSeparation(&edgeB, polygonB, xfB, polygonA, xfA);
	if (sepB > totalRadius)
		return;	// no collision

	const p2dPolygon *poly1, *poly2;
	p2dTransform xf1, xf2;
	int edgeIndex;
	bool flip = false;

//	const float k_tol = 0.1f * p2d_linearSlop;

	if (sepB > sepA) {
		poly1 = polygonB;
		poly2 = polygonA;
		xf1 = xfB;
		xf2 = xfA;
		edgeIndex = edgeB;
		flip = true;		// flip~
	} else {
		poly1 = polygonA;
		poly2 = polygonB;
		xf1 = xfA;
		xf2 = xfB;
		edgeIndex = edgeA;
	}

	p2dClipVertex incidentEdge[2];
	p2dFindIncidentEdge(incidentEdge, edgeIndex, poly1, xf1, poly2, xf2);

	int i1 = edgeIndex;
	int i2 = (i1+1 == poly1->count) ? 0 : i1+1;

	p2dVec2 v1 = poly1->vertices[i1];
	p2dVec2 v2 = poly1->vertices[i2];

	p2dVec2 localTangent = v2 - v1;

	p2dVec2 tangent = mul(xf1.q, localTangent);

	const p2dVec2& localNormal = poly1->normals[edgeIndex];
	p2dVec2 normal = mul(xf1.q, localNormal);

	v1 = mul(xf1, v1);	//transform v1, v2 into world coordinate
	v2 = mul(xf1, v2);

	float offset1 = dot(v1, tangent) - totalRadius;	// clip offsets
	float offset2 = -dot(v2, tangent) - totalRadius;

	p2dClipVertex clipPoints1[2], clipPoints2[2];
	int np = p2dClipSegment(clipPoints1, incidentEdge, tangent, offset1);
	if (np < 2) return;

	np = p2dClipSegment(clipPoints2, clipPoints1, -tangent, offset2);
	if (np < 2) return;

	manifold->normal = normal;	// localNormal?
	for (int i = 0; i < np; ++i) {
		// only remain the point(s) which is(are) actually across the edge
		if (dot(clipPoints2[i].v - v1, normal) <= totalRadius) {
			manifold->points[i].point = clipPoints2[i].v;

			if (flip) {
				// for warm starting (not ready yet)
			}
		}
	}
	manifold->pointCount = np;
}

}
