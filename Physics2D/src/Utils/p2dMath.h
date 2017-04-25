#ifndef _P2D_MATH_H_
#define _P2D_MATH_H_

#include <cmath>
#include "p2dSettings.h"

class p2dVec2 {
public:
	float x, y;
public:
	p2dVec2() : x(0.0f), y(0.0f) {}
	p2dVec2(float _x, float _y) : x(_x), y(_y) {}
	~p2dVec2() {}

public:
	void set(float _x, float _y) {
		x = _x, y = _y;
	}
	void setZero() {
		x = y = 0.0f;
	}

	float normalize() {
		float len = length();
		if (len < p2d_epsilon)
			return 0.0f;
		float invLen = 1.0f / len;
		x *= invLen, y *= invLen;
		return len;
	}

	float length() const {
		return sqrtf(x*x + y*y);
	}

	float squaredLength() const {
		return x*x + y*y;
	}

	void operator*=(float a) {
		x *= a, y *= a;
	}
	void operator+=(const p2dVec2& vec) {
		x += vec.x, y += vec.y;
	}
	void operator-=(const p2dVec2& vec) {
		x -= vec.x, y -= vec.y;
	}
	float operator()(int i) const {
		return (&x)[i];
	}
	float& operator()(int i) {
		return (&x)[i];
	}
	p2dVec2 operator-() const {
		return p2dVec2(-x, -y);
	}
};

extern p2dVec2 p2d_vec2Zero;

class p2dVec3 {
public:
	float x, y, z;
public:
	p2dVec3() : x(0.0f), y(0.0f), z(0.0f) {}
	p2dVec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	~p2dVec3() {}
public:
	void set(float _x, float _y, float _z) {
		x = _x, y = _y, z = _z;
	}
	void setZero() {
		x = y = z = 0;
	}
	float normalize() {
		float len = length();
		if (len < p2d_epsilon)
			return 0.0f;
		float invLen = 1.0f / len;
		x *= invLen; y *= invLen; z *= invLen;
		return len;
	}

	float length() const {
		return sqrtf(x*x + y*y + z*z);
	}
	float squaredLength() const {
		return x*x + y*y + z*z;
	}
	void operator+=(const p2dVec3& v) {
		x += v.x; y += v.y; z += v.z;
	}
	void operator-=(const p2dVec3& v) {
		x -= v.x; y -= v.y; z -= v.z;
	}
	void operator*=(float a) {
		x *= a; y *= a; z *= a;
	}
	float operator()(int i) const {
		return (&x)[i];
	}
	float& operator()(int i) {
		return (&x)[i];
	}
	p2dVec3 operator-() const {
		return p2dVec3(-x, -y, -z);
	}
};

class p2dMat22 {
public:
	p2dVec2 ex, ey;

public:
	p2dMat22() {}
	p2dMat22(const p2dVec2& _ex, const p2dVec2& _ey) : ex(_ex), ey(_ey) {}
	p2dMat22(float a11, float a12, float a21, float a22) {
		ex.x = a11; ex.y = a21;
		ey.x = a12; ey.y = a22;
	}
	~p2dMat22() {}
public:
	void set(const p2dVec2& _ex, const p2dVec2& _ey) {
		ex = _ex;
		ey = _ey;
	}
	void set(float a11, float a12, float a21, float a22) {
		ex.x = a11; ex.y = a21;
		ey.x = a12; ey.y = a22;
	}
	void setIdentity() {
		ex.set(1.0f, 0.0f);
		ey.set(0.0f, 1.0f);
	}
	void setZero() {
		ex.setZero();
		ey.setZero();
	}

