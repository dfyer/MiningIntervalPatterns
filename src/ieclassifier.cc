
#include "ieclassifier.h"


IEClassifier::IEClassifier(const std::vector<std::vector<Event> >& database, const std::vector<std::string>& database_class, const cemap_t& frequentPatterns, const double& info_gain) :
        database_class_(database_class), frequentPatterns_(frequentPatterns), info_gain_(info_gain) {
    assert(discriminatingPatterns_.size() == 0);
    assert(conf_.size() == 0);
    assert(clabel_.size() == 0);
    assert(sup_.size() == 0);
    assert(database.size() == database_class.size());

    // 1. Get discriminating patterns
    std::map<std::string, int> class_count;
    for(int i = 0; i < database_class.size(); i++)
        ++class_count[database_class[i]];

    double h_total = 0;
    for(std::map<std::string, int>::iterator iter = class_count.begin(); iter != class_count.end(); iter++) {
        double p_ci = ( (double) iter->second ) / database.size();
        h_total += -1 * p_ci * log(p_ci);
    }

    // for each tp in frequentPatterns
    for(cemap_t::iterator jter = frequentPatterns_.begin(); jter != frequentPatterns_.end(); jter++) {
        double p_tp = ( (double) jter->second ) / database.size();

        std::map<std::string, int> class_count_tp;
        std::map<std::string, int> class_count_tp_bar;
        int tp_size = 0;
        for(int i = 0; i < database.size(); i++) {
            if(containsPattern(jter->first, database[i])) {
                ++class_count_tp[database_class[i]];
                ++tp_size;
            }
            else {
                ++class_count_tp_bar[database_class[i]];
            }
        }

        // there's at least one event list that contains temporal pattern TP
        assert(class_count_tp.size() > 0);

        double h_tp = 0;
        double highest_conf_ci_tp = -1;
        std::map<std::string, int>::iterator iter_highest_conf_ci_tp = class_count_tp.end();
        for(std::map<std::string, int>::iterator iter = class_count_tp.begin(); iter != class_count_tp.end(); iter++) {
            assert(iter->second > 0);
            double p_ci_tp = ( (double) iter->second ) / tp_size;
            if(highest_conf_ci_tp < p_ci_tp) {
                highest_conf_ci_tp = p_ci_tp;
                iter_highest_conf_ci_tp = iter;
            }
            h_tp += -1 * p_ci_tp * log(p_ci_tp);
        }
        h_tp *= p_tp;

        assert(highest_conf_ci_tp != -1);
        assert(iter_highest_conf_ci_tp != class_count_tp.end());

        double h_tp_bar = 0;
        for(std::map<std::string, int>::iterator iter = class_count_tp_bar.begin(); iter != class_count_tp_bar.end(); iter++) {
            assert(iter->second > 0);
            double p_ci_tp_bar = ( (double) iter->second )  / ( database.size() - tp_size );
            h_tp_bar += -1 * p_ci_tp_bar * log(p_ci_tp_bar);
        }
        h_tp_bar *= ( 1 - p_tp );

        double g_tp = h_total - h_tp - h_tp_bar;
        if(g_tp >= info_gain_) {
            discriminatingPatterns_.push_back(jter->first);
            conf_.push_back(highest_conf_ci_tp);
            clabel_.push_back(iter_highest_conf_ci_tp->first);
            sup_.push_back(iter_highest_conf_ci_tp->second);

            assert(discriminatingPatterns_.size() == conf_.size() && conf_.size() == clabel_.size() && clabel_.size() == sup_.size());
        }
    }
}

std::string IEClassifier::best_conf(std::vector<Event> event_sequence) {
    std::string best_class;
    double best_conf_now = 0;
    int sup_now = 0;

    // Get PatternMatchI
    std::vector<int> index_matchingPatterns;
    for(int i = 0; i < discriminatingPatterns_.size(); i++)
        if(containsPattern(discriminatingPatterns_[i], event_sequence))
            index_matchingPatterns.push_back(i);

    if(index_matchingPatterns.size() > 0) {
        for(int i = 0; i < index_matchingPatterns.size(); i++) {
            if(conf_[i] > best_conf_now) {
                best_class = clabel_[i];
                best_conf_now = conf_[i];
                sup_now = sup_[i];
            } else if(conf_[i] == best_conf_now && sup_[i] > sup_now) {
                best_class = clabel_[i];
                best_conf_now = conf_[i];
                sup_now = sup_[i];
            }
        }
        return best_class;
    } else {
        return NULL;
    }
}

std::string IEClassifier::majority_class(std::vector<Event> event_sequence) {
    std::string best_class;

    // Get PatternMatchI
    std::vector<int> index_matchingPatterns;
    for(int i = 0; i < discriminatingPatterns_.size(); i++)
        if(containsPattern(discriminatingPatterns_[i], event_sequence))
            index_matchingPatterns.push_back(i);

    std::map<std::string, int> count;
    //for(int i = 0; i < database_class_.size(); i++)
    //    count[database_class_[i]] = 0;

    for(int i = 0; i < index_matchingPatterns.size(); i++) {
        ++count[clabel_[i]];
    }

    if(count.size() > 0) {
        int max_count = 0;
        for(std::map<std::string, int>::iterator iter = count.begin(); iter != count.end(); iter++)
            if(iter->second > max_count)
                best_class = iter->first;
        return best_class;
    } else {
        return NULL;
    }
}

bool IEClassifier::containsPattern(CompositeEvent tp, std::vector<Event> event_sequence) {
    CompositeEvent cmp;
    int now = 0;
    for(int i = 0; i < event_sequence.size(); i++) {
        if(tp.event_list_[now].type_ == event_sequence[i].type_) {
            cmp.append(event_sequence[i]);
            ++now;
        }
    }

    if(cmp.getLength() != tp.getLength())
        return false;

    // implementing operator== is boring
    bool rtn = ( !(tp < cmp) ) && ( !(cmp < tp) );

    return rtn;
}
