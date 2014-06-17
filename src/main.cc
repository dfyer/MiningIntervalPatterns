
#include <vector>
#include <string>

#include "event.h"
#include "temporal_relation.h"
#include "composite_event.h"
#include "ieminer.h"
#include "ieclassifier.h"

int main() {
	// Set-up test data set
	std::vector<std::vector<Event> > database;
    std::vector<std::string> database_class;

	std::vector<Event> el1;
	el1.push_back(Event(100, 0, 10));
	el1.push_back(Event(101, 5, 16));
	el1.push_back(Event(102, 8, 30));
	el1.push_back(Event(103, 21, 26));
	database.push_back(el1);
    database_class.push_back("A");

	std::vector<Event> el2;
	el2.push_back(Event(100, 0, 10));
	el2.push_back(Event(104, 34, 40));
	el2.push_back(Event(105, 45, 55));
	database.push_back(el2);
    database_class.push_back("B");

	std::vector<Event> el3;
	el3.push_back(Event(100, 0, 10));
	el3.push_back(Event(101, 5, 16));
	el3.push_back(Event(102, 8, 30));
	el3.push_back(Event(103, 20, 25));
	el3.push_back(Event(104, 34, 40));
	database.push_back(el3);
    database_class.push_back("A");

	std::vector<Event> el4;
	el4.push_back(Event(100, 1, 11));
	el4.push_back(Event(101, 6, 17));
	el4.push_back(Event(103, 21, 26));
	el4.push_back(Event(104, 35, 41));
	el4.push_back(Event(105, 46, 56));
	database.push_back(el4);
    database_class.push_back("B");

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

    // IEClassifier
    IEClassifier* iec = new IEClassifier(database, database_class, ieminer_result, 0.00000001);
    for(int i = 0; i < iec->discriminatingPatterns_.size(); i++) {
        printf("Discriminating Patterns: conf = %f, sup = %d, class_label = %s >> ", iec->conf_[i], iec->sup_[i], iec->clabel_[i].c_str());
        iec->discriminatingPatterns_[i].printAll();
    }

	std::vector<Event> es;
	es.push_back(Event(100, 0, 10));
	es.push_back(Event(102, 8, 30));
	es.push_back(Event(103, 21, 26));
    printf("Best_Conf >> %s\n", iec->best_conf(es).c_str());
    printf("Majority_Class >> %s\n", iec->majority_class(es).c_str());
}