	p2dMat22 getInverse() const {
		float a = ex.x, b = ey.x, c = ex.y, d = ey.y;
		p2dMat22 B;
		float det = a * d - b * c;
		if (det != 0.0f) {
			det = 1.0f / det;
		}
		B.ex.x =  det * d;	B.ey.x = -det * b;
		B.ex.y = -det * c;	B.ey.y =  det * a;
		return B;
	}
	p2dVec2 solve(const p2dVec2& vec) const {
		float a11 = ex.x, a12 = ey.x, a21 = ex.y, a22 = ey.y;
		float det = a11 * a22 - a12 * a21;
		if (det != 0.0f) {
			det = 1.0f / det;
		}
		p2dVec2 x(det * (a22*vec.x - a12*vec.y), det * (a11*vec.y - a21*vec.x));
		return x;
	}
};
class p2dMat33 {
public:
	p2dVec3 ex, ey, ez;
public:
	p2dMat33() {}
	p2dMat33(const p2dVec3& _ex, const p2dVec3& _ey, const p2dVec3& _ez)
		: ex(_ex), ey(_ey), ez(_ez) {}
public:
	void setZero() {
		ex.setZero();
		ey.setZero();
		ez.setZero();
	}
	void setIdentity() {
		ex.set(1.0f, 0.0f, 0.0f);
		ey.set(0.0f, 1.0f, 0.0f);
		ez.set(0.0f, 0.0f, 1.0f);
	}
	p2dVec3 solve33(const p2dVec3& b) const;
	p2dVec2 solve22(const p2dVec2& b) const;
	void getInverse22(p2dMat33* M) const;
	void getSymInverse33(p2dMat33* M) const;
};

class p2dRot {
public:
	float s, c;	// sine and cosine
public:
	p2dRot() : s(0.0f), c(1.0f) {}
	p2dRot(float _s, float _c) : s(_s), c(_c) {}
	p2dRot(float angle) {
		s = sinf(angle);
		c = cosf(angle);
	}
	~p2dRot() {}
public:
	void set(float angle) {
		s = sinf(angle);
		c = cosf(angle);
	}
	void setIdentity() {
		s = 0.0f;
		c = 1.0f;
	}
	float getAngle() const {
		return atan2f(s, c);
	}
	p2dVec2 getXAxis() const {
		return p2dVec2(c, s);
	}
	p2dVec2 getYAxis() const {
		return p2dVec2(-s, c);
	}
	p2dRot getInverse() const {
		return p2dRot(-s, c);
	}
};

class p2dTransform {
public:
	p2dVec2 p;
	p2dRot q;
public:
	p2dTransform() {}
	p2dTransform(const p2dVec2& position, const p2dRot& rotation)
		: p(position), q(rotation) {}
	p2dTransform(const p2dVec2& position, float angle)
		: p(position), q(angle) {}
	~p2dTransform() {}
public:
	void set(const p2dVec2& position, const p2dRot& rotation) {
		p = position;
		q = rotation;
	}
	void set(const p2dVec2& position, float angle) {
		p = position;
		q.set(angle);
	}
	void setIdentity() {
		p.setZero();
		q.setIdentity();
	}
};

class p2dSweep {
public:
	p2dVec2 localCenter;	// local center of mass position
	p2dVec2 center0, center;			// center world positions
	float angle0, angle;			// world angles

	/// Fraction of the current time step in the range [0,1]
	/// center0 and angle0 are the positions at alpha0.
	float alpha0;
public:
	p2dSweep() {}
	~p2dSweep() {}
public:
	void getTransform(p2dTransform* xf, float32 beta) const;
	void advance(float32 alpha);
	void normalize();
};


// p2dVec2
inline bool operator==(const p2dVec2& v1, const p2dVec2& v2) {
	return v1.x == v2.x && v1.y == v2.y;
}
inline bool operator!=(const p2dVec2& v1, const p2dVec2& v2) {
	return v1.x != v2.x || v1.y != v2.y;
}

inline p2dVec2 operator+(const p2dVec2& v1, const p2dVec2& v2) {
	return p2dVec2(v1.x + v2.x, v1.y + v2.y);
}
inline p2dVec2 operator-(const p2dVec2& v1, const p2dVec2& v2) {
	return p2dVec2(v1.x - v2.x, v1.y - v2.y);
}
inline p2dVec2 operator*(const p2dVec2& v, float a) {
	return p2dVec2(v.x * a, v.y * a);
}
inline p2dVec2 operator*(float a, const p2dVec2& v) {
	return p2dVec2(v.x * a, v.y * a);
}
inline float dot(const p2dVec2& v1, const p2dVec2& v2) {
	return v1.x*v2.x + v1.y*v2.y;
}
inline float cross(const p2dVec2& v1, const p2dVec2& v2) {
	return v1.x*v2.y - v1.y*v2.x;
}
inline p2dVec2 cross(const p2dVec2& v1, float v) {
	return p2dVec2(v1.y*v, -v1.x*v);
}
inline p2dVec2 cross(float v, const p2dVec2& v1) {
	return p2dVec2(-v1.y*v, v1.x*v);
}
inline float distance(const p2dVec2& v1, const p2dVec2& v2) {
	return (v2-v1).length();
}
inline float squaredDistance(const p2dVec2& v1, const p2dVec2& v2) {
	return (v2-v1).squaredLength();
}
//p2dVec3
inline bool operator==(const p2dVec3& v1, const p2dVec3& v2) {
	return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}
