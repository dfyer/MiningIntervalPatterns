
#ifndef IEMINER_H_
#define IEMINER_H_

#include <vector>
#include <map>

#include "event.h"
#include "temporal_relation.h"
#include "composite_event.h"

typedef std::map<CompositeEvent, int, CmpCompositeEvent> cemap_t;

class IEMiner {
public:
    cemap_t getNextCandidateSet(int k, cemap_t fK);
	//TODO: void countSupport(int level, CompositeEventList el, CandidateSet candidate);
	void ieMiner(double min_sup_ratio, std::vector<std::vector<Event> > database);
//private:
    cemap_t getFrequentTwoPatterns(int min_sup, cemap_t fK);
    cemap_t getStartingFrequentTwoPatterns(int min_sup, std::vector<std::vector<Event> > database);
};

#endif
