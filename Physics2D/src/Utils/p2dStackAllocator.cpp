/*
 * p2dStackAllocator.cpp
 *
 *  Created on: 2017-01-19
 *      Author: lanz
 */

#include "p2dStackAllocator.h"

p2dStackAllocator::p2dStackAllocator() : m_index(0), m_entryCount(0) {}

p2dStackAllocator::~p2dStackAllocator() {
	p2dAssert(m_index == 0);
	p2dAssert(m_entryCount == 0);
}

void* p2dStackAllocator::Allocate(uint32 size) {
	p2dAssert(m_entryCount < p2d_maxStackEntries);

	p2dStackEntry &entry = m_entries[m_entryCount];

	if (m_index + size > p2d_stackSize) {
		entry.data = p2dAlloc(size);
		entry.usedMalloc = true;
	} else {
		entry.data = m_data + m_index;
		entry.usedMalloc = false;
		m_index += size;
	}
	++m_entryCount;
	entry.size = size;
	return entry.data;
}

void p2dStackAllocator::Free(void *p) {
	p2dAssert(m_entryCount > 0);
	p2dStackEntry &entry = m_entries[--m_entryCount];
	p2dAssert(p == entry.data);

	if (entry.usedMalloc) {
		p2dFree(p);
	} else {
		p2dAssert(m_index >= entry.size);
		m_index -= entry.size;
	}
}


