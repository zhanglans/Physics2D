#include "p2dMath.h"

p2dVec2 p2d_vec2Zero;

p2dVec3 p2dMat33::solve33(const p2dVec3& b) const {
	float det = dot(ex, cross(ey, ez));
	if (det != 0.0f) {
		det = 1.0f / det;
	}
	p2dVec3 x;
	x.x = det * dot(b, cross(ey, ez));
	x.y = det * dot(ex, cross(b, ez));
	x.z = det * dot(ex, cross(ey, b));
	return x;
}

p2dVec2 p2dMat33::solve22(const p2dVec2& b) const {
	float a11 = ex.x, a12 = ey.x, a21 = ex.y, a22 = ey.y;
	float det = a11 * a22 - a12 * a21;
	if (det != 0.0f) {
		det = 1.0f / det;
	}
	p2dVec2 x(det * (a22*b.x - a12*b.y), det * (a11*b.y - a21*b.x));
	return x;
}

void p2dMat33::getInverse22(p2dMat33* M) const {
	float a = ex.x, b = ey.x, c = ex.y, d = ey.y;
	float det = a * d - b * c;
	if (det != 0.0f) {
		det = 1.0f / det;
	}
	M->ex.x =  det * d;	M->ey.x = -det * b; M->ez.x = 0.0f;
	M->ex.y = -det * c;	M->ey.y =  det * a; M->ez.y = 0.0f;
	M->ex.z =  0.0f;    M->ey.z =  0.0f;    M->ez.z = 0.0f;
}

void p2dMat33::getSymInverse33(p2dMat33* M) const {
	float det = dot(ex, cross(ey, ez));
	if (det != 0.0f) {
		det = 1.0f / det;
	}
	float a11 = ex.x, a12 = ey.x, a13 = ez.x;
	float a22 = ey.y, a23 = ez.y;
	float a33 = ez.z;

	M->ex.x = det * (a22 * a33 - a23 * a23);
	M->ex.y = det * (a13 * a23 - a12 * a33);
	M->ex.z = det * (a12 * a23 - a13 * a22);

	M->ey.x = M->ex.y;
	M->ey.y = det * (a11 * a33 - a13 * a13);
	M->ey.z = det * (a13 * a12 - a11 * a23);

	M->ez.x = M->ex.z;
	M->ez.y = M->ey.z;
	M->ez.z = det * (a11 * a22 - a12 * a12);
}


void p2dSweep::getTransform(p2dTransform* xf, float32 beta) const {
	xf->p = center0 + (center - center0) * beta;
	xf->q.set(angle0 + (angle - angle0) * beta);
	// Shift from center to origin
	xf->p -= mul(xf->q, localCenter);
}

void p2dSweep::advance(float32 alpha) {
	p2dAssert(alpha0 < 1.0f);
	float beta = (alpha - alpha0) / (1.0f - alpha0);
	center0 += (center - center0) * beta;
	angle0 += (angle - angle0) * beta;
	alpha0 = alpha;
}

// normalize the angle and angle0 to be between 0~2*pi
void p2dSweep::normalize() {
	float twoPi = 2.0f * p2d_pi;
	float d =  twoPi * floorf(angle0 / twoPi);
	angle0 -= d;
	angle -= d;
}
