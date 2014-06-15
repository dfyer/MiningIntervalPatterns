
#include <vector>

#include "event.h"
#include "temporal_relation.h"
#include "composite_event.h"
#include "ieminer.h"

int main() {
	// Set-up test data set
	std::vector<std::vector<Event> > database;

	std::vector<Event> el1;
	el1.push_back(Event(100, 0, 10));
	el1.push_back(Event(101, 5, 16));
	el1.push_back(Event(102, 8, 30));
	el1.push_back(Event(103, 21, 26));
	database.push_back(el1);

	std::vector<Event> el2;
	el2.push_back(Event(100, 0, 10));
	el2.push_back(Event(104, 34, 40));
	el2.push_back(Event(105, 45, 55));
	database.push_back(el2);

	std::vector<Event> el3;
	el3.push_back(Event(100, 0, 10));
	el3.push_back(Event(101, 5, 16));
	el3.push_back(Event(102, 8, 30));
	el3.push_back(Event(103, 20, 25));
	el3.push_back(Event(104, 34, 40));
	database.push_back(el3);

	std::vector<Event> el4;
	el4.push_back(Event(100, 1, 11));
	el4.push_back(Event(101, 6, 17));
	el4.push_back(Event(103, 21, 26));
	el4.push_back(Event(104, 35, 41));
	el4.push_back(Event(105, 46, 56));
	database.push_back(el4);

	// CompositeEvent append test
	std::vector<CompositeEvent*> ce_list;
	for(std::vector<std::vector<Event> >::iterator iter = database.begin(); iter != database.end(); iter++) {
		printf("creating a composite event..\n");
		CompositeEvent* ce = new CompositeEvent();
		for(std::vector<Event>::iterator jter = (*iter).begin(); jter != (*iter).end(); jter++) {
			printf("appending an event..\n");
			ce->append(*jter);
			ce->printAll();
		}
		printf("One composite event created!\n");
		ce_list.push_back(ce);
	}

    printf("\nCheck CELIST!\n");
	for(std::vector<CompositeEvent*>::iterator iter = ce_list.begin(); iter != ce_list.end(); iter++) {
        (*iter)->printAll();
    }

    // IEMiner
    double min_sup_ratio = 0.7;
    IEMiner* iem = new IEMiner();
    cemap_t ieminer_result = iem->ieMiner(min_sup_ratio, database);
    for(cemap_t::iterator iter = ieminer_result.begin(); iter != ieminer_result.end(); iter++) {
        printf("Result of \"IEMiner\" >> Count %d with ce ", iter->second);
        iter->first.printAll();
    }

    /*
	// Generate TP
    //IEMiner* iem = new IEMiner();
    cemap_t fK = iem->getStartingFrequentTwoPatterns(2, database);
    for(cemap_t::iterator iter = fK.begin(); iter != fK.end(); iter++) {
        printf("fStartingTwo >> Count %d with ce ", iter->second);
        iter->first.printAll();
    }

    cemap_t fTwo = iem->getFrequentTwoPatterns(1, fK);
    for(cemap_t::iterator iter = fTwo.begin(); iter != fTwo.end(); iter++) {
        printf("fTwo >> Count %d with ce ", iter->second);
        iter->first.printAll();
    }

    // 2
    cemap_t nextCandidateSet = iem->getNextCandidateSet(3, fK);
    for(cemap_t::iterator iter = nextCandidateSet.begin(); iter != nextCandidateSet.end(); iter++) {
        printf("nextCandidateSet >> Count %d with ce ", iter->second);
        iter->first.printAll();
    }

    for(int i = 0; i < database.size(); i++)
        iem->countSupport(3, database[i], nextCandidateSet);
    for(cemap_t::iterator iter = nextCandidateSet.begin(); iter != nextCandidateSet.end(); iter++) {
        printf("countSupport >> Count %d with ce ", iter->second);
        iter->first.printAll();
    }

    std::vector<CompositeEvent> toBeRemoved;
    for(cemap_t::iterator iter_tpK = nextCandidateSet.begin(); iter_tpK != nextCandidateSet.end(); iter_tpK++) {
        if((*iter_tpK).second < 2)
            toBeRemoved.push_back((*iter_tpK).first);
    }
    for(std::vector<CompositeEvent>::iterator iter_toBeRemoved = toBeRemoved.begin(); iter_toBeRemoved != toBeRemoved.end(); iter_toBeRemoved++) {
        nextCandidateSet.erase(*iter_toBeRemoved);
    }
    for(cemap_t::iterator iter = nextCandidateSet.begin(); iter != nextCandidateSet.end(); iter++) {
        printf("frequentSet >> Count %d with ce ", iter->second);
        iter->first.printAll();
    }
    toBeRemoved.clear();

    // 3
    cemap_t nextCandidateSet2 = iem->getNextCandidateSet(4, nextCandidateSet);
    for(cemap_t::iterator iter = nextCandidateSet2.begin(); iter != nextCandidateSet2.end(); iter++) {
        printf("nextCandidateSet2 >> Count %d with ce ", iter->second);
        iter->first.printAll();
    }

    for(int i = 0; i < database.size(); i++)
        iem->countSupport(4, database[i], nextCandidateSet2);
    for(cemap_t::iterator iter = nextCandidateSet2.begin(); iter != nextCandidateSet2.end(); iter++) {
        printf("countSupport >> Count %d with ce ", iter->second);
        iter->first.printAll();
    }
    
    for(cemap_t::iterator iter_tpK = nextCandidateSet2.begin(); iter_tpK != nextCandidateSet2.end(); iter_tpK++) {
        if((*iter_tpK).second < 2)
            toBeRemoved.push_back((*iter_tpK).first);
    }
    for(std::vector<CompositeEvent>::iterator iter_toBeRemoved = toBeRemoved.begin(); iter_toBeRemoved != toBeRemoved.end(); iter_toBeRemoved++) {
        nextCandidateSet2.erase(*iter_toBeRemoved);
    }
    for(cemap_t::iterator iter = nextCandidateSet2.begin(); iter != nextCandidateSet2.end(); iter++) {
        printf("frequentSet >> Count %d with ce ", iter->second);
        iter->first.printAll();
    }
    toBeRemoved.clear();

    // 5
    cemap_t nextCandidateSet3 = iem->getNextCandidateSet(4, nextCandidateSet2);
    for(cemap_t::iterator iter = nextCandidateSet3.begin(); iter != nextCandidateSet3.end(); iter++) {
        printf("nextCandidateSet3 >> Count %d with ce ", iter->second);
        iter->first.printAll();
    }
    */
}
