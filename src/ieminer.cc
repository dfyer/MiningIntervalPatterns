
#include "ieminer.h"

cemap_t IEMiner::getNextCandidateSet(const int k_plus_one, const cemap_t fK) {
    cemap_t nextCandidateSet;
    cemap_t fTwo(getFrequentTwoPatterns(k_plus_one - 2, fK));

    for(cemap_t::const_iterator iter_tpK = fK.begin(); iter_tpK != fK.end(); iter_tpK++) {
        for(cemap_t::iterator iter_tpTwo = fTwo.begin(); iter_tpTwo != fTwo.end(); iter_tpTwo++) {
			assert((*iter_tpK).first.getDominant() != NULL_EVENT);
			assert((*iter_tpTwo).first.getFirst() != NULL_EVENT);

            if((*iter_tpK).first.getDominant() == (*iter_tpTwo).first.getFirst()) {
                if(!hasDuplicate(&iter_tpK->first, &iter_tpTwo->first)) {
                    CompositeEvent newCandidate(&(*iter_tpK).first, &(*iter_tpTwo).first);
                    nextCandidateSet[newCandidate] = 0;
                }
            }
        }
    }

    return nextCandidateSet;
}

void IEMiner::countSupport(const int level, const std::vector<Event> el, cemap_t& candidateSet) {
    int64_t currentTime = -1;
    std::vector<CompositeEvent> activeTp;
    std::vector<CompositeEvent> passiveTp;
    for(std::vector<Event>::const_iterator iter_nextEvent = el.begin(); iter_nextEvent != el.end(); iter_nextEvent++) {
        //if(nextEvent = frequent event) - to reduce #events considering??
        std::vector<CompositeEvent> toBeAdded;
        currentTime = iter_nextEvent->start_;
        for(std::vector<CompositeEvent>::iterator iter_atp = activeTp.begin(); iter_atp != activeTp.end(); iter_atp++) {
            if(iter_atp->getEnd() < currentTime) {
                // to passive
                passiveTp.push_back(*iter_atp);
                activeTp.erase(iter_atp);
                iter_atp--;
            } else {
                // check prefix
                CompositeEvent* newtp = new CompositeEvent(*iter_atp);
                newtp->append(*iter_nextEvent);

                if(newtp->getLength() == level && candidateSet.count(*newtp) == 1)
                    ++candidateSet[*newtp];
                else if(newtp->getLength() < level && containsPrefixIn(candidateSet, *newtp))
                    toBeAdded.push_back(*newtp);
            }
        }
        for(std::vector<CompositeEvent>::iterator iter_ptp = passiveTp.begin(); iter_ptp != passiveTp.end(); iter_ptp++) {
            // check prefix
            CompositeEvent* newtp = new CompositeEvent(*iter_ptp);
            newtp->append(*iter_nextEvent);

            if(newtp->getLength() == level && candidateSet.count(*newtp) == 1)
                ++candidateSet[*newtp];
            else if(newtp->getLength() < level && containsPrefixIn(candidateSet, *newtp))
                toBeAdded.push_back(*newtp);
        }
        CompositeEvent* nextce = new CompositeEvent(*iter_nextEvent);
        activeTp.push_back(*nextce);
        for(int i = 0; i < toBeAdded.size(); i++)
            activeTp.push_back(toBeAdded[i]);
        toBeAdded.clear();
    }
}

cemap_t IEMiner::ieMiner(const double min_sup_ratio, const std::vector<std::vector<Event> > database) {
    // Bases (minsup= good with flooring of double to int cast_)
    int min_sup = (int) database.size() * min_sup_ratio;
    cemap_t candidateSet = getStartingFrequentTwoPatterns(min_sup, database);
    int level = 2;
    
    cemap_t result;
    cemap_t single = getSingle(min_sup, database);
    result.insert(single.begin(), single.end());
    //result.insert(candidateSet.begin(), candidateSet.end());

    bool flag = false;
    do {
        for(int i = 0; i < database.size(); i++)
            countSupport(level, database[i], candidateSet);
        std::vector<CompositeEvent> toBeRemoved;

        for(cemap_t::iterator iter_tpK = candidateSet.begin(); iter_tpK != candidateSet.end(); iter_tpK++)
            if((*iter_tpK).second < min_sup)
                toBeRemoved.push_back((*iter_tpK).first);
        for(int i = 0; i < toBeRemoved.size(); i++)
            candidateSet.erase(toBeRemoved[i]);

        result.insert(candidateSet.begin(), candidateSet.end());
        ++level;
        candidateSet = getNextCandidateSet(level, candidateSet);

    } while(candidateSet.size() != 0);

    return result;
}

bool IEMiner::containsPrefixIn(const cemap_t& candidateSet, const CompositeEvent newtp) {
    for(cemap_t::const_iterator iter = candidateSet.begin(); iter != candidateSet.end(); iter++) {
        if(iter->first.containsPrefix(newtp))
            return true;
    }
    return false;
}

