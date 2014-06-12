#include "stdafx.h"   
#include "datastruct.h"   
#include "Prefix.h"   
   
#include<iostream>   
#include<string>   
#include<xstring>   
#include<algorithm>   
using namespace std;   
   
Prefix* Prefix::pthis;   
extern bool IsFinished;   
Prefix::Prefix(int support,int kind,CPrefixSpanDlg * Dlg/*=NULL*/)   
//Prefix::Prefix(int support,int kind)   
{   
    Prefix::pthis=this;   
    this->outDlg=Dlg;   
    this->support=support;   
    this->kind=kind;   
//  outfile.open("out.txt", ios::binary | ios::out);   
}   
   
Prefix::~Prefix()   
{   
//  outfile.close();   
}   
   
/**  
 * @param fileName a file containing transaction sequences  
 */   
   
void Prefix::initialize(string fileName)   
{   
    vector<Transaction> tempTrans;    //temperary vector to hold transactions read    
                                    //from file. It will be pruned later   
    map<int,int> tempItemCounter; //temperary map to hold items and their supports   
                                    //for fast access   
       
    readData(fileName, &tempTrans, &tempItemCounter);   
    pruneItem(&tempTrans, &tempItemCounter);   
}   
   
   
/**  
 * @param fileName a file containing transaction sequences  
 * @param tempTrans temperary vector to hold transactions read from file  
 * @param tempItemCounter temperary map to hold items and their supports for fast access  
 */   
   
void Prefix::readData(string fileName, vector<Transaction>* tempTrans,    
                          map<int,int>* tempItemCounter)   
{   
    ifstream fs(fileName.c_str(), ios::binary | ios::in);   
    string c;   
    map<int,int> inTranItemCounter;   
//  뗍흙匡숭櫓돨埼죗       
    do   
    {   
        getline(fs,c);   
        unsigned int i=0;   
        Item item;   
        ItemSet itemSet;   
        Transaction transaction;   
        bool bJoin=false;   
           
        while(i<c.length())   
        {   
            if(c[i]>='0' && c[i]<='9')   
            {   
                int s=0;   
                while(c[i]>='0' && c[i]<='9')   
                {   
                    s=s*10+c[i]-48;   
                    i++;   
                }   
                item.id=s;   
                inTranItemCounter[s]++; //inTranItemCounter[s]션쩌禱羹s瞳뎠품埼죗櫓놔君돨늴鑒   
   
                if(!bJoin)  //島돕"|"橙쉥빈충돨item隣槨寧몸itemSet속흙돕transaction櫓   
                {   
                    itemSet.items.push_back(item);   
                    transaction.itemSets.push_back(itemSet);   
                    itemSet.items.clear();   
                    bJoin=true;   
                }   
                else   
                {          
                    transaction.itemSets.back().items.push_back(item);  //vector::back()삿혤離빈寧몸禱羹，굶얌쉥item속흙돕離빈寧몸itemSet櫓   
                    //bJoin=false;   
                }   
            }   
            if(c[i]=='|')   
                bJoin=false;   
            i++;   
        }   
           
        if(transaction.itemSets.size()!=0)   
        {   
            map<int,int>::iterator it;   
            for(it=inTranItemCounter.begin(); it!=inTranItemCounter.end(); it++)   
                (*tempItemCounter)[it->first]++; //괏닸몹몸禱羹돨鑒커，옵鹿닒櫓돤돕틉런item섞북，   
            //tempItemCounter[s]션쩌죄놔君s돨埼죗돨몸鑒,페櫓it->first깊刻禱羹s   
            inTranItemCounter.clear();   
   
            tempTrans->push_back(transaction);   //쉥寧몸transaction속흙돕埼죗섞북櫓   
            transaction.itemSets.clear();   
        }   
           
    }while(!fs.eof());   
//  渴놔埼죗섞북   
    ofstream test("testout.txt");   
    for(int k=0;k<tempTrans->size();k++)   
    {   
        for(int k1=0;k1<(*tempTrans)[k].itemSets.size();k1++)   
        {   
            for(int k2=0;k2<(*tempTrans)[k].itemSets[k1].items.size();k2++)   
            {   
                test<<(*tempTrans)[k].itemSets[k1].items[k2].id;   
                if(k2!=(*tempTrans)[k].itemSets[k1].items.size()-1)   
                    test<<" ";   
            }   
            if(k1!=(*tempTrans)[k].itemSets.size()-1)   
                test<<"|";   
        }   
        test<<endl;   
    }   
   
}   
   
