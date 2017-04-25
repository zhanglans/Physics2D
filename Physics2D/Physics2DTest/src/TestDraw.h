#ifndef _TEST_DRAW_H_
#define _TEST_DRAW_H_

#include "p2dDraw.h"

class Camera {
private:

public:
	p2dVec2 convertScreenToWorld(int x, int y);
	p2dVec2 convertScreenToWorld(const p2dVec2& ps);
	p2dVec2 convertWorldToScreen(const p2dVec2& pw);
};

class TestDraw : public p2dDraw {
public:
	TestDraw() {}
public:
	void init();

	virtual void drawPolygon(const p2dVec2* vertices, int vertexCount, const p2dColor& color) override;
	virtual void drawSolidPolygon(const p2dVec2* vertices, int vertexCount, const p2dColor& color) override;

	virtual void drawCircle(const p2dVec2& center, float32 radius, const p2dColor& color) override;
	virtual void drawSolidCircle(const p2dVec2& center, float32 radius, const p2dVec2& axis, const p2dColor& color) override;
	
	virtual void drawSegment(const p2dVec2& p1, const p2dVec2& p2, const p2dColor& color) override;

	virtual void drawTransform(const p2dTransform& xf) override;

	virtual void drawPoint(const p2dVec2& p, float32 size, const p2dColor& color) override;

	void drawText(int x, int y, const char* str);
};

extern TestDraw g_testDraw;
extern Camera g_camera;

#endif
