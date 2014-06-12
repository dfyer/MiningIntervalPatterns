#include<vector> 
using namespace std; 
 
class Item 
{ 
public: 
    int id; 
 
    Item(int id); 
    Item(); 
    bool operator==(const Item& another); 
}; 
 
class ItemSet 
{ 
public: 
    vector<Item> items; 
 
    ItemSet(); 
}; 
 
class Transaction 
{ 
public: 
    vector<ItemSet> itemSets; 
    Transaction(); 
}; 
 
class ProjectedDB 
{ 
public: 
    int nTran; 
    int nSet; 
    int nItem; 
 
    ProjectedDB(int nTran, int nSet, int nItem); 
    ProjectedDB(); 
}; 
 
class ActiveItem 
{ 
public: 
    int id; 
    bool bJoin; 
 
    ActiveItem(int id, bool bJoin); 
    ActiveItem(); 
}; 
 
 
 
class BiActiveItem 
{ 
public: 
    ActiveItem first; 
    ActiveItem second; 
    void operator=(const BiActiveItem& another); 
    bool operator==(const BiActiveItem& another); 
    BiActiveItem(ActiveItem first,ActiveItem second); 
    BiActiveItem(); 
};
