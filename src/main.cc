
#include <vector>

#include "event.h"
#include "temporal_relation.h"
#include "composite_event.h"

int main() {
	// Set-up test data set
	std::vector<std::vector<Event> > database;

	std::vector<Event> el1;
	el1.push_back(Event(100, 0, 10));
	el1.push_back(Event(101, 5, 16));
	el1.push_back(Event(102, 8, 30));
	el1.push_back(Event(103, 20, 25));
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
	el4.push_back(Event(100, 0, 10));
	el4.push_back(Event(101, 5, 16));
	el4.push_back(Event(103, 20, 25));
	el4.push_back(Event(104, 34, 40));
	el4.push_back(Event(105, 45, 55));
	database.push_back(el4);

	// CompositeEvent append test
	std::vector<CompositeEvent> ce_list;
	for(std::vector<std::vector<Event> >::iterator iter = database.begin(); iter != database.end(); iter++) {
		printf("creating a composite event..\n");
		CompositeEvent* ce = new CompositeEvent();
		for(std::vector<Event>::iterator jter = (*iter).begin(); jter != (*iter).end(); jter++) {
			printf("appending an event..\n");
			ce->append(*jter);
			ce->printAll();
		}
		printf("One composite event created!\n");
		ce_list.push_back(*ce);
	}

	// Generate TP
}
