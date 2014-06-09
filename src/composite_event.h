
#ifndef COMPOSITE_EVENT_H_
#define COMPOSITE_EVENT_H_

#include <cstdio>
#include <vector>
#include <map>

#include "event.h"
#include "temporal_relation.h"

struct CmpCompositeEvent;

class CompositeEvent {
public:
	CompositeEvent() : start_(NULL_TIME), end_(NULL_TIME), first_(NULL_EVENT), dominant_(NULL_EVENT) {}
	CompositeEvent(Event* e) {
		this->start_ = e->start_;
		this->end_ = e->end_;

        this->event_list_.push_back(*e);
	}
    CompositeEvent(const CompositeEvent& ce) {
        this->start_ = ce.start_;
        this->end_ = ce.end_;
        this->first_ = ce.first_;
        this->dominant_ = ce.dominant_;

        std::copy(ce.event_list_.begin(), ce.event_list_.end(), this->event_list_.begin());
        std::copy(ce.relation_list_.begin(), ce.relation_list_.end(), this->relation_list_.begin());
    }
    // "join" constructor TODO
	CompositeEvent(CompositeEvent* tpK, CompositeEvent* tpTwo);

	bool append(Event e);
    void getFrequentTwoPatterns(std::map<CompositeEvent, int, CmpCompositeEvent>& fTwo);
    bool contains(CompositeEvent* ce);

	int64_t getFirst() const { return first_; }
	int64_t getDominant() const { return dominant_; }

	int64_t getStart() const { return start_; }
	int64_t getEnd() const { return end_; }
	int getLength() const { return event_list_.size(); }

	void printAll() const;

	std::vector<Event> event_list_;
	std::vector<TemporalRelation> relation_list_;

private:
	TemporalRelation getRelation(Event e);
	REL_TYPE getRelationType(Event e);
	void getRelationCount(int32_t* rel_count, Event e);

	event_t first_;
    int index_of_first_;
	event_t dominant_;
    int index_of_dominant_;

	int64_t start_;
	int64_t end_;
};

struct CmpCompositeEvent {
    bool operator()(const CompositeEvent& a,const CompositeEvent& b) const {
        if(a.getLength() != b.getLength())
            return false;

        if(a.event_list_[0].type_ != b.event_list_[0].type_)
            return false;
        for(int i = 1; i < a.getLength(); i++) {
            if(a.event_list_[i].type_ != b.event_list_[i].type_)
                return false;
            if(!a.relation_list_[i - 1].isEqualTo(b.relation_list_[i - 1]))
                return false;
        }
        return true;
    }
};

#endif