/**  
 * callback function call by sort  
 */   
   
bool Prefix::greaterThan(Item elem1, Item elem2)   
{   
    return pthis->compare(elem1.id,elem2.id);   
}   
   
bool Prefix::compare(int elem1, int elem2)   
{   
    map<int,int>::iterator it1=itemCounter.find(elem1);   
    map<int,int>::iterator it2=itemCounter.find(elem2);   
       
    return it1->second>it2->second;   
}   
   
/**  
 * @param fileName a file containing transaction sequences  
 * @param tempTrans temperary vector to hold transactions that will be pruned  
 * @param tempItemCounter temperary map to hold items and their supports for fast access  
 */   
//뚤닒readData변鑒櫓뗍흙돨淃섞숏輦，페茄앎角烱뇜렷틉런淃，쏭괏즛틉런淃，깻뚤itemSet櫓돨   
//禱羹（객亮連넣똑）탤埼，깻쉥숏輦빈돨埼죗渴놔돕匡숭testout1.txt   
void Prefix::pruneItem(vector<Transaction>* tempTrans,   
                           map<int,int>* tempItemCounter)   
{   
    map<int,int>::iterator it;   
    for(it=tempItemCounter->begin(); it!=tempItemCounter->end(); it++)   
    {   
        if(it->second>=support)   //prune the infrequent items，페櫓it->second럿쀼밑젬돨令，   
            //it->first럿쀼숩令   
        {   
            frequentItem.push_back(Item(it->first)); //괏닸틉런淃섞   
            itemCounter.insert(pair<int,int>(it->first, it->second));//괏닸틉런淃섞섟페連넣똑，긱黨우醵꿴冷   
        }   
    }   
       
    //Sort frequent items by their supports. Stability is required!   
    stable_sort(frequentItem.begin(),frequentItem.end(),greaterThan); //뚤틉런淃섞객連넣똑탤埼(슉埼탤埼)   
   
    //Add an increment to the frequency of items to ensure there are no ties   
    //멥꼇헌뇝侶얌폅痂척鱗痰？？   
    for(unsigned int j=0; j<frequentItem.size(); j++)   
        itemCounter[frequentItem[j].id]+=frequentItem.size()-j;   
   
       
    ItemSet itemSet;   
    Transaction trans;   
       
    //Prune infrequent items from transactions   
    for(unsigned int i=0; i<tempTrans->size(); i++)   
    {   
        Transaction tempTran=(*tempTrans)[i];   
        for(unsigned int j=0; j<tempTran.itemSets.size(); j++)   
        {   
            ItemSet tempItemSet=tempTran.itemSets[j];   
            for(unsigned int k=0; k<tempItemSet.items.size(); k++)   
            {   
                //ItemCounter contains only those frequent   
                //map::end()변鑒럿쀼離빈寧몸禱羹돨苟寧몸，섦왕禱羹，if刀얌돨벵屢앎옵鹿깊刻槨   
                //흼瞳itemCounter櫓콘冷돕禱羹k   
                if(itemCounter.end()!=itemCounter.find(tempItemSet.items[k].id))   
                {   
                    //An item can only appear in an item set once at most   
                    //苟충刀얌鱗痰앎角瞳뎠품itemSet櫓烱뇜렷틉런淃섞，깻횅괏캐item쏭놔君寧늴   
                    //   
                    unsigned int x;   
                    for(x=0; x<itemSet.items.size(); x++)   
                    {   
                        if(itemSet.items[x].id==tempItemSet.items[k].id) break;   
                    }   
                    if(itemSet.items.size()==0 || x==itemSet.items.size())   
                        itemSet.items.push_back(tempItemSet.items[k]);   
                }   
            }   
            if(itemSet.items.size()!=0)   
            {   
                //뚤綠쒔烱뇜죄렷틉런淃，쏭관벵틉런淃섞돨itemSet탤埼   
                stable_sort(itemSet.items.begin(), itemSet.items.end(), greaterThan);   
                trans.itemSets.push_back(itemSet);   
                itemSet.items.clear();   
            }   
        }   
        if(trans.itemSets.size()!=0)   
        {   
            transactions.push_back(trans);   
            trans.itemSets.clear();   
        }      
    }   
/*  
    ofstream test("testout1.txt");  
    for(int t=0;t<frequentItem.size();t++)  
        test<<frequentItem[t].id<<" ";  
    test<<endl;  
    for(int k=0;k<transactions.size();k++)  
    {  
        for(int k1=0;k1<transactions[k].itemSets.size();k1++)  
        {  
            for(int k2=0;k2<transactions[k].itemSets[k1].items.size();k2++)  
            {  
                test<<transactions[k].itemSets[k1].items[k2].id;  
                if(k2!=transactions[k].itemSets[k1].items.size()-1)  
                    test<<" ";  
            }  
            if(k1!=transactions[k].itemSets.size()-1)  
                test<<"|";  
        }  
        test<<endl;  
    }  
*/   
}   
   
