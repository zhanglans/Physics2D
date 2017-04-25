/*
 * p2dBlockAllocator.cpp
 *
 *  Created on: 2017-01-18
 *      Author: lanz
 */

#include "p2dBlockAllocator.h"
#include "p2dSettings.h"

#include <limits.h>
#include <string.h>
#include <stddef.h>

uint32 p2dBlockAllocator::s_blockSizes[p2d_blockSizes] =
{
	16,		// 0
	32,		// 1
	64,		// 2
	96,		// 3
	128,	// 4
	160,	// 5
	192,	// 6
	224,	// 7
	256,	// 8
	320,	// 9
	384,	// 10
	448,	// 11
	512,	// 12
	640,	// 13
};
uint8 p2dBlockAllocator::s_blockSizeLookup[p2d_maxBlockSize + 1];
bool p2dBlockAllocator::s_blockSizeLookupInitialized;

struct p2dChunk {
	int32 blockSize;
	p2dBlock *blocks;
};

struct p2dBlock {
	p2dBlock *next;
};


p2dBlockAllocator::p2dBlockAllocator()
	: m_chunkSpace(p2d_chunkArrayIncrement),
	  m_chunkCount(0)
{
	m_chunks = reinterpret_cast<p2dChunk*>(p2dAlloc(m_chunkSpace * sizeof(p2dChunk)));
	memset(m_chunks, 0, m_chunkSpace * sizeof(p2dChunk));
	memset(m_freeList, 0, p2d_blockSizes * sizeof(p2dBlock*));

	if (!s_blockSizeLookupInitialized) {
		int j = 0;
		for (uint32 i = 1; i < p2d_maxBlockSize; ++i) {
			s_blockSizeLookup[i] = (uint8)(i <= s_blockSizes[j]) ? j : ++j;
		}
		s_blockSizeLookupInitialized = true;
	}
}

p2dBlockAllocator::~p2dBlockAllocator() {
	for (uint32 i = 0; i < m_chunkCount; ++i) {
		p2dFree(m_chunks[i].blocks);
	}
	p2dFree(m_chunks);
}

void* p2dBlockAllocator::Allocate(size_type size) {
	if (size == 0)
		return nullptr;

	if (size > p2d_maxBlockSize) {
		return p2dAlloc(size);
	}
	uint32 index = s_blockSizeLookup[size];
	p2dAssert(0u <= index && index < p2d_blockSizes);

	if (m_freeList[index]) {
		p2dBlock *block = m_freeList[index];
		m_freeList[index] = block->next;
		return block;
	} else {
		if (m_chunkCount == m_chunkSpace) {
			p2dChunk *oldChunks = m_chunks;
			m_chunks = reinterpret_cast<p2dChunk*>(p2dAlloc((m_chunkSpace + p2d_chunkArrayIncrement) * sizeof(p2dChunk)));
			memcpy(m_chunks, oldChunks, m_chunkSpace * sizeof(p2dChunk));
			m_chunkSpace += p2d_chunkArrayIncrement;
			p2dFree(oldChunks);
		}
		p2dChunk *chunk = m_chunks + m_chunkCount;
		chunk->blocks = reinterpret_cast<p2dBlock*>(p2dAlloc(p2d_chunkSize));
		auto blockSize = s_blockSizes[index];
		int blockCount = p2d_chunkSize / blockSize;
		p2dBlock *p = chunk->blocks;
		for (int i = 0; i < blockCount-2; ++i) {
			p2dAssert(blockSize * (i+1) < p2d_chunkSize);
			p->next = reinterpret_cast<p2dBlock*>((int8*)chunk->blocks + blockSize * (i+1));
			p = p->next;
		}
		p->next = nullptr;
		chunk->blockSize = blockSize;

		++m_chunkCount;
		m_freeList[index] = chunk->blocks->next;

		return chunk->blocks;
	}
}

void p2dBlockAllocator::Free(void *p, size_type size) {
	if (0 == size)
		return;
	if (size > p2d_maxBlockSize) {
		p2dFree(p);
		return;
	}

	int index = s_blockSizeLookup[size];
//	memset(p, 0, s_blockSizes[index]); //not necessary
	p2dBlock *block = reinterpret_cast<p2dBlock*>(p);
	block->next = m_freeList[index];
	m_freeList[index] = block;
}

void p2dBlockAllocator::Clear() {
	for (uint32 i = 0; i < m_chunkCount; ++i) {
		p2dFree(m_chunks[i].blocks);
	}
	m_chunkCount = 0;
	memset(m_chunks, 0, m_chunkSpace * sizeof(p2dChunk));
	memset(m_freeList, 0, p2d_blockSizes * sizeof(p2dBlock*));
}

