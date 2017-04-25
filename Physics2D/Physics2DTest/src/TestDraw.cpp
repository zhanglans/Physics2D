#include "TestDraw.h"
#include "glut.h"
#include <cstdio>
#include <cstring>

TestDraw g_testDraw;
Camera g_camera;

p2dVec2 Camera::convertScreenToWorld(int x, int y) {
	return p2dVec2(x, y);
}
p2dVec2 Camera::convertScreenToWorld(const p2dVec2& ps) {
	return ps;
}

p2dVec2 Camera::convertWorldToScreen(const p2dVec2& pw) {
	
	return pw;
}

void TestDraw::drawPolygon(const p2dVec2* vertices, int vertexCount, const p2dColor& color) {
	glColor4f(color.r, color.g, color.b, color.a);

	glBegin(GL_LINE_LOOP);
		for (int i = 0; i < vertexCount; ++i) {
			p2dVec2 v = g_camera.convertWorldToScreen(vertices[i]);
			glVertex2f(v.x, v.y);
		}
	glEnd();
}

void TestDraw::drawSolidPolygon(const p2dVec2* vertices, int vertexCount, const p2dColor& color) {
	glColor4f(color.r, color.g, color.b, color.a);

	glBegin(GL_FILL);
		for (int i = 0; i < vertexCount; ++i) {
			p2dVec2 v = g_camera.convertWorldToScreen(vertices[i]);
			glVertex2f(v.x, v.y);
		}
	glEnd();
}

void TestDraw::drawCircle(const p2dVec2& center, float32 radius, const p2dColor& color) {

}

void TestDraw::drawSolidCircle(const p2dVec2& center, float32 radius, const p2dVec2& axis, const p2dColor& color) {

}

void TestDraw::drawSegment(const p2dVec2& p1, const p2dVec2& p2, const p2dColor& color) {
	glColor4f(color.r, color.g, color.b, color.a);

	glBegin(GL_LINES);
		p2dVec2 v1 = g_camera.convertWorldToScreen(p1);
		p2dVec2 v2 = g_camera.convertWorldToScreen(p2);
		glVertex2f(v1.x, v1.y);
		glVertex2f(v2.x, v2.y);
	glEnd();
}

void TestDraw::drawTransform(const p2dTransform& xf) {

}

void TestDraw::drawPoint(const p2dVec2& p, float32 size, const p2dColor& color) {
	glColor4f(color.r, color.g, color.b, color.a);
	glPointSize(size);

	glBegin(GL_POINT);
		p2dVec2 v = g_camera.convertWorldToScreen(p);
		glVertex2f(v.x, v.y);
	glEnd();
}


void TestDraw::drawText(int x, int y, const char *string)
{
	int len, i;

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(0.9f, 0.6f, 0.6f);
	glRasterPos2i(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
