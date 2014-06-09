
#ifndef IEMINER_H_
#define IEMINER_H_

#include <vector>
#include <map>

#include "event.h"
#include "relationship.h"
#include "composite_event_list.h"

class IEMiner {
public:
    std::map<CompositeEvent, int, CmpCompositeEvent> getNextCandidateSet(int k, std::map<CompositeEvent, int, CmpCompositeEvent> fK);
	void countSupport(int level, CompositeEventList el, CandidateSet candidate);
	void ieMiner();
private:
    std::map<CompositeEvent, int, CmpCompositeEvent> getFrequentTwoPatterns(int min_sup, std::map<CompositeEvent, int, CmpCompositeEvent> fK);
};

#endif
