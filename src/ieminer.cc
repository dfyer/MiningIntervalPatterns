
#include "ieminer.h"

std::map<CompositeEvent, int, CmpCompositeEvent> IEMiner::getNextCandidateSet(int k, std::map<CompositeEvent, int, CmpCompositeEvent> fK) {
    std::map<CompositeEvent, int, CmpCompositeEvent> fNext;
    std::map<CompositeEvent, int, CmpCompositeEvent> fTwo(getFrequentTwoPatterns(k - 1, fK));

    for(std::map<CompositeEvent, int, CmpCompositeEvent>::iterator iter_tpK = fK.begin(); iter_tpK != fK.end(); iter_tpK++) {
        for(std::map<CompositeEvent, int, CmpCompositeEvent>::iter iter_tpTwo = fTwo.begin(); iter_tpTwo != fTwo.end(); iter_tpTwo) {
            if(iter_tpK.first.dominant == iter_tpTwo.first.first) {
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
