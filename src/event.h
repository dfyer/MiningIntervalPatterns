
#ifndef EVENT_H_
#define EVENT_H_

#include <cstdio>
#include <cassert>
#include <stdint.h>
#include <cstring>

#include "temporal_relation.h"

#define NULL_TIME -1
#define NULL_EVENT -1

typedef int64_t event_t;

class Event {
public:
	Event(event_t t, int64_t s, int64_t e) : type_(t), start_(s), end_(e) {}
	Event(const Event& e) : type_(e.type_), start_(e.start_), end_(e.end_) {}
	REL_TYPE getRelationType(Event e);

	event_t type_;
	int64_t start_;
	int64_t end_;

	void print() const;
};

#endif
