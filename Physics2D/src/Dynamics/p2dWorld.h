#ifndef _P2D_WORLD_H_
#define _P2D_WORLD_H_

#include "p2dSettings.h"
#include "p2dMath.h"
#include "p2dBlockAllocator.h"
#include "p2dStackAllocator.h"

namespace Physics2D {

class p2dBody;
class p2dBodyDef;

class p2dWorld {
private:
	static const int defaultVelocityIterations;
	static const int defaultPositionIterations;
private:
	p2dVec2 gravity;

	p2dBlockAllocator blockAllocator;
	p2dStackAllocator stackAllocator;

	p2dBody* bodyList {nullptr};
	int bodyCount {0};

public:
	p2dWorld(const p2dVec2& g) {
		gravity = g;
	}
	~p2dWorld() {
		while (bodyCount > 0) {
			destroyBody(bodyList);
		}
	}
public:
	p2dBody* getBodyList();
	int getBodyCount() const;

	void setGravity(const p2dVec2& g);
	const p2dVec2& getGravity() const;
public:
	p2dBody* createBody(const p2dBodyDef* def);
	void destroyBody(p2dBody* body);

	void step(float timeStep, int velocityIterations = defaultVelocityIterations, 
				int positionIterations = defaultPositionIterations);
//	void rayCast(p2dRayCastCallback* callback, const p2dVec2& point1, const p2dVec2& point2) const;
//	void queryAABB(p2dQueryAABBCallback* callback, const p2dAABB& aabb) const;
public:
	friend class p2dBody;
};

inline p2dBody* p2dWorld::getBodyList() {
	return bodyList;
}
inline int p2dWorld::getBodyCount() const {
	return bodyCount;
}
inline const p2dVec2& p2dWorld::getGravity() const {
	return gravity;
}
inline void p2dWorld::setGravity(const p2dVec2& g) {
	gravity = g;
}


}
#endif
