#ifndef _P2D_DRAW_H_
#define _P2D_DRAW_H_

#include "p2dSettings.h"
#include "p2dMath.h"

class p2dColor {
public:
	float32 r {0.0f}, g {0.0f}, b {0.0f}, a{0.0f};
public:
	p2dColor() { }
	p2dColor(float32 _r, float32 _g, float32 _b, float32 _a)
			: r(_r), g(_g), b(_b), a(_a) {
	}
	void set(float32 _r, float32 _g, float32 _b, float32 _a) {
		r = _r, g = _g, b = _b, a = _a;
	}
};

class p2dDraw {
protected:
	uint32 drawFlags {0x00};
public:
	enum
	{
		e_shapeBit				= 0x0001,	///< draw shapes
		e_jointBit				= 0x0002,	///< draw joint connections
		e_aabbBit				= 0x0004,	///< draw axis aligned bounding boxes
		e_pairBit				= 0x0008,	///< draw broad-phase pairs
		e_centerOfMassBit		= 0x0010	///< draw center of mass frame
	};
public:
	p2dDraw() { }
	virtual ~p2dDraw() { }

	void setFlags(uint32 flags);
	uint32 getFlags() const;
	void appendFlags(uint32 flags);
	void clearFlags(uint32 flags);
	void clearFlags();
public:
	virtual void drawPolygon(const p2dVec2* vertices, int vertexCount, const p2dColor& color) = 0;
	virtual void drawSolidPolygon(const p2dVec2* vertices, int vertexCount, const p2dColor& color) = 0;

	virtual void drawCircle(const p2dVec2& center, float32 radius, const p2dColor& color) = 0;
	virtual void drawSolidCircle(const p2dVec2& center, float32 radius, const p2dVec2& axis, const p2dColor& color) = 0;
	
	virtual void drawSegment(const p2dVec2& p1, const p2dVec2& p2, const p2dColor& color) = 0;

	virtual void drawTransform(const p2dTransform& xf) = 0;

	virtual void drawPoint(const p2dVec2& p, float32 size, const p2dColor& color) = 0;
};

inline void p2dDraw::setFlags(uint32 flags) {
	drawFlags = flags;
}

inline uint32 p2dDraw::getFlags() const {
	return drawFlags;
}

inline void p2dDraw::appendFlags(uint32 flags) {
	drawFlags |= flags;
}

inline void p2dDraw::clearFlags(uint32 flags) {
	drawFlags &= ~flags;
}

inline void p2dDraw::clearFlags() {
	drawFlags = 0x00;
}

#endif

