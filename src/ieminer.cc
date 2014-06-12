
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
    cemap_t fSingle;

    // Get every single patterns. Frequent pattern list (CompositeEvent) keeps order for next step
    for(std::vector<std::vector<Event> >::iterator iter = database.begin(); iter != database.end(); iter++) {
        // acquire an ordered event list el
        std::vector<Event>& el = (*iter);
        for(int i = 0; i < el.size(); i++) {
            for(int j = i + 1; j < el.size(); j++) {
                CompositeEvent* tpSingle = new CompositeEvent(el[i]);
                tpSingle->append(el[j]);
                if(fSingle.count(*tpSingle) == 0)
                    fSingle[*tpSingle] = 1;
                else
                    fSingle[*tpSingle] += 1;
            }
        }
    }

    // Get every two patterns.
    /*
    cemap_t fTwo;
    for(cemap_t::iterator iter = fSingle.begin(); iter != fSingle.end(); ++iter) {
        cemap_t::iterator jter = iter;
        jter ++;
        for(; jter != fSingle.end(); ++jter) {
            CompositeEvent* tpTwo = new CompositeEvent(iter->first);
            tpTwo->append(jter->first.event_list_[0]);
            if(fTwo.count(*tpTwo) == 0)
                fTwo[*tpTwo] = 1;
            else
                fTwo[*tpTwo] += 1;
        }
    }
    */

    // Delete with sup < min_sup
    /*
    std::vector<CompositeEvent> toBeRemoved;
    for(cemap_t::iterator iter_tpK = fTwo.begin(); iter_tpK != fTwo.end(); iter_tpK++) {
        if((*iter_tpK).second < min_sup)
            toBeRemoved.push_back((*iter_tpK).first);
    }
    for(std::vector<CompositeEvent>::iterator iter_toBeRemoved = toBeRemoved.begin(); iter_toBeRemoved != toBeRemoved.end(); iter_toBeRemoved++) {
        fTwo.erase(*iter_toBeRemoved);
    }
    */

    return fSingle;
}
