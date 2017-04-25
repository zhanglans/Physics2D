#include "p2dWorld.h"
#include "p2dBody.h"
#include <new>

namespace Physics2D {

const int p2dWorld::defaultVelocityIterations = 10;
const int p2dWorld::defaultPositionIterations = 10;

p2dBody* p2dWorld::createBody(const p2dBodyDef* def) {
	void* mem = blockAllocator.Allocate(sizeof(p2dBody));
	p2dBody* body = new (mem) p2dBody(def, this);

	body->next = bodyList;
	if (bodyList)
		bodyList->prev = body;
	bodyList = body;
	
	++bodyCount;
	return body;
}

void p2dWorld::destroyBody(p2dBody* body) {
	if (!body) return;

	// delete fixture list of the body
	p2dFixture* f = body->fixtureList;
	while (f) {
		p2dFixture* f0 = f;
		f = f->next;

		f0->destroy(&blockAllocator);
		f0->~p2dFixture();
		blockAllocator.Free(f0, sizeof(p2dFixture));
	}

	// delete body in bodyList and free memory
	if (body->prev) {
		body->prev->next = body->next;
	}

	if (body->next) {
		body->next->prev = body->prev;
	}

	if (body == bodyList) {
		bodyList = body->next;
	}

	body->~p2dBody();
	blockAllocator.Free(body, sizeof(p2dBody));

	--bodyCount;
}

void p2dWorld::step(float timeStep, int velocityIterations, int positionIterations) {

}

}