cemap_t IEMiner::getFrequentTwoPatterns(const int min_sup, const cemap_t fK) {
    cemap_t fTwo;

    // Get every single patterns. Frequent pattern list (CompositeEvent) keeps order for next step
    for(cemap_t::const_iterator iter_tpK = fK.begin(); iter_tpK != fK.end(); iter_tpK++) {
        // acquire an ordered event list el
        const CompositeEvent& ce = iter_tpK->first;
        for(int i = 0; i < ce.getLength(); i++) {
            for(int j = i + 1; j < ce.getLength(); j++) {
                CompositeEvent tpTwo(ce.event_list_[i]);
                tpTwo.append(ce.event_list_[j]);
                
                ++fTwo[tpTwo];

                assert(fTwo.count(tpTwo) > 0);
            }
        }
    }

    for(cemap_t::iterator iter = fTwo.begin(); iter != fTwo.end(); ++iter) {
        cemap_t::iterator jter = iter;
        ++jter;
        if(jter != fTwo.end())
            assert(iter->first < jter-> first);
    }

    // Delete with sup < min_sup
    std::vector<CompositeEvent> toBeRemoved;
    for(cemap_t::iterator iter_tpK = fTwo.begin(); iter_tpK != fTwo.end(); iter_tpK++) {
        if((*iter_tpK).second < min_sup)
            toBeRemoved.push_back((*iter_tpK).first);
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

cemap_t IEMiner::getStartingFrequentTwoPatterns(const int min_sup, const std::vector<std::vector<Event> > database) {
    cemap_t fTwo;

    // Get every single patterns. Frequent pattern list (CompositeEvent) keeps order for next step
    for(std::vector<std::vector<Event> >::const_iterator iter = database.begin(); iter != database.end(); ++iter) {
        // acquire an ordered event list el
        for(int i = 0; i < (*iter).size(); i++) {
            for(int j = i + 1; j < (*iter).size(); j++) {
                CompositeEvent tpTwo((*iter)[i]);
                tpTwo.append((*iter)[j]);
                
                ++fTwo[tpTwo];

                assert(fTwo.count(tpTwo) > 0);
            }
        }
    }

    for(cemap_t::iterator iter = fTwo.begin(); iter != fTwo.end(); iter++) {
        iter->second = 0;
    }

    /*
    // Delete with sup < min_sup
    std::vector<CompositeEvent> toBeRemoved;
    for(cemap_t::iterator iter_tpK = fTwo.begin(); iter_tpK != fTwo.end(); iter_tpK++) {
        if((*iter_tpK).second < min_sup)
            toBeRemoved.push_back((*iter_tpK).first);
    }
    for(std::vector<CompositeEvent>::iterator iter_toBeRemoved = toBeRemoved.begin(); iter_toBeRemoved != toBeRemoved.end(); iter_toBeRemoved++) {
        fTwo.erase(*iter_toBeRemoved);
    }

    // assert
    for(cemap_t::iterator iter_tpTwo = fTwo.begin(); iter_tpTwo != fTwo.end(); iter_tpTwo++) {
        assert((*iter_tpTwo).second >= min_sup);
    }
    */

    return fTwo;
}

cemap_t IEMiner::getSingle(const int min_sup, const std::vector<std::vector<Event> > database) {
    cemap_t fSingle;

    // Get every single patterns. Frequent pattern list (CompositeEvent) keeps order for next step
    for(std::vector<std::vector<Event> >::const_iterator iter = database.begin(); iter != database.end(); ++iter) {
        // acquire an ordered event list el
        for(int i = 0; i < (*iter).size(); i++) {
            CompositeEvent tpSingle((*iter)[i]);
            
            ++fSingle[tpSingle];

            assert(fSingle.count(tpSingle) > 0);
        }
    }

    // Delete with sup < min_sup
    std::vector<CompositeEvent> toBeRemoved;
    for(cemap_t::iterator iter_tpK = fSingle.begin(); iter_tpK != fSingle.end(); iter_tpK++) {
        if((*iter_tpK).second < min_sup)
            toBeRemoved.push_back((*iter_tpK).first);
    }
    for(std::vector<CompositeEvent>::iterator iter_toBeRemoved = toBeRemoved.begin(); iter_toBeRemoved != toBeRemoved.end(); iter_toBeRemoved++) {
        fSingle.erase(*iter_toBeRemoved);
    }

    // assert
    for(cemap_t::iterator iter_tpSingle = fSingle.begin(); iter_tpSingle != fSingle.end(); iter_tpSingle++) {
        assert((*iter_tpSingle).second >= min_sup);
    }

    return fSingle;
}

bool IEMiner::hasDuplicate(const CompositeEvent* tpK, const CompositeEvent* tpTwo) {
    for(int i = 0; i < tpK->event_list_.size(); i++)
        if(tpK->event_list_[i].type_ == (*tpTwo->event_list_.rbegin()).type_)
            return true;
    return false;
}
