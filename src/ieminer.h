
#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include <vector>

#include "src/types/event.h"
#include "src/types/relationship.h"
#include "src/types/composite_event_list.h"

class IEMiner {
public:
	CandidateSet getNextCandidateSet(int k, FrequentPatternSet freq_k_patterns);
	void countSupport(int level, CompositeEventList el, CandidateSet candidate);
	void ieMiner();
}

#endif
