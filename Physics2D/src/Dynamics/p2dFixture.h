#ifndef _P2D_FIXTURE_H_
#define _P2D_FIXTURE_H_

class p2dBlockAllocator;

namespace Physics2D {

class p2dBody;
class p2dShape;

struct p2dFixtureDef {
	const p2dShape* shape {nullptr};
	float friction {0.0f};
	float density {0.0f};
};

class p2dFixture {
private:
	p2dBody* body;

	p2dShape* shape {nullptr};
	float friction {0.0f};
	float density {0.0f};
public:
	p2dFixture* next;
public:
	friend class p2dBody;
public:
	p2dFixture() {
	}
	~p2dFixture() {
	}

	const p2dShape* getShape() const {
		return shape;
	}

	void create(p2dBlockAllocator* allocator, p2dBody* body, const p2dFixtureDef* def);
	void destroy(p2dBlockAllocator* allocator);
};

}
#endif
