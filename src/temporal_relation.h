
#ifndef RELATION_H_
#define RELATION_H_

#include <stdint.h>
#include <cstdio>

#define NUM_REL_TYPE 7
#define NUM_REL_COUNT 5

enum REL_TYPE { TYPE_BEFORE, TYPE_MEET, TYPE_OVERLAP, TYPE_START, TYPE_FINISHED_BY, TYPE_CONTAIN, TYPE_EQUAL, TYPE_NULL, };
enum REL_COUNT { COUNT_C, COUNT_F, COUNT_M, COUNT_O, COUNT_S, };

class TemporalRelation {
public:
	// set of counts
	// each means as below
	//   rel_count[0] : 'c'ontain
	//   rel_count[1] : 'f'inished by
	//   rel_count[2] : 'm'eet
	//   rel_count[3] : 'o'verlap
	//   rel_count[4] : 's'tart

	TemporalRelation(REL_TYPE rel_type, int c, int f, int m, int o, int s) : rel_type_(rel_type) {
		rel_count_[COUNT_C] = c;
		rel_count_[COUNT_F] = f;
		rel_count_[COUNT_M] = m;
		rel_count_[COUNT_O] = o;
		rel_count_[COUNT_S] = s;
	}
	TemporalRelation(REL_TYPE rel_type, int32_t* rel_count) : rel_type_(rel_type) {
		rel_count_[COUNT_C] = rel_count[COUNT_C];
		rel_count_[COUNT_F] = rel_count[COUNT_F];
		rel_count_[COUNT_M] = rel_count[COUNT_M];
		rel_count_[COUNT_O] = rel_count[COUNT_O];
		rel_count_[COUNT_S] = rel_count[COUNT_S];
	}
	TemporalRelation(const TemporalRelation& tr) : rel_type_(tr.rel_type_) {
		rel_count_[COUNT_C] = tr.rel_count_[COUNT_C];
		rel_count_[COUNT_F] = tr.rel_count_[COUNT_F];
		rel_count_[COUNT_M] = tr.rel_count_[COUNT_M];
		rel_count_[COUNT_O] = tr.rel_count_[COUNT_O];
		rel_count_[COUNT_S] = tr.rel_count_[COUNT_S];
	}

	bool isEqualTo(const TemporalRelation& cmp) const {
		if(this->rel_type_ != cmp.rel_type_)
			return false;
		if(this->rel_count_[COUNT_C] != cmp.rel_count_[COUNT_C])
			return false;
		if(this->rel_count_[COUNT_F] != cmp.rel_count_[COUNT_F])
			return false;
		if(this->rel_count_[COUNT_M] != cmp.rel_count_[COUNT_M])
			return false;
		if(this->rel_count_[COUNT_O] != cmp.rel_count_[COUNT_O])
			return false;
		if(this->rel_count_[COUNT_S] != cmp.rel_count_[COUNT_S])
			return false;
		return true;
	}

	bool isLessThan(const TemporalRelation& cmp) const {
        const REL_TYPE& rt = this->rel_type_;
        const int32_t& rc = this->rel_count_[COUNT_C];
        const int32_t& rf = this->rel_count_[COUNT_F];
        const int32_t& rm = this->rel_count_[COUNT_M];
        const int32_t& ro = this->rel_count_[COUNT_O];
        const int32_t& rs = this->rel_count_[COUNT_S];

        const REL_TYPE& ct = cmp.rel_type_;
        const int32_t& cc = cmp.rel_count_[COUNT_C];
        const int32_t& cf = cmp.rel_count_[COUNT_F];
        const int32_t& cm = cmp.rel_count_[COUNT_M];
        const int32_t& co = cmp.rel_count_[COUNT_O];
        const int32_t& cs = cmp.rel_count_[COUNT_S];

        if(rt < ct) {
            return true;
        } else if(rt == ct && rc < cc) {
            return true;
        } else if(rt == ct && rc == cc && rf < cf) {
            return true;
        } else if(rt == ct && rc == cc && rf == cf && rm < cm) {
            return true;
        } else if(rt == ct && rc == cc && rf == cf && rm == cm && ro < co) {
            return true;
        } else if(rt == ct && rc == cc && rf == cf && rm == cm && ro == co && rs < cs) {
            return true;
        } else {
            return false;
        }
	}

	REL_TYPE rel_type_;
	int32_t rel_count_[5];

	void print() const {
		switch(rel_type_) {
			case TYPE_BEFORE:
				printf("Before"); break;
			case TYPE_MEET:
				printf("Meet"); break;
			case TYPE_OVERLAP:
				printf("Overlap"); break;
			case TYPE_START:
				printf("Start"); break;
			case TYPE_FINISHED_BY:
				printf("Finished-By"); break;
			case TYPE_CONTAIN:
				printf("Contain"); break;
			case TYPE_EQUAL:
				printf("Equal"); break;
			default:
				break;
		}
		printf("[%d,%d,%d,%d,%d] ", rel_count_[COUNT_C], rel_count_[COUNT_F], rel_count_[COUNT_M], rel_count_[COUNT_O], rel_count_[COUNT_S]);
	}
};

#endif

