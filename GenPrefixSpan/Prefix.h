#include <map> 
#include <vector> 
#include <fstream> 
#include <time.h> 
using namespace std; 
#include "PrefixSpanDlg.h" 
 
class Item; 
class ItemSet; 
class Transaction; 
class ProjectedDB; 
class ActiveItem; 
class BiActiveItem; 
 
class Prefix 
{ 
public: 
 
//  ofstream outfile; 
 
    //For callback function greaterThan() to call compare() 
    static Prefix * pthis; 
 
    //User specified support 
    int support; 
 
    //User choose the method 
    int kind; 
 
    //Contain the initial transactions with infrequent items pruned 
    vector<Transaction> transactions; 
 
    //Contain frequent items sorted in decrease order by frequency 
    vector<Item> frequentItem; 
 
    //Contain frequent items and their supports for fast access 
    map<int,int> itemCounter; 
 
//  ofstream outfile; 
    CPrefixSpanDlg * outDlg; 
 
    Prefix(int support,int kind,CPrefixSpanDlg * Dlg=NULL); 
    ~Prefix(); 
 
    void setSupport(int support){ this->support=support;} 
 
    //Call readData() to read the transactions from file and  
    //call pruneItem() to prune infrequent items 
    void initialize(string fileName); 
 
    virtual void readData(string fileName, vector<Transaction>* tempTrans, 
                    map<int,int>* tempItemCounter); 
 
    void pruneItem(vector<Transaction>* tempTrans, 
                    map<int,int>* tempItemCounter); 
     
    //Mine sequential patterns. Encapsulate mineSeqPattern(). Do some preprocessing work 
    //before calling mineSeqPattern(). It is called by the user 
    virtual void mine(); 
 
    //Mine sequential patterns. It's the core function. Called by mine(); 
    void mineSeqPattern(vector<ProjectedDB> projectedDBs, 
                vector<ActiveItem> activeItems, Transaction prefix); 
 
    virtual vector<ProjectedDB> makeProjectedDB(vector<ProjectedDB> projectedDBs, 
                                        Transaction prefix); 
 
    //Active items are the items to which we'll apply the 3-way Apriori to generate 
    //candidate active items 
    virtual vector<ActiveItem> generateActiveItem(vector<ProjectedDB> projectedDBs, int index,  
                                vector<ActiveItem> activeItems, Transaction prefix); 
     
    //Prune active items whose supports are less than the user specified value 
    virtual vector<ActiveItem> pruneActiveItem(vector<ProjectedDB> projectedDBs,  
                                vector<ActiveItem> tempAIs, Transaction prefix); 
 
    //count the support of the prefix. Called by pruneActiveItem to prune 
    //candidate active item 
    virtual int countSupport(vector<ProjectedDB> projectedDBs, Transaction prefix); 
 
    //write the sequential patterns found to file 
    virtual void writeSeqPattern(Transaction seqPattern); 
 
    //Callback function for sorting frequent items 
    static bool greaterThan(Item elem1, Item elem2); 
    bool compare(int elem1, int elem2); 
 
 
 
    void biLevelMine(vector<ProjectedDB> projectedDBs,vector<BiActiveItem> biActiveItems,vector<ActiveItem> activeItems,Transaction prefix); 
     vector<BiActiveItem> generateBiActiveItem(vector<ProjectedDB> projectedDBs,vector<ActiveItem> activeItems,Transaction prefix); 
 
    vector<ProjectedDB> makebiProjectedDB(vector<ProjectedDB> projectedDBs,BiActiveItem biActiveItem,Transaction prefix); 
     
};
