// GenPrefix.h: interface for the CGenPrefix class. 
// 
////////////////////////////////////////////////////////////////////// 
 
#if !defined(AFX_GENPREFIX_H__835C40FF_3787_482F_B67C_5F29EF395648__INCLUDED_) 
#define AFX_GENPREFIX_H__835C40FF_3787_482F_B67C_5F29EF395648__INCLUDED_ 
 
#if _MSC_VER > 1000 
#pragma once 
#endif // _MSC_VER > 1000 
#include "datastruct.h" 
 
#include "Prefix.h" 
 
class CGenPrefix : public Prefix   
{ 
public: 
    double GetRunTime(); 
    vector<ActiveItem> pruneActiveItem(vector<ProjectedDB> projectedDBs,vector<ActiveItem> tempAIs, Transaction prefix); 
//  vector<ActiveItem> generateActiveItem(vector<ProjectedDB> projectedDBs,int index, vector<ActiveItem> activeItems, Transaction prefix); 
    int countSupport(vector<ProjectedDB> projectedDBs, Transaction prefix); 
    vector<ProjectedDB> makeProjectedDB(vector<ProjectedDB> projectedDBs,Transaction prefix); 
    void SetGap(int gap); 
    int GetGap(); 
    CGenPrefix(int gap, int support,int kind,  CPrefixSpanDlg * Dlg=NULL);//,ofstream &o); 
    CGenPrefix(int support,int kind ,CPrefixSpanDlg * Dlg=NULL);//,ofstream &o);  
    virtual ~CGenPrefix(); 
    void readData(string fileName, vector<Transaction>* tempTrans, 
                    map<int,int>* tempItemCounter); 
//  void mine(); 
//  void writeSeqPattern(Transaction seqPattern); 
private: 
    int gap; 
    ofstream *out; 
    double mine_time; 
}; 
 
#endif // !defined(AFX_GENPREFIX_H__835C40FF_3787_482F_B67C_5F29EF395648__INCLUDED_) 
