
#ifndef IECLASSIFIER_H_
#define IECLASSIFIER_H_

#include <vector>
#include <map>
#include <math.h>
#include <string>

#include "composite_event.h"

class IEClassifier {
public:
    IEClassifier(const std::vector<std::vector<Event> >& database, const std::vector<std::string>& database_class, const cemap_t& frequentPatterns, const double& info_gain);

    std::string best_conf(std::vector<Event> event_sequence);
    std::string majority_class(std::vector<Event> event_sequence);

    bool containsPattern(CompositeEvent tp, std::vector<Event> event_sequence);
    
//private:
    std::vector<std::string> database_class_;
    cemap_t frequentPatterns_;
    double info_gain_;
    std::vector<CompositeEvent> discriminatingPatterns_;
    std::vector<double> conf_;
    std::vector<std::string> clabel_;
    std::vector<int> sup_;
};

#endif
