/*
 * p2dBlockAllocator.h
 *
 *  Created on: 2017-01-18
 *      Author: lanz
 */

#ifndef MB2BLOCKALLOCATOR_H_
#define MB2BLOCKALLOCATOR_H_

#include "p2dSettings.h"

const uint32 p2d_chunkSize = 16 * 1024;
const uint32 p2d_maxBlockSize = 640;
const uint32 p2d_blockSizes = 14;
const uint32 p2d_chunkArrayIncrement = 128;

struct p2dBlock;
struct p2dChunk;

class p2dBlockAllocator {
public:
	p2dBlockAllocator();
	virtual ~p2dBlockAllocator();

	void* Allocate(size_type size);
	void Free(void *p, size_type size);
	void Clear();
private:
	static uint32 s_blockSizes[p2d_blockSizes];
	static uint8 s_blockSizeLookup[p2d_maxBlockSize + 1];
	static bool s_blockSizeLookupInitialized;

	p2dChunk *m_chunks = nullptr;

	uint32 m_chunkSpace;
	uint32 m_chunkCount;

	p2dBlock *m_freeList[p2d_blockSizes];
};

#endif /* MB2BLOCKALLOCATOR_H_ */
