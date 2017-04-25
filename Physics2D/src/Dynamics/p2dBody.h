#ifndef _P2D_BODY_H_
#define _P2D_BODY_H_

#include "p2dMath.h"
#include "p2dShape.h"
#include "p2dFixture.h"

namespace Physics2D {

class p2dWorld;

enum p2dBodyType {
	staticBody = 0,
	dynamicBody
};

class p2dBodyDef {
public:
	p2dBodyType bodyType = staticBody;

	float linearDamping {0.0f};
	float angularDamping {0.0f};

	p2dVec2 velocity {0.0f, 0.0f};
	float angularVelocity = 0.0f;

	p2dVec2 position {0.0f, 0.0f};
	float angle = 0.0f;

};

class p2dBody {
private:
	p2dBodyType bodyType = staticBody;

	float linearDamping;
	float angularDamping;

	p2dVec2 velocity {0.0f, 0.0f};
	float angularVelocity {0.0f};

	p2dTransform xf;
	p2dSweep sweep;

	p2dFixture* fixtureList;	// at most one fixture yet
	int fixtureCount = 0;	// unused yet

	float mass, invMass;
	float I, invI;

	p2dVec2 force {0.0f, 0.0f};
	float torque {0.0f};

	p2dWorld* world;
	p2dBody* prev {nullptr};
	p2dBody* next {nullptr};

public:
	p2dBody(const p2dBodyDef* bodyDef, p2dWorld* world);
	virtual ~p2dBody();
public:
	p2dFixture* createFixture(const p2dFixtureDef* def);
	p2dFixture* createFixture(const p2dShape* shape, float density);
	void destroyFixture(p2dFixture* fixture);

	void applyForceToCenter(const p2dVec2& f);
	void applyForce(const p2dVec2& f, const p2dVec2& point);
	void applyTorque(float torque);

	void applyLinearImpulse(const p2dVec2& impulse, const p2dVec2& point);
	void applyLinearImpulseToCenter(const p2dVec2& impulse);
	void applyAngularImpulse(float impulse);
	
	float getMass() const;
	float getInertia() const;
	void getMassData(p2dMassData* massData);

	float getAngularVelocity() const;
	const p2dVec2& getLinearVelocity() const;
	const p2dVec2& getPosition() const;
	float getAngle() const;
	const p2dTransform& getTransform() const;

	const p2dVec2& getWorldCenter() const;
	const p2dVec2& getLocalCenter() const;

	p2dVec2 getWorldPoint(const p2dVec2& localPoint) const;
	p2dVec2 getWorldVector(const p2dVec2& localVector) const;
	p2dVec2 getLocalPoint(const p2dVec2& worldPoint) const;
	p2dVec2 getLocalVector(const p2dVec2& localVector) const;

	p2dVec2 getVelocityFromWorldPoint(const p2dVec2& worldPoint) const;
	p2dVec2 getVelocityFromLocalPoint(const p2dVec2& localPoint) const;
	float getLinearDamping() const;
	float getAngularDamping() const;
	p2dBodyType getType() const;

	const p2dFixture* getFixtureList() const {
		return fixtureList;
	}
	int getFixtureCount() const {
		return fixtureCount;
	}

	// setters
	void setType(p2dBodyType type);

	void setMassData(const p2dMassData& massData);

	void setTransform(const p2dVec2& position, float angle);
	void setAngularDamping(float angularDamping);
	void setLinearDamping(float linearDamping);
	void setAngularVelocity(float omega);
	void setLinearVelocity(const p2dVec2& v);

	//For test only
	void rotate(float angle);
	void moveTo(const p2dVec2& position);
public:
	friend class p2dWorld;
};


inline float p2dBody::getMass() const {
	return mass;
}
inline float p2dBody::getInertia() const {
	return I + mass * dot(sweep.localCenter, sweep.localCenter);		//??
}
inline void p2dBody::getMassData(p2dMassData* massData) {
	massData->center = sweep.localCenter;
	massData->mass = mass;
	massData->I = getInertia();
}

inline float p2dBody::getAngularVelocity() const {
	return angularVelocity;
}
inline const p2dVec2& p2dBody::getLinearVelocity() const {
	return velocity;
}
inline const p2dVec2& p2dBody::getPosition() const {
	return xf.p;
}
inline float p2dBody::getAngle() const {
	return sweep.angle;
}
inline const p2dTransform& p2dBody::getTransform() const {
	return xf;
}

inline const p2dVec2& p2dBody::getWorldCenter() const {
	return sweep.center;
}

inline const p2dVec2& p2dBody::getLocalCenter() const {
	return sweep.localCenter;
}

inline p2dVec2 p2dBody::getWorldPoint(const p2dVec2& localPoint) const {
	return mul(xf, localPoint);
}
inline p2dVec2 p2dBody::getWorldVector(const p2dVec2& localVector) const {
	return mul(xf.q, localVector);
}
inline p2dVec2 p2dBody::getLocalPoint(const p2dVec2& worldPoint) const {
	return mulT(xf, worldPoint);
}
inline p2dVec2 p2dBody::getLocalVector(const p2dVec2& worldVector) const {
	return mulT(xf.q, worldVector);
}

inline p2dVec2 p2dBody::getVelocityFromWorldPoint(const p2dVec2& worldPoint) const {
	return cross(angularVelocity, worldPoint - getWorldCenter()) + velocity;
}

inline p2dVec2 p2dBody::getVelocityFromLocalPoint(const p2dVec2& localPoint) const {
	return getVelocityFromWorldPoint(getWorldPoint(localPoint));
}

inline float p2dBody::getLinearDamping() const {
	return linearDamping;
}
inline float p2dBody::getAngularDamping() const {
	return angularDamping;
}
inline p2dBodyType p2dBody::getType() const {
	return bodyType;
}

// setters
inline void p2dBody::setType(p2dBodyType type) {

}

inline void p2dBody::setMassData(const p2dMassData& massData) {

}

inline void p2dBody::setTransform(const p2dVec2& position, float angle) {
	xf.set(position, angle);
	sweep.center0 = sweep.center = mul(xf, sweep.localCenter);

	sweep.angle0 = sweep.angle = angle;
	
}

inline void p2dBody::setAngularDamping(float _angularDumping) {
	angularDamping = _angularDumping;
}
inline void p2dBody::setLinearDamping(float _linearDamping) {
	linearDamping = _linearDamping;
}
inline void p2dBody::setAngularVelocity(float w) {
	if (bodyType == staticBody)
		return;
	angularVelocity = w;
}
inline void p2dBody::setLinearVelocity(const p2dVec2& v) {
	if (bodyType == staticBody)
		return;
	velocity = v;
}

inline void p2dBody::rotate(float angle) {
	setTransform(getPosition(), getAngle() + angle);
}

inline void p2dBody::moveTo(const p2dVec2& position) {
	setTransform(position, getAngle());
}

}

#endif
