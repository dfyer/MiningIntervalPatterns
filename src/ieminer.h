
#ifndef IEMINER_H_
#define IEMINER_H_

#include <vector>
#include <map>

#include "event.h"
#include "temporal_relation.h"
#include "composite_event.h"

class IEMiner {
public:
    cemap_t getNextCandidateSet(const int k_plus_one, const cemap_t fK);
	void countSupport(const int level, const std::vector<Event> el, cemap_t& candidateSet);
	cemap_t ieMiner(const double min_sup_ratio, const std::vector<std::vector<Event> > database);
//private:
    bool containsPrefixIn(const cemap_t& candidateSet, const CompositeEvent newtp);
    cemap_t getFrequentTwoPatterns(const int min_sup, const cemap_t fK);
    cemap_t getStartingFrequentTwoPatterns(const int min_sup, const std::vector<std::vector<Event> > database);
    cemap_t getSingle(const int min_sup, const std::vector<std::vector<Event> > database);
    bool hasDuplicate(const CompositeEvent* tpK, const CompositeEvent* tpTwo);
};

#endif