/**  
 * Make projected database according to the last element of prefix  
 * @param projectedDBs projected database w.r.t. prefix with the last element ignored  
 * @return projected database w.r.t. prefix  
 */   
//뭐芚埼죗prefix돨projectedDB깻럿쀼,닒굶넋埼돨獵契법넋였，慨빎pruneItem변鑒唐硅댄轎，   
//pruneItem변鑒커품茄君돨角쉥寧몸itemSet櫓돨禱羹객亮페連넣똑탤埼，랍茄셥壇맡角객亮禱羹   
//굶�돨댕鬼탤埼。   
vector<ProjectedDB> Prefix::makeProjectedDB(vector<ProjectedDB> projectedDBs,   
                            Transaction prefix)   
{   
    vector<ProjectedDB> newPDBs;   
    ProjectedDB pdb;   
   
    if(prefix.itemSets.back().items.size()==1)  //prefix離빈寧몸itemSet섞북돨禱羹鑒槨1   
    {   
        int id=prefix.itemSets.back().items.front().id; //삿혤맡id   
        for(int i=0; i<projectedDBs.size(); i++)   
        {   
            int tranId=projectedDBs[i].nTran;   
            int max;   
            if(projectedDBs[i].nSet==-1)   
            {   
                max=transactions[tranId].itemSets.size();   
            }   
            else   
            {   
                max=transactions[tranId].itemSets.size()>projectedDBs[i].nSet+2?projectedDBs[i].nSet+2:transactions[tranId].itemSets.size();   
            }   
//          for(int j=projectedDBs[i].nSet+1; j<max; j++)   
            for(int j=projectedDBs[i].nSet+1; j<transactions[tranId].itemSets.size(); j++)   
            {   
                vector<Item> items=transactions[tranId].itemSets[j].items;   
                for(int k=0; k<items.size(); k++)   
                {   
                    if(id==items[k].id)   
                    {   
                        pdb.nTran=tranId;   
                        pdb.nSet=j;   
                        pdb.nItem=k;   
                        newPDBs.push_back(pdb);   
                        goto L1;   
                    }   
                }   
            }   
L1:         ;   
        }   
    }   
    else    //prefix離빈寧몸itemSet섞북돨禱羹鑒>1   
    {   
        for(int i=0; i<projectedDBs.size(); i++)   
        {   
            int tranId=projectedDBs[i].nTran;   
            for(int j=projectedDBs[i].nSet; j<transactions[tranId].itemSets.size(); j++)   
            {   
                vector<Item> items=transactions[tranId].itemSets[j].items;   
                bool bFind=true;   
   
                int last=0;   
                for(int k=0; k<prefix.itemSets.back().items.size(); k++)   
                {   
                    int id=prefix.itemSets.back().items[k].id;   
                    for(int q=last; q<items.size(); q++)   
                    {   
                        if(items[q].id==id)   
                        {   
                            last=q+1;   
                            break;   
                        }   
   
                        if(q==items.size())   
                        {   
                            bFind=false;   
                            break;   
                        }   
                    }   
                       
                }   
                if(bFind==true)   
                {      
                    pdb.nTran=tranId;   
                    pdb.nSet=j;   
                    pdb.nItem=last;   
                    newPDBs.push_back(pdb);   
                    goto L2;   
                }   
            }   
L2:         ;   
        }   
    }   
    return newPDBs;   
}   
   
