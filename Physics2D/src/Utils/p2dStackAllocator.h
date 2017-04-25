/*
 * p2dStackAllocator.h
 *
 *  Created on: 2017-01-19
 *      Author: lanz
 */

#ifndef COMMON_MB2STACKALLOCATOR_H_
#define COMMON_MB2STACKALLOCATOR_H_

#include "p2dSettings.h"

const int32 p2d_stackSize = 100 * 1024;		//100k
const int32 p2d_maxStackEntries = 32;


struct p2dStackEntry {
	void *data;
	int32 size;
	bool usedMalloc;
};

class p2dStackAllocator {
public:
	p2dStackAllocator();
	virtual ~p2dStackAllocator();

	void* Allocate(uint32 size);
	void Free(void *p);
	int32 GetMaxAllocation() const;

private:
	int8 m_data[p2d_stackSize];
	int32 m_index = 0;
	p2dStackEntry m_entries[p2d_maxStackEntries];
	int32 m_entryCount = 0;
};

#endif /* COMMON_MB2STACKALLOCATOR_H_ */
