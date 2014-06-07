
#include "event.h"

REL_TYPE Event::getRelationType(Event* e) {
	assert(this->start_ <= e->start_);

	// Decide which basic relation this and e has
	if(this->end_ < e->start_) {
		return  TYPE_BEFORE;
	} else if(this->end_ == e->start_) {
		return  TYPE_MEET;
	} else if(this->end_ > e->start_ && this->end_ < e->end_ && this->start_ < e->start_) {
		return  TYPE_OVERLAP;
	} else if(this->start_ == e->start_ && this->end_ < e->end_) {
		return  TYPE_START;
	} else if(this->start_ < e->start_ && this->end_ == e->end_) {
		return  TYPE_FINISHED_BY;
	} else if(this->start_ < e->start_ && this->end_ > e->end_) {
		return  TYPE_CONTAIN;
	} else if(this->start_ == e->start_ && this->end_ == e->end_) {
		return  TYPE_EQUAL;
	}

	// Relation is undefined
	assert(0);
	return TYPE_NULL;
}

void Event::print() {
	printf("%lld ", type_);
}