//generator the biProjectedDBs   
vector<ProjectedDB> Prefix::makebiProjectedDB(vector<ProjectedDB> projectedDBs,BiActiveItem biActiveItem,    
                            Transaction prefix)   
{   
    vector<ProjectedDB> newPDBs;   
    ProjectedDB pdb;   
   
    if(!biActiveItem.first.bJoin && !biActiveItem.second.bJoin)   
    {   
        int id1=biActiveItem.first.id;   
        int id2=biActiveItem.second.id;   
        int flag=2;   
        for(int i=0; i<projectedDBs.size(); i++)   
        {   
            int tranId=projectedDBs[i].nTran;   
            for(int j=projectedDBs[i].nSet+1; j<transactions[tranId].itemSets.size(); j++)   
            {   
                vector<Item> items=transactions[tranId].itemSets[j].items;   
                if(2==flag)   
                    for(int k=0; k<items.size(); k++)   
                    {   
                        if(id1==items[k].id)   
                        {   
                            flag--;   
                            break;   
                        }   
                    }   
                else if(1==flag)   
                    for(int k1=0;k1<items.size();k1++)   
                        if(id2==items[k1].id)   
                        {   
                            pdb.nTran=tranId;   
                            pdb.nSet=j;   
                            pdb.nItem=k1;   
                            newPDBs.push_back(pdb);   
                            goto L1;   
                        }   
            }   
L1:         ;flag=2;   
        }   
    }   
    else if(biActiveItem.first.bJoin && biActiveItem.second.bJoin)   
    {   
        for(int i=0; i<projectedDBs.size(); i++)   
        {   
            int tranId=projectedDBs[i].nTran;   
            for(int j=projectedDBs[i].nSet; j<transactions[tranId].itemSets.size(); j++)   
            {   
                vector<Item> items=transactions[tranId].itemSets[j].items;   
                bool bFind=true;   
   
                int last=0;   
                for(int k=0; k<prefix.itemSets.back().items.size(); k++)   
                {   
                    int id=prefix.itemSets.back().items[k].id;   
                    for(int q=last; q<items.size(); q++)   
                    {   
                        if(items[q].id==id)   
                        {   
                            last=q+1;   
                            break;   
                        }   
   
                        if(q==items.size())   
                        {   
                            bFind=false;   
                            break;   
                        }   
                    }   
                }   
                if(bFind==true)   
                {                      
                    pdb.nTran=tranId;   
                    pdb.nSet=j;   
                    pdb.nItem=last;   
                    newPDBs.push_back(pdb);   
                    goto L2;   
                }   
            }   
L2:         ;   
        }   
    }   
    else if(!biActiveItem.first.bJoin && biActiveItem.second.bJoin)   
    {   
        for(int i=0; i<projectedDBs.size(); i++)   
        {   
            int tranId=projectedDBs[i].nTran;   
            for(int j=projectedDBs[i].nSet+1; j<transactions[tranId].itemSets.size(); j++)   
            {   
                vector<Item> items=transactions[tranId].itemSets[j].items;   
                bool bFind=true;   
   
                int last=0;   
                for(int k=0; k<prefix.itemSets.back().items.size(); k++)   
                {   
                    int id=prefix.itemSets.back().items[k].id;   
                    for(int q=last; q<items.size(); q++)   
                    {   
                        if(items[q].id==id)   
                        {   
                            last=q+1;   
                            break;   
                        }   
   
                        if(q==items.size())   
                        {   
                            bFind=false;   
                            break;   
                        }   
                    }   
                }   
                if(bFind==true)   
                {                      
                    pdb.nTran=tranId;   
                    pdb.nSet=j;   
                    pdb.nItem=last;   
                    newPDBs.push_back(pdb);   
                    goto L3;   
                }   
            }   
L3:         ;   
        }   
    }   
    else if(biActiveItem.first.bJoin && !biActiveItem.second.bJoin)   
    {   
        for(int i=0; i<projectedDBs.size(); i++)   
        {   
            int tranId=projectedDBs[i].nTran;   
            int ipos=0;   
            for(int j=projectedDBs[i].nSet; j<transactions[tranId].itemSets.size(); j++)   
            {   
                vector<Item> items=transactions[tranId].itemSets[j].items;   
                bool bFind=true;   
   
                int last=0;   
                int pos=prefix.itemSets.size();   
                for(int k=0; k<prefix.itemSets[pos-2].items.size(); k++)   
                {   
                    int id=prefix.itemSets[pos-2].items[k].id;   
                    for(int q=last; q<items.size(); q++)   
                    {   
                        if(items[q].id==id)   
                        {   
                            last=q+1;   
                            break;   
                        }   
   
                        if(q==items.size())   
                        {   
                            bFind=false;   
                            break;   
                        }   
                    }   
                }   
                if(bFind==true)   
                {                      
                    ipos=j+1;   
                    break;   
                }   
            }   
            int id=biActiveItem.second.id;   
            while(ipos<transactions[tranId].itemSets.size())   
            {   
                for(int t=0;t<transactions[tranId].itemSets[ipos].items.size();t++)   
                    if(id==transactions[tranId].itemSets[ipos].items[t].id)   
                    {   
                        pdb.nTran=tranId;   
                        pdb.nSet=ipos;   
                        pdb.nItem=t;   
                        newPDBs.push_back(pdb);   
                        goto L4;   
                    }   
                    ipos++;   
            }   
L4:         ;   
        }   
    }   
    return newPDBs;   
}   
/**  
 *Generate new active items  
 *@param projectedDBs projected database w.r.t. to prefix  
 *@param index the index of the main item in activeItems and it is the last element of prefix  
 *@param activeItems candidate items to which we'll apply the 3-way Apriori  
 *@return new active items whose prefix is the parameter prefix  
 */   
