
#ifndef COMPOSITE_EVENT_H_
#define COMPOSITE_EVENT_H_

#include <cstdio>
#include <vector>
#include <map>

#include "event.h"
#include "temporal_relation.h"


class CompositeEvent;

typedef std::map<CompositeEvent, int> cemap_t;

class CompositeEvent {
public:
	CompositeEvent() : start_(NULL_TIME), end_(NULL_TIME), first_(NULL_EVENT), dominant_(NULL_EVENT) {}
	CompositeEvent(const Event& e) {
		this->start_ = e.start_;
		this->end_ = e.end_;

        this->first_ = e.type_;
        this->dominant_ = e.type_;

        this->event_list_.push_back(e);
	}
	CompositeEvent(const Event* e) {
		this->start_ = e->start_;
		this->end_ = e->end_;

        this->first_ = e->type_;
        this->dominant_ = e->type_;

        this->event_list_.push_back(*e);
	}
    CompositeEvent(const CompositeEvent& ce) {
        this->start_ = ce.start_;
        this->end_ = ce.end_;

        this->first_ = ce.first_;
        this->dominant_ = ce.dominant_;

        if(ce.event_list_.size() > 0 )
            std::copy(ce.event_list_.begin(), ce.event_list_.end(), std::back_inserter(this->event_list_));
        if(ce.relation_list_.size() > 0)
            std::copy(ce.relation_list_.begin(), ce.relation_list_.end(), std::back_inserter(this->relation_list_));
    }
    // "join" constructor TODO
	CompositeEvent(const CompositeEvent* tpK, const CompositeEvent* tpTwo);

	bool append(Event e);
    void getFrequentTwoPatterns(cemap_t& fTwo) const;
    bool containsPrefix(const CompositeEvent& tp) const;
	TemporalRelation getRelation(Event e);

	int64_t getFirst() const { return first_; }
	int64_t getDominant() const { return dominant_; }

	int64_t getStart() const { return start_; }
	int64_t getEnd() const { return end_; }
	int getLength() const { return event_list_.size(); }

	void printAll() const;

    bool operator<(const CompositeEvent& b) const {
        // true when a < b
        assert(this->getLength() > 0);
        assert(b.getLength() > 0);

        if(this->getLength() < b.getLength())
            return true;
        if(this->getLength() > b.getLength())
            return false;

        if(this->event_list_[0].type_ < b.event_list_[0].type_)
            return true;
        if(this->event_list_[0].type_ > b.event_list_[0].type_)
            return false;

        for(int i = 1; i < this->getLength(); i++) {
            if(this->event_list_[i].type_ < b.event_list_[i].type_)
                return true;
            if(this->event_list_[i].type_ > b.event_list_[i].type_)
                return false;
            if(this->relation_list_[i - 1].isLessThan(b.relation_list_[i - 1]))
                return true;
            if(b.relation_list_[i - 1].isLessThan(this->relation_list_[i - 1]))
                return false;
        }
        return false;
    }

	std::vector<Event> event_list_;
	std::vector<TemporalRelation> relation_list_;

private:
	REL_TYPE getRelationType(Event e);
	void getRelationCount(int32_t* rel_count, Event e);

	event_t first_;
	event_t dominant_;

	int64_t start_;
	int64_t end_;
};

#endif

