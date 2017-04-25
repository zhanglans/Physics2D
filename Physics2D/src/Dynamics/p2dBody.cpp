#include "p2dBody.h"
#include "p2dWorld.h"
#include <new>

namespace Physics2D {

p2dBody::p2dBody(const p2dBodyDef* def, p2dWorld* w) {
	bodyType = def->bodyType;
	linearDamping = def->linearDamping;
	angularDamping = def->angularDamping;
	velocity = def->velocity;
	angularVelocity = def->angularVelocity;
	xf.set(def->position, def->angle);

	prev = next = nullptr;
	world = w;
}

p2dBody::~p2dBody() {

}

p2dFixture* p2dBody::createFixture(const p2dFixtureDef* def) {
	p2dBlockAllocator* allocator = &world->blockAllocator;

	void* mem = allocator->Allocate(sizeof(p2dFixture));
	p2dFixture* fixture = new (mem) p2dFixture();
	fixture->create(allocator, this, def);

	// add into fixtureList
	fixture->next = fixtureList;
	fixtureList = fixture;

	//recalculate mass data
	if (def->density > 0.0f) {
	//	resetMassData();
	}
	return fixture;
}

p2dFixture* p2dBody::createFixture(const p2dShape* shape, float density) {
	return NULL;
}

void p2dBody::destroyFixture(p2dFixture* fixture) {
	if (!fixture) return;
	
	bool found = false;
	if (fixture == fixtureList) {
		fixtureList = fixture->next;
		found = true;
	} else if(fixtureList) {
		p2dFixture* p = fixtureList;
		while (p->next) {
			if (p->next == fixture) {
				p->next = fixture->next;
				found = true;
				break;
			}
			p = p->next;
		}
	}
	p2dLog("p2dBody::destroyFixture: %s", found ? "found" : "not found");

	fixture->destroy(&world->blockAllocator);
	fixture->~p2dFixture();
	world->blockAllocator.Free(fixture, sizeof(p2dFixture));
}

void p2dBody::applyForceToCenter(const p2dVec2& f) {
	force += f;
}

void p2dBody::applyForce(const p2dVec2& f, const p2dVec2& point) {
	p2dVec2 axle = sweep.center - point;
	float t = cross(f, axle);

//	force += axle * (dot(f, axle) / dot(axle, axle));
	force += f;	// this is from box2d...  is it right?
	torque += t;
}

void p2dBody::applyTorque(float t) {
	torque += t;
}

void p2dBody::applyLinearImpulse(const p2dVec2& impulse, const p2dVec2& point) {
	p2dVec2 axle = sweep.center - point;
	float angularImpulse = cross(impulse, axle);

	velocity += invMass * impulse; // this is from box2d...
	angularVelocity += invI * angularImpulse;
}

void p2dBody::applyLinearImpulseToCenter(const p2dVec2& impulse) {
	velocity += invMass * impulse;
}

void p2dBody::applyAngularImpulse(float impulse) {
	angularVelocity += invI * impulse;
}

}



