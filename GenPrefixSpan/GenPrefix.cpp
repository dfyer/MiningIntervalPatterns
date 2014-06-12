// GenPrefix.cpp: implementation of the CGenPrefix class.   
//   
//////////////////////////////////////////////////////////////////////   
   
#include "stdafx.h"   
#include "PrefixSpan.h"   
#include "GenPrefix.h"   
   
#include<string>   
   
#ifdef _DEBUG   
#undef THIS_FILE   
static char THIS_FILE[]=__FILE__;   
#define new DEBUG_NEW   
#endif   
   
//////////////////////////////////////////////////////////////////////   
// Construction/Destruction   
//////////////////////////////////////////////////////////////////////   
extern bool IsFinished;   
CGenPrefix::~CGenPrefix()   
{   
//  if(out!=NULL)   
//      out.close();   
}   
   
CGenPrefix::CGenPrefix(int support, int kind,CPrefixSpanDlg *Dlg):Prefix(support, kind,Dlg)   
//CGenPrefix::CGenPrefix(int support, int kind, ofstream &o):Prefix(support, kind)   
{   
//  out=&o;   
}   
   
CGenPrefix::CGenPrefix(int gap, int support, int kind, CPrefixSpanDlg *Dlg):Prefix(support, kind,Dlg)   
//CGenPrefix::CGenPrefix(int gap, int support, int kind, ofstream &o):Prefix(support, kind)   
{   
    this->gap=gap;   
//  out=&o;   
//  out=new ofstream("result.txt", ios::out);   
}   
   
int CGenPrefix::GetGap()   
{   
    return gap;   
}   
   
void CGenPrefix::SetGap(int gap)   
{   
    this->gap=gap;   
}   
vector<ProjectedDB> CGenPrefix::makeProjectedDB(vector<ProjectedDB> projectedDBs,   
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
            for(int j=projectedDBs[i].nSet+1; j<max; j++)   
//          for(int j=projectedDBs[i].nSet+1; j<transactions[tranId].itemSets.size(); j++)   
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
   
int CGenPrefix::countSupport(vector<ProjectedDB> projectedDBs, Transaction prefix)   
{   
    int count=0;   
    if(prefix.itemSets.back().items.size()==1)   
    {   
        int id=prefix.itemSets.back().items.front().id;   
        for(int i=0; i<projectedDBs.size(); i++)   
        {   
            int tranId=projectedDBs[i].nTran;   
            int max;   
            if(projectedDBs[i].nSet==-1)   
                max=transactions[tranId].itemSets.size();   
            else   
                max=transactions[tranId].itemSets.size()>projectedDBs[i].nSet+gap+2?projectedDBs[i].nSet+gap+2:transactions[tranId].itemSets.size();   
            for(int j=projectedDBs[i].nSet+1; j<max; j++)   
//          for(int j=projectedDBs[i].nSet+1; j<transactions[tranId].itemSets.size(); j++)   
            {   
                vector<Item> items=transactions[tranId].itemSets[j].items;   
   
                for(int k=0; k<items.size(); k++)   
                    if(id==items[k].id) // &&((j-projectedDBs[i].nSet<=1)||(projectedDBs[i].nSet==-1)))   
                    {   
                        projectedDBs[i].nSet=j;   
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
            int max;   
            if(projectedDBs[i].nSet==-1)   
                max=transactions[tranId].itemSets.size();   
            else   
                max=transactions[tranId].itemSets.size()>projectedDBs[i].nSet+2+gap?projectedDBs[i].nSet+2+gap:transactions[tranId].itemSets.size();   
            for(int j=projectedDBs[i].nSet+1; j<max; j++)   
//          for(int j=projectedDBs[i].nSet; j<transactions[tranId].itemSets.size(); j++)   
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
vector<ActiveItem> CGenPrefix::pruneActiveItem(vector<ProjectedDB> projectedDBs,    
                        vector<ActiveItem> tempAIs, Transaction prefix)   
{   
    vector<ActiveItem> newAIs;   
    Transaction newPrefix;   
   
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
   
void CGenPrefix::readData(string fileName, vector<Transaction>* tempTrans,    
                          map<int,int>* tempItemCounter)   
{   
    ifstream fs(fileName.c_str(), ios::binary | ios::in);   
    string c;   
    map<int,int> inTranItemCounter;   
//  MessageBox("dfas");   
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
   
                if(!bJoin)   
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
            if(c[i]==' ')   
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
}   
/*  
void CGenPrefix::mine()  
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
    mine_time=t;  
    CString str;  
    str.Format("The program costs %f seconds!",t);  
    AfxMessageBox(str);  
    if(out!=NULL)  
    {  
        out->write(str, str.GetLength());  
        out->put('\n');  
    }  
  
}*/   
/*  
void CGenPrefix::writeSeqPattern(Transaction seqPattern)  
{  
    vector<ItemSet>::iterator it;  
    CString pattern="";  
    for(it=seqPattern.itemSets.begin(); it!=seqPattern.itemSets.end(); it++)  
    {  
        vector<Item>::iterator it2;  
        for(it2=it->items.begin(); it2!=it->items.end(); it2++)  
            pattern+=CUtil::intToString(it2->id)+" ";  
    }  
  
    if(outDlg!=NULL)  
    {  
        outDlg->m_ResultDisplay.AddString(pattern);  
    }  
  
    if(out!=NULL)  
    {  
        out->write(pattern, pattern.GetLength());  
        out->put('\n');  
    }  
  
}*/   
/*  
vector<ActiveItem> CGenPrefix::generateActiveItem(vector<ProjectedDB> projectedDBs,int index, vector<ActiveItem> activeItems, Transaction prefix)  
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
  
//  newAIs=pruneActiveItem(projectedDBs, activeItems, newPrefix);  
  
    return newAIs;  
}  
*/   
   
double CGenPrefix::GetRunTime()   
{   
    return this->mine_time;   
}   
