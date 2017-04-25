#include "p2dFixture.h"

#include "p2dBlockAllocator.h"
#include "p2dPolygon.h"

namespace Physics2D {

void p2dFixture::create(p2dBlockAllocator* allocator, p2dBody* b, const p2dFixtureDef* def) {

	next = nullptr;
	body = b;
	friction = def->friction;
	density = def->density;
	shape = def->shape->clone(allocator);
}


void p2dFixture::destroy(p2dBlockAllocator* allocator) {
	if (shape) {
		switch (shape->getType()) {
		case p2dShape::e_polygon:
			p2dPolygon* polygon = reinterpret_cast<p2dPolygon*>(shape);
			polygon->~p2dPolygon();
			allocator->Free(polygon, sizeof(p2dPolygon));
			break;
		}
	}
}

}
