
#ifndef EVENT_H_
#define EVENT_H_

#include <cstdio>
#include <cassert>
#include <stdint.h>
#include <cstring>

#include "temporal_relation.h"

class Event {
public:
	Event(int64_t t, int64_t s, int64_t e) : type_(t), start_(s), end_(e) {}
	REL_TYPE getRelationType(Event* e);

	int64_t type_;
	int64_t start_;
	int64_t end_;

	void print();
};

#endif