vector<ActiveItem> Prefix::generateActiveItem(vector<ProjectedDB> projectedDBs,   
                                int index, vector<ActiveItem> activeItems, Transaction prefix)   
{   
    vector<ActiveItem> newAIs;   
    vector<ActiveItem> tempAIs;   
    Transaction newPrefix=prefix;   
//  bJoin깊刻좃몸itemSet角뤠젯쌈瞳寧폅，뚤壇bi-level렘랬櫓，苟힛실앤黎櫓돨3禱莉페茄痰寧몸n≠n앤黎윱덜競，첼몸禱羹槨랗禱莉   
    //（寮뚤실窟�뇜棍）   
    if(!activeItems[index].bJoin)   
    {   
        for(int i=0; i<activeItems.size(); i++)   
        {   
            if(!activeItems[i].bJoin)   
            {   
                tempAIs.push_back(ActiveItem(activeItems[i].id, false));   
                if(i!=index)   
                    tempAIs.push_back(ActiveItem(activeItems[i].id, true));   
            }   
        }   
    }   
    else   
    {   
        for(int i=0; i<activeItems.size(); i++)   
        {   
            if(i!=index)   
            {   
                if(!activeItems[i].bJoin)   
                    tempAIs.push_back(ActiveItem(activeItems[i].id, false));   
                else   
                    tempAIs.push_back(ActiveItem(activeItems[i].id, true));   
            }   
        }   
    }   
    newAIs=pruneActiveItem(projectedDBs, tempAIs, newPrefix);   
    return newAIs;   
}   
   
vector<ActiveItem> Prefix::pruneActiveItem(vector<ProjectedDB> projectedDBs,    
                        vector<ActiveItem> tempAIs, Transaction prefix)   
{   
    vector<ActiveItem> newAIs;   
    Transaction newPrefix;   
/*  
    for(int i=0; i<tempAIs.size(); i++)  
    {  
        newPrefix=prefix;  
        if(tempAIs[i].bJoin)  
            newPrefix.itemSets.back().items.push_back(Item(tempAIs[i].id));  
        else  
        {  
            ItemSet itemSet;  
            itemSet.items.push_back(Item(tempAIs[i].id));  
            newPrefix.itemSets.push_back(itemSet);  
        }  
  
        if(countSupport(projectedDBs, newPrefix)>=support)  
            newAIs.push_back(tempAIs[i]);  
    }  
*/   
    for(int i=0; i<frequentItem.size(); i++)   
    {   
        newPrefix=prefix;   
            ItemSet itemSet;   
            itemSet.items.push_back(Item(frequentItem[i].id));   
            newPrefix.itemSets.push_back(itemSet);   
        if(countSupport(projectedDBs, newPrefix)>=support)   
            newAIs.push_back(ActiveItem(frequentItem[i].id, false));   
    }   
    return newAIs;   
}   
   
