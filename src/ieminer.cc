
#include "ieminer.h"

std::map<CompositeEvent, int, CmpCompositeEvent> IEMiner::getNextCandidateSet(int k, std::map<CompositeEvent, int, CmpCompositeEvent> fK) {
    std::map<CompositeEvent, int, CmpCompositeEvent> fNext;
    std::map<CompositeEvent, int, CmpCompositeEvent> fTwo(getFrequentTwoPatterns(k - 1, fK));

    for(std::map<CompositeEvent, int, CmpCompositeEvent>::iterator iter_tpK = fK.begin(); iter_tpK != fK.end(); iter_tpK++) {
        for(std::map<CompositeEvent, int, CmpCompositeEvent>::iter iter_tpTwo = fTwo.begin(); iter_tpTwo != fTwo.end(); iter_tpTwo) {
			assert(iter_tpK.first.dominant_ != NULL_EVENT);
			assert(iter_tpTwo.first.first_ != NULL_EVENT);
            if(iter_tpK.first.dominant_ == iter_tpTwo.first.first_) {
                CompositeEvent newCandidate = new CompositeEvent(&iter_tpK.first, &iter_tpTwo.first);
                fNext[newCandidate] = 0;
            }
        }
    }

    return fNext;
}

std::map<CompositeEvent, int, CmpCompositeEvent> IEMiner::getFrequentTwoPatterns(int min_sup, std::map<CompositeEvent, int, CmpCompositeEvent> fK) {
    std::map<CompositeEvent, int, CmpCompositeEvent> fTwo;
    for(std::map<CompositeEvent, int, CmpCompositeEvent>::iterator iter_tpK = fK.begin(); iter_tpK != fK.end(); iter_tpK++) {
        iter_tpK.first.getFrequentTwoPatterns(fTwo);
    }

    return fTwo;
}
