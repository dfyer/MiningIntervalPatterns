
#include "ieminer.h"

cemap_t IEMiner::getNextCandidateSet(int k, cemap_t fK) {
    cemap_t fNext;
    cemap_t fTwo(getFrequentTwoPatterns(k - 1, fK));

    for(cemap_t::iterator iter_tpK = fK.begin(); iter_tpK != fK.end(); iter_tpK++) {
        for(cemap_t::iterator iter_tpTwo = fTwo.begin(); iter_tpTwo != fTwo.end(); iter_tpTwo) {
			assert((*iter_tpK).first.getDominant() != NULL_EVENT);
			assert((*iter_tpTwo).first.getFirst() != NULL_EVENT);
            if((*iter_tpK).first.getDominant() == (*iter_tpTwo).first.getFirst()) {
                CompositeEvent* newCandidate = new CompositeEvent(&(*iter_tpK).first, &(*iter_tpTwo).first);
                fNext[*newCandidate] = 0;
            }
        }
    }

    return fNext;
}

void IEMiner::ieMiner(double min_sup_ratio, std::vector<std::vector<Event> > database) {
    int min_sup = (int) database.size() * min_sup_ratio;
    cemap_t fK = getStartingFrequentTwoPatterns(min_sup, database);
}

cemap_t IEMiner::getFrequentTwoPatterns(int min_sup, cemap_t fK) {
    cemap_t fTwo;
    for(cemap_t::iterator iter_tpK = fK.begin(); iter_tpK != fK.end(); iter_tpK++) {
        (*iter_tpK).first.getFrequentTwoPatterns(fTwo);
    }

    // Delete with sup < min_sup
    std::vector<CompositeEvent> toBeRemoved;
    for(cemap_t::iterator iter_tpTwo = fTwo.begin(); iter_tpTwo != fTwo.end(); iter_tpTwo++) {
        if((*iter_tpTwo).second < min_sup)
            toBeRemoved.push_back((*iter_tpTwo).first);
    }
    for(std::vector<CompositeEvent>::iterator iter_toBeRemoved = toBeRemoved.begin(); iter_toBeRemoved != toBeRemoved.end(); iter_toBeRemoved++) {
        fTwo.erase(*iter_toBeRemoved);
    }

    // assert
    for(cemap_t::iterator iter_tpTwo = fTwo.begin(); iter_tpTwo != fTwo.end(); iter_tpTwo++) {
        assert((*iter_tpTwo).second >= min_sup);
    }

    return fTwo;
}

cemap_t IEMiner::getStartingFrequentTwoPatterns(int min_sup, std::vector<std::vector<Event> > database) {
    cemap_t fK;

    for(std::vector<std::vector<Event> >::iterator iter = database.begin(); iter != database.end(); iter++) {
        if((*iter).size() > 1) {
            for(int i = 0; i < (*iter).size(); i++) {
                for(int j = i + 1; j < (*iter).size(); j++) {
                    CompositeEvent* tpTwo = new CompositeEvent(&(*iter)[i]);
                    tpTwo->append((*iter)[j]);

                    if(fK.count(*tpTwo))
                        fK.find(*tpTwo)->second += 1;
                    else
                        fK.insert(cemap_t::value_type(*tpTwo, 0));
                }
            }
        }
    }

    // Delete with sup < min_sup
    std::vector<CompositeEvent> toBeRemoved;
    for(cemap_t::iterator iter_tpK = fK.begin(); iter_tpK != fK.end(); iter_tpK++) {
        if((*iter_tpK).second < min_sup)
            toBeRemoved.push_back((*iter_tpK).first);
    }
    for(std::vector<CompositeEvent>::iterator iter_toBeRemoved = toBeRemoved.begin(); iter_toBeRemoved != toBeRemoved.end(); iter_toBeRemoved++) {
        fK.erase(*iter_toBeRemoved);
    }

    return fK;
}