int Prefix::countSupport(vector<ProjectedDB> projectedDBs, Transaction prefix)   
{   
    int count=0;   
    if(prefix.itemSets.back().items.size()==1)   
    {   
        int id=prefix.itemSets.back().items.front().id;   
        for(int i=0; i<projectedDBs.size(); i++)   
        {   
            int tranId=projectedDBs[i].nTran;   
            for(int j=projectedDBs[i].nSet+1; j<transactions[tranId].itemSets.size(); j++)   
            {   
                vector<Item> items=transactions[tranId].itemSets[j].items;   
   
                for(int k=0; k<items.size(); k++)   
                    if(id==items[k].id)    
                    {   
                        count++;   
                        goto L1;   
                    }   
            }   
L1:         ;   
        }   
    }   
    else   
    {   
        for(int i=0; i<projectedDBs.size(); i++)   
        {   
               
            int tranId=projectedDBs[i].nTran;   
            for(int j=projectedDBs[i].nSet; j<transactions[tranId].itemSets.size(); j++)   
            {   
                vector<Item> items=transactions[tranId].itemSets[j].items;   
                bool bFind=true;   
                   
                int last=0;   
                for(int k=0; k<prefix.itemSets.back().items.size(); k++)   
                {   
                    int id=prefix.itemSets.back().items[k].id;   
                    for(int q=last; q<items.size(); q++)   
                    {   
                        if(items[q].id==id)   
                        {   
                            last=q;   
                            break;   
                        }   
                           
                    }   
                    if(q==items.size())   
                        {   
                            bFind=false;   
                            break;   
                        }   
                }   
                if(bFind==true)   
                {                      
                    count++;   
                    goto L2;   
                }   
            }   
L2:         ;   
        }   
    }   
    return count;   
}   
   
   
void Prefix::mine()   
{   
    clock_t start,finish;   
    start=clock();   
    vector<ProjectedDB> projectedDBs;   
    for(unsigned int i=0; i<transactions.size(); i++)   
        projectedDBs.push_back(ProjectedDB(i,-1, -1));   
   
    vector<ActiveItem> activeItems;   
    for(i=0; i<frequentItem.size(); i++)   
    {   
        activeItems.push_back(ActiveItem(frequentItem[i].id, false));   
        if(IsFinished)   
            return ;   
    }   
       
    Transaction prefix;   
   
    if(kind)    // Bi_Level렘랬   
        mineSeqPattern(projectedDBs, activeItems, prefix);   
    else        //Level_Bi_Level렘랬   
    {   
        vector<BiActiveItem> biActiveItems;   
        biActiveItems=generateBiActiveItem(projectedDBs,activeItems,prefix);   
   
        biLevelMine(projectedDBs,biActiveItems,activeItems,prefix);   
    }   
   
   
    finish=clock();   
    double t=(double)(finish-start)/CLOCKS_PER_SEC;   
    CString str;   
    str.Format("The program costs %f seconds!",t);   
    AfxMessageBox(str);   
}   
   
void Prefix::mineSeqPattern(vector<ProjectedDB> projectedDBs, vector<ActiveItem> activeItems,   
                    Transaction prefix)   
{   
    for(int i=0; i<activeItems.size(); i++)   
    {          
        if(IsFinished)   
            return ;   
   
        Transaction newPrefix=prefix;   
        if(!activeItems[i].bJoin)   
        {   
            ItemSet itemSet;   
            itemSet.items.push_back(Item(activeItems[i].id));   
            newPrefix.itemSets.push_back(itemSet);   
        }   
        else   
        {   
            newPrefix.itemSets.back().items.push_back(Item(activeItems[i].id));   
        }   
   
        writeSeqPattern(newPrefix); //쉥뎠품틉런埼죗渴놔   
   
        //돤돕newPrefix埼죗돨Proected-DB   
        vector<ProjectedDB> newProjectedDBs=makeProjectedDB(projectedDBs, newPrefix);   
           
        //It is impossible to generate new sequential patterns   
        if(newProjectedDBs.size()<support) continue;   
   
        vector<ActiveItem> newActiveItems=generateActiveItem(newProjectedDBs, i, activeItems, newPrefix);   
                   
                vector<ActiveItem>::iterator it;   
                CString pattern="";   
                for(it=newActiveItems.begin(); it!=newActiveItems.end(); it++)   
                {   
                    pattern+=CUtil::intToString(it->id)+" "+CUtil::intToString(it->bJoin)+"|";   
                }      
                   
        mineSeqPattern(newProjectedDBs,newActiveItems,newPrefix);   
    }   
}   
// 渴놔틉런埼죗   
void Prefix::writeSeqPattern(Transaction seqPattern)   
{   
    vector<ItemSet>::iterator it;   
    CString pattern="";   
    for(it=seqPattern.itemSets.begin(); it!=seqPattern.itemSets.end(); it++)   
    {   
        vector<Item>::iterator it2;   
        for(it2=it->items.begin(); it2!=it->items.end(); it2++)   
            pattern+=CUtil::intToString(it2->id)+" ";   
//          pattern.AppendFormat("%d  ",it2->id);   
            //outfile<<it2->id<<" ";   
        if(it!=seqPattern.itemSets.begin()+seqPattern.itemSets.size()-1)   
            pattern+="| ";   
//          pattern.Append("| ");   
    }   
    if(outDlg!=NULL)   
    {   
        outDlg->m_ResultDisplay.AddString(pattern);   
    }   
}   
   
