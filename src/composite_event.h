
#ifndef COMPOSITE_EVENT_H_
#define COMPOSITE_EVENT_H_

#include <cstdio>
#include <vector>

#include "event.h"
#include "temporal_relation.h"

#define NULL_TIME -1

struct CompositeEvent {
public:
	CompositeEvent() : start_(NULL_TIME), end_(NULL_TIME) {}
	CompositeEvent(Event* e) {
		this->start_ = e->start_;
		this->end_ = e->end_;
	}

	bool append(Event* e);
	CompositeEvent* join(int idx_dominant, CompositeEvent* ce, int idx_first);

	Event* getFirst();
	Event* getDominant();

	int64_t getStart();
	int64_t getEnd();
	int getLength();

	void printAll();

private:
	TemporalRelation* getRelation(Event* e);
	REL_TYPE getRelationType(Event* e);
	void getRelationCount(int32_t* rel_count, Event* e);

	std::vector<Event> event_list_;
	std::vector<TemporalRelation> relation_list_;

	Event* first_;
	Event* dominant_;

	int64_t start_;
	int64_t end_;
};

#endif

