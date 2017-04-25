/*
 * p2dGrowableStack.h
 *
 *  Created on: 2017年1月19日
 *      Author: lanz
 */

#ifndef COMMON_MB2GROWABLESTACK_H_
#define COMMON_MB2GROWABLESTACK_H_

#include "p2dSettings.h"

template<typename T, uint32 N>

class p2dGrowableStack {
private:
	T *m_stack;
	T m_array[N];
	uint32 m_capacity;
	uint32 m_count;
public:
	p2dGrowableStack() : m_stack(m_array), m_capacity(N), m_count(0) {}
	~p2dGrowableStack() {
		if (m_stack != m_array)
			p2dFree(m_stack);
	}

	void Push(const T &e) {
		if (m_count == m_capacity) {
			T *old = m_stack;
			m_capacity <<= 1;
			m_stack = reinterpret_cast<T*>(p2dAlloc(m_capacity * sizeof(T)));
			memcpy(m_stack, old, m_count * sizeof(T));
			if (old != m_array) {
				p2dFree(old);
			}
		}
		m_stack[m_count++] = e;
	}

	T Pop() {
		p2dAssert(m_count > 0);
		return m_stack[--m_count];
	}
	uint32 GetCount() {
		return m_count;
	}
};

#endif /* COMMON_MB2GROWABLESTACK_H_ */