inline bool operator!=(const p2dVec3& v1, const p2dVec3& v2) {
	return v1.x != v2.x || v1.y != v2.y || v1.z != v2.z;
}
inline p2dVec3 operator+(const p2dVec3& v1, const p2dVec3& v2) {
	return p2dVec3(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
}
inline p2dVec3 operator-(const p2dVec3& v1, const p2dVec3& v2) {
	return p2dVec3(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
}
inline p2dVec3 operator*(const p2dVec3& v, float a) {
	return p2dVec3(v.x*a, v.y*a, v.z*a);
}
inline p2dVec3 operator*(float a, const p2dVec3& v) {
	return p2dVec3(v.x*a, v.y*a, v.z*a);
}
inline float dot(const p2dVec3& v1, const p2dVec3& v2) {
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}
inline p2dVec3 cross(const p2dVec3& v1, const p2dVec3& v2) {
	float _x = v1.y*v2.z - v1.z*v2.y;
	float _y = v1.z*v2.x - v1.x*v2.z;
	float _z = v1.x*v2.y - v1.y*v2.x;
	return p2dVec3(_x, _y, _z);
}
inline float distance(const p2dVec3& v1, const p2dVec3& v2) {
	return (v2-v1).length();
}
inline float squaredDistance(const p2dVec3& v1, const p2dVec3& v2) {
	return (v2-v1).squaredLength();
}

// p2dMat22
inline p2dMat22 operator+(const p2dMat22& ma, const p2dMat22& mb) {
	return p2dMat22(ma.ex + mb.ex, ma.ey + mb.ey);
}
inline p2dMat22 operator-(const p2dMat22& ma, const p2dMat22& mb) {
	return p2dMat22(ma.ex - mb.ex, ma.ey - mb.ey);
}
inline p2dVec2 mul(const p2dMat22& mat, const p2dVec2& vec) {
	return p2dVec2(mat.ex.x*vec.x + mat.ey.x*vec.y, mat.ex.y*vec.x + mat.ey.y*vec.y);
}
inline p2dMat22 mul(const p2dMat22& ma, const p2dMat22& mb) {
	p2dMat22 ret(mul(ma, mb.ex), mul(ma, mb.ey));
	return ret;
}
inline p2dVec2 mulT(const p2dMat22& mat, const p2dVec2& vec) {
	return p2dVec2(dot(mat.ex, vec), dot(mat.ey, vec));
}
inline p2dMat22 mulT(const p2dMat22& ma, const p2dMat22& mb) {
	p2dMat22 ret(mulT(ma, mb.ex), mulT(ma, mb.ey));
	return ret;
}
//p2dMat33
inline p2dMat33 operator+(const p2dMat33& ma, const p2dMat33& mb) {
	return p2dMat33(ma.ex + mb.ex, ma.ey + mb.ey, ma.ez + mb.ez);
}
inline p2dMat33 operator-(const p2dMat33& ma, const p2dMat33& mb) {
	return p2dMat33(ma.ex - mb.ex, ma.ey - mb.ey, ma.ez - mb.ez);
}
inline p2dVec3 mul(const p2dMat33& mat, const p2dVec3& vec) {
	return p2dVec3(
			mat.ex.x*vec.x + mat.ey.x*vec.y + mat.ez.x*vec.z,
			mat.ex.y*vec.x + mat.ey.y*vec.y + mat.ez.y*vec.z,
			mat.ex.z*vec.x + mat.ey.z*vec.y + mat.ez.z*vec.z
			);
}
inline p2dMat33 mul(const p2dMat33& ma, const p2dMat33& mb) {
	p2dMat33 ret(mul(ma, mb.ex), mul(ma, mb.ey), mul(ma, mb.ez));
	return ret;
}
inline p2dVec3 mulT(const p2dMat33& mat, const p2dVec3& vec) {
	return p2dVec3(dot(mat.ex, vec), dot(mat.ey, vec), dot(mat.ez, vec));
}
inline p2dMat33 mulT(const p2dMat33& ma, const p2dMat33& mb) {
	p2dMat33 ret(mulT(ma, mb.ex), mulT(ma, mb.ey), mulT(ma, mb.ez));
	return ret;
}
//p2dRot
inline p2dVec2 mul(const p2dRot& rot, const p2dVec2& vec) {
	/**	 _		   _
	 *  |cos	-sin|
	 *	|sin	 cos| * vec
	 *	|_         _|
	 */
	return p2dVec2(vec.x*rot.c - vec.y*rot.s, vec.x*rot.s + vec.y*rot.c);
}
inline p2dVec2 mulT(const p2dRot& rot, const p2dVec2& vec) {
   return p2dVec2(vec.x*rot.c + vec.y*rot.s, vec.y*rot.c - vec.x*rot.s);
}
inline p2dRot mul(const p2dRot& rot1, const p2dRot& rot2) {
	p2dRot ret;
	ret.c = rot1.c*rot2.c - rot1.s*rot2.s;
	ret.s = rot1.s*rot2.c + rot1.c*rot2.s;
	return ret;
}
inline p2dRot mulT(const p2dRot& rot1, const p2dRot& rot2) {
	p2dRot ret;
	ret.c = rot1.c*rot2.c + rot1.s*rot2.s;
	ret.s = rot1.c*rot2.s - rot1.s*rot2.c;
	return ret;
}
//p2dTransform
inline p2dVec2 mul(const p2dTransform& T, const p2dVec2& vec) {
	float x = (vec.x*T.q.c - vec.y*T.q.s) + T.p.x;
	float y = (vec.x*T.q.s + vec.y*T.q.c) + T.p.y;
	return p2dVec2(x, y);
}
inline p2dVec2 mulT(const p2dTransform& T, const p2dVec2& vec) {
	float px = vec.x - T.p.x;
	float py = vec.y - T.p.y;
	float x = px*T.q.c + py*T.q.s;
	float y = py*T.q.c - px*T.q.s;
	return p2dVec2(x, y);
}
inline p2dTransform mul(const p2dTransform& A, const p2dTransform& B) {
	p2dTransform C;
	C.q = mul(A.q, B.q);
	C.p = mul(A.q, B.p) + A.p;		//****
	return C;
}
inline p2dTransform mulT(const p2dTransform& A, const p2dTransform& B) {
	p2dTransform C;
	C.q = mulT(A.q, B.q);
	C.p = mulT(A.q, B.p - A.p);
	return C;
}

// utils
template<typename T>
inline T p2dAbs(T t) {
	return t > T(0) ? t : -t;
}
inline p2dVec2 p2dAbs(const p2dVec2& vec) {
	return p2dVec2(p2dAbs(vec.x), p2dAbs(vec.y));
}
inline p2dMat22 p2dAbs(const p2dMat22& mat) {
	return p2dMat22(p2dAbs(mat.ex), p2dAbs(mat.ey));
}
inline p2dMat22 p2dAbs(const p2dRot& rot) {
	float absc = p2dAbs(rot.c);
	float abss = p2dAbs(rot.s);
	return p2dMat22(absc, abss, abss, absc);
}

template<typename T>
inline T p2dMin(T a, T b) {
	return a > b ? b : a;
}
inline p2dVec2 p2dMin(const p2dVec2& v1, const p2dVec2& v2) {
	return p2dVec2(p2dMin(v1.x, v2.x), p2dMin(v1.y, v2.y));
}

template<typename T>
inline T p2dMax(T a, T b) {
	return a < b ? b : a;
}
inline p2dVec2 p2dMax(const p2dVec2& v1, const p2dVec2& v2) {
	return p2dVec2(p2dMax(v1.x, v2.x), p2dMax(v1.y, v2.y));
}

template<typename T>
inline T p2dClamp(T a, T low, T high) {
	return p2dMax(low, p2dMin(a, high));
}
inline p2dVec2 p2dClamp(const p2dVec2& a, const p2dVec2& low, const p2dVec2& high) {
	p2dVec2 ret;
	ret.x = p2dClamp(a.x, low.x, high.x);
	ret.y = p2dClamp(a.y, low.y, high.y);
	return ret;
}

template<typename T>
inline void p2dSwap(T& a, T& b) {
	T tmp = a;
	a = b;
	b = tmp;
}

inline float sign(float a) {
	return a < 0.0f ? -1.0f : 1.0f;
}

#endif
