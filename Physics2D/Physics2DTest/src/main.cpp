/*
 * main.cpp
 *
 *  Created on: 2017年2月15日
 *      Author: Administrator
 */

#include "glut.h"
#include <cstring>
#include "p2dSettings.h"
#include "p2dBody.h"
#include "p2dWorld.h"

#include "p2dPolygon.h"

#include "TestDraw.h"

using namespace Physics2D;

const int MAX_BODY_NUM = 100;
const int MAX_JOINT_NUM = 50;
const int MAX_SPRING_NUM = 10;

const float iterations = 30;
const p2dVec2 g(0.0f, -9.8f);
const float timeStep = 1.0f / 60.0f;	

namespace {
	p2dWorld world(g);
	int windowIdentifier;

	p2dBodyDef def;
	p2dBody* g_testBody = world.createBody(&def);

	p2dFixtureDef fixtureDef;

	p2dTransform g_transform;
	float g_angle = 0.0f;
	p2dVec2 g_vs[p2d_maxPolygonVertices];

	p2dPolygon g_polygon;
}

int getBodyPolygonVerticesInWorldCoordinate(const p2dBody* body, int fixtureIndex, p2dVec2 vs[p2d_maxPolygonVertices]) {
	const p2dFixture* fixture= body->getFixtureList();

	for (int i = 0; i < fixtureIndex; ++i) {
		fixture = fixture->next;
	}
	const p2dPolygon* shape = reinterpret_cast<const p2dPolygon*>(fixture->getShape());
	const p2dVec2* vertices = shape->getVertices();
	int vertexCount = shape->getVertexCount();
	for (int i = 0; i < vertexCount; ++i) {
		vs[i] = body->getWorldPoint(vertices[i]);
	}
	return vertexCount;
}

void simpleDraw() {
	int vsCnt = getBodyPolygonVerticesInWorldCoordinate(g_testBody, 0, g_vs);
	g_testDraw.drawPolygon(g_vs, vsCnt, p2dColor(0.9f, 0.8f, 0.0f, 0.5f));
}


void simulationLoop() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, -7.0f, -25.0f);

	simpleDraw();

	glutSwapBuffers();
}

void onKeyboard(unsigned char key, int x, int y) {

}

void onMouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			p2dVec2 pw = g_camera.convertScreenToWorld(x, y);
			g_testBody->moveTo(pw);
		} else if (button == GLUT_RIGHT_BUTTON) {
			g_testBody->rotate(0.1f);
		}
	}
}

void onReshape(int width, int height) {

	if (height == 0)
		height = 1;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)width / (float)height, 0.1, 100.0);
}


int main(int argc, char* argv[]) {
	g_polygon.setAsBox(2.0f, 1.0f, p2d_vec2Zero, 0.0f);
	fixtureDef.shape = &g_polygon;

	g_testBody->createFixture(&fixtureDef);

	glutInit(&argc, argv);
	glutInitWindowSize(1000, 800);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	windowIdentifier = glutCreateWindow("Physics2D");

	glutDisplayFunc(simulationLoop);
	glutReshapeFunc(onReshape);
	glutKeyboardFunc(onKeyboard);
	glutMouseFunc(onMouse);
	glutIdleFunc(simulationLoop);


	glutMainLoop();
	return 0;
}


