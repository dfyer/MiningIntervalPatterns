
#include "composite_event.h"

/* todo-list
// Publics
CompositeEvent* CompositeEvent::join(CompositeEvent* ce) {
	assert((*first).type_ == (*ce->dominant).type_);
}
Event* CompositeEvent::getFirst();
Event* CompositeEvent::getDominant();
int64_t getStart();
int64_t getEnd();

// Privates
*/

CompositeEvent::CompositeEvent(const CompositeEvent* tpK, const CompositeEvent* tpTwo) {
    assert(tpK->getDominant() == tpTwo->getFirst());

    this->start_ = tpK->start_;
    this->end_ = tpK->end_;
    this->first_ = tpK->first_;
    this->dominant_ = tpK->dominant_;

    std::copy(tpK->event_list_.begin(), tpK->event_list_.end(), std::back_inserter(this->event_list_));
    std::copy(tpK->relation_list_.begin(), tpK->relation_list_.end(), std::back_inserter(this->relation_list_));

    this->append(Event((*tpTwo->event_list_.rbegin()).type_, (*tpTwo->event_list_.rbegin()).start_, (*tpTwo->event_list_.rbegin()).end_));
}

bool CompositeEvent::append(Event e) {
	assert(this->start_ < e.start_);
	
	if(event_list_.size() > 0) {
		relation_list_.push_back(getRelation(e));
        event_list_.push_back(e);
        if(this->end_ < e.end_) {
            this->end_ = e.end_;
            this->dominant_ = e.type_;
        }
	} else {
		this->start_ = e.start_;
		this->end_ = e.end_;

		this->first_= e.type_;
		this->dominant_ = e.type_;

		event_list_.push_back(e);
	}
	assert(event_list_.size() == (relation_list_.size() + 1));

	return true;
}

void CompositeEvent::getFrequentTwoPatterns(cemap_t& fTwo) const {
    if(event_list_.size() > 1) {
        for(int i = 0; i < event_list_.size(); i++) {
            for(int j = i + 1; j < event_list_.size(); j++) {
                CompositeEvent* tpTwo = new CompositeEvent(event_list_[i]);
                //tpTwo->append(Event(event_list_[j].type_, event_list_[j].start_, event_list_[j].end_));
                tpTwo->append(event_list_[j]);
                
                ++fTwo[*tpTwo];
                /*
                if(fTwo.count(*tpTwo))
                    fTwo.find(*tpTwo)->second += 1;
                else
                    fTwo.insert(cemap_t::value_type(*tpTwo, 0));
                */
            }
        }
    }
}

bool CompositeEvent::containsPrefix(const CompositeEvent& tp) const {
    assert(this->getLength() > 0);
    assert(tp.getLength() > 0);
    assert(this->getLength() > tp.getLength());

    if(this->event_list_[0].type_ != tp.event_list_[0].type_)
        return false;
    for(int i = 1; i < tp.getLength(); i++) {
        if(!this->relation_list_[i - 1].isEqualTo(tp.relation_list_[i - 1]))
            return false;
        if(this->event_list_[0].type_ != tp.event_list_[0].type_)
            return false;
    }
    return true;
}

void CompositeEvent::printAll() const {
	assert(event_list_.size() == (relation_list_.size() + 1));

	for(int i = 0; i < relation_list_.size(); i++) {
		printf("( ");
	}

	if(event_list_.size() > 0) {
        if((*event_list_.begin()).type_ == first_)
            printf("f*");
        if((*event_list_.begin()).type_ == dominant_)
            printf("d*");
		(*event_list_.begin()).print();
		for(int i = 0; i < relation_list_.size(); i++) {
			relation_list_[i].print();

            if(event_list_[i+1].type_ == first_)
                printf("f*");
            else if(event_list_[i+1].type_ == dominant_)
                printf("d*");
			event_list_[i+1].print();

			printf(") ");
		}
	}

	printf("\n");
}

TemporalRelation CompositeEvent::getRelation(Event e) {
	assert(this->start_ < e.start_);

	REL_TYPE rel_type = getRelationType(e);
	int32_t rel_count[5] = { 0, 0, 0, 0, 0 };
	getRelationCount(rel_count, e);

	return TemporalRelation(rel_type, rel_count);
}

REL_TYPE CompositeEvent::getRelationType(Event e) {
	assert(this->start_ < e.start_);

	// Decide which basic relation this and e has
	if(this->end_ < e.start_) {
		return  TYPE_BEFORE;
	} else if(this->end_ == e.start_) {
		return  TYPE_MEET;
	} else if(this->end_ > e.start_ && this->end_ < e.end_ && this->start_ < e.start_) {
		return  TYPE_OVERLAP;
	} else if(this->start_ == e.start_ && this->end_ < e.end_) {
		return  TYPE_START;
	} else if(this->start_ < e.start_ && this->end_ == e.end_) {
		return  TYPE_FINISHED_BY;
	} else if(this->start_ < e.start_ && this->end_ > e.end_) {
		return  TYPE_CONTAIN;
	} else if(this->start_ == e.start_ && this->end_ == e.end_) {
		return  TYPE_EQUAL;
	}

	// Relation is undefined
	assert(0);
	return TYPE_NULL;
}

void CompositeEvent::getRelationCount(int32_t* rel_count, Event e) {
	assert(this->start_ < e.start_);
	
	int32_t& c = rel_count[COUNT_C];
	int32_t& f = rel_count[COUNT_F];
	int32_t& m = rel_count[COUNT_M];
	int32_t& o = rel_count[COUNT_O];
	int32_t& s = rel_count[COUNT_S];

	// Count each item's relation with e
	for(std::vector<Event>::iterator iter =  event_list_.begin(); iter != event_list_.end(); iter++) {
		switch((*iter).getRelationType(e)) {
			case TYPE_CONTAIN:
				c++; break;
			case TYPE_FINISHED_BY:
				f++; break;
			case TYPE_MEET:
				m++; break;
			case TYPE_OVERLAP:
				o++; break;
			case TYPE_START:
				s++; break;
			case TYPE_EQUAL:
				f++; s++; break;
			case TYPE_BEFORE:
			default:
				break;
		}
	}
}