void Prefix::biLevelMine(vector<ProjectedDB> projectedDBs,vector<BiActiveItem> biActiveItems,vector<ActiveItem> activeItems,Transaction prefix)   
{   
        for(int k=0;k<activeItems.size();k++)   
        {   
            if(IsFinished)   
                return;   
   
            Transaction outPrefix=prefix;   
            if(!activeItems[k].bJoin)   
            {   
                ItemSet itemSet;   
                itemSet.items.push_back(Item(activeItems[k].id));   
                outPrefix.itemSets.push_back(itemSet);   
            }   
            else   
            {   
                outPrefix.itemSets.back().items.push_back(Item(activeItems[k].id));   
            }   
            writeSeqPattern(outPrefix);   
        }   
   
   
    for(int i=0; i<biActiveItems.size(); i++)   
    {          
        if(IsFinished)   
            return ;   
   
        Transaction newPrefix=prefix;   
   
        if(!biActiveItems[i].first.bJoin)   
        {   
            ItemSet itemSet;   
            itemSet.items.push_back(Item(biActiveItems[i].first.id));   
            newPrefix.itemSets.push_back(itemSet);   
        }   
        else   
        {   
            newPrefix.itemSets.back().items.push_back(Item(biActiveItems[i].first.id));   
        }   
   
        //writeSeqPattern(newPrefix);   
        vector<ProjectedDB> newProjectedDBs=makeProjectedDB(projectedDBs, newPrefix);   
   
        if(!biActiveItems[i].second.bJoin)   
        {   
            ItemSet itemSet;   
            itemSet.items.push_back(Item(biActiveItems[i].second.id));   
            newPrefix.itemSets.push_back(itemSet);   
        }   
        else   
        {   
            newPrefix.itemSets.back().items.push_back(Item(biActiveItems[i].second.id));   
        }   
   
        writeSeqPattern(newPrefix);   
       
        newProjectedDBs=makeProjectedDB(newProjectedDBs,newPrefix);   
        //vector<ProjectedDB> newProjectedDBs=makebiProjectedDB(projectedDBs,biActiveItems[i], newPrefix);   
   
        //It is impossible to generate new sequential patterns   
        if(newProjectedDBs.size()<support) continue;   
   
        int index=-1;   
        for(int j=0;j<activeItems.size();j++)   
            if(biActiveItems[i].second.id==activeItems[j].id  && biActiveItems[i].second.bJoin==activeItems[j].bJoin)   
            {   
                index=j;   
                break;   
            }   
        if(-1==index)   
            for(int k=0;k<activeItems.size();k++)   
                if(biActiveItems[i].second.id==activeItems[k].id)   
                {   
                    index=k;   
                    break;   
                }   
   
        vector<ActiveItem> newActiveItems=generateActiveItem(newProjectedDBs, index, activeItems, newPrefix);   
       
        vector<BiActiveItem> newBiActiveItems=generateBiActiveItem(newProjectedDBs,newActiveItems,newPrefix);   
   
        biLevelMine(newProjectedDBs,newBiActiveItems,newActiveItems,newPrefix);   
    }   
}   
   
   
vector<BiActiveItem>  Prefix::generateBiActiveItem(vector<ProjectedDB> projectedDBs,vector<ActiveItem> activeItems,Transaction prefix)   
{   
    vector<BiActiveItem> tempBiAItems;   
    vector<BiActiveItem>::iterator pTemp;   
    for(int i=0;i<activeItems.size();i++)   
        for(int j=i;j<activeItems.size();j++)   
            if(i==j)   
            {   
                if(!activeItems[i].bJoin)   
                {   
                    ActiveItem first(activeItems[i].id,activeItems[i].bJoin);   
                    ActiveItem second(activeItems[j].id,activeItems[j].bJoin);   
                    BiActiveItem biActiveItem1(first,second);   
                    pTemp=find(tempBiAItems.begin(),tempBiAItems.end(),biActiveItem1);   
                    if(pTemp==tempBiAItems.end())   
                        tempBiAItems.push_back(biActiveItem1);   
                }   
            }   
            else   
            {   
                if(!activeItems[j].bJoin)   
                {   
                    ActiveItem first(activeItems[i].id,activeItems[i].bJoin);   
                    ActiveItem second(activeItems[j].id,activeItems[j].bJoin);   
                    BiActiveItem biActiveItem(first,second);   
                    pTemp=find(tempBiAItems.begin(),tempBiAItems.end(),biActiveItem);   
                    if(pTemp==tempBiAItems.end())   
                        tempBiAItems.push_back(biActiveItem);   
                }   
                if(!activeItems[i].bJoin)   
                {   
                    ActiveItem first(activeItems[i].id,activeItems[i].bJoin);   
                    ActiveItem second(activeItems[j].id,activeItems[j].bJoin);   
                    BiActiveItem biActiveItem(second,first);   
                    pTemp=find(tempBiAItems.begin(),tempBiAItems.end(),biActiveItem);   
                    if(pTemp==tempBiAItems.end())   
                        tempBiAItems.push_back(biActiveItem);   
                }   
                if(0==activeItems[i].bJoin && 0==activeItems[j].bJoin && activeItems[i].id!=activeItems[j].id)   
                {   
                    ActiveItem first(activeItems[i].id,activeItems[i].bJoin);   
                    ActiveItem second(activeItems[j].id,1);   
                    BiActiveItem biActiveItem(first,second);   
                    pTemp=find(tempBiAItems.begin(),tempBiAItems.end(),biActiveItem);   
                    if(pTemp==tempBiAItems.end())   
                        tempBiAItems.push_back(biActiveItem);   
                }   
                else if(1==activeItems[i].bJoin && 1==activeItems[j].bJoin && activeItems[i].id!=activeItems[j].id)   
                //  if(find(prefix.itemSets.back().items.begin(),prefix.itemSets.back().items.end(),Item(activeItems[j].id))!=prefix.itemSets.back().items.end())   
                       
                {   
                    ActiveItem first(activeItems[i].id,activeItems[i].bJoin);   
                    ActiveItem second(activeItems[j].id,activeItems[j].bJoin);   
                    BiActiveItem biActiveItem(first,second);   
                    pTemp=find(tempBiAItems.begin(),tempBiAItems.end(),biActiveItem);   
                    if(pTemp==tempBiAItems.end())   
                        tempBiAItems.push_back(biActiveItem);   
                }   
   
            }   
   
   
    vector<BiActiveItem> newBiActiveItems;   
    for(int k=0;k<tempBiAItems.size();k++)   
    {   
        Transaction newPrefix=prefix;   
        if(!tempBiAItems[k].first.bJoin)   
        {   
            ItemSet itemSet;   
            itemSet.items.push_back(Item(tempBiAItems[k].first.id));   
            newPrefix.itemSets.push_back(itemSet);   
        }   
        else   
        {   
            newPrefix.itemSets.back().items.push_back(Item(tempBiAItems[k].first.id));   
        }   
   
        vector<ProjectedDB> newProjectedDBs=makeProjectedDB(projectedDBs, newPrefix);   
   
        if(!tempBiAItems[k].second.bJoin)   
        {   
            ItemSet itemSet;   
            itemSet.items.push_back(Item(tempBiAItems[k].second.id));   
            newPrefix.itemSets.push_back(itemSet);   
        }   
        else   
        {   
            newPrefix.itemSets.back().items.push_back(Item(tempBiAItems[k].second.id));   
        }   
   
   
        if(countSupport(newProjectedDBs, newPrefix)>=support)   
            newBiActiveItems.push_back(tempBiAItems[k]);   
    }   
   
    return newBiActiveItems;   
   
}
