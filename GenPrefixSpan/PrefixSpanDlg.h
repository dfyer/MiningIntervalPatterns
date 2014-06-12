// PrefixSpanDlg.h : header file 
// 
 
#if !defined(AFX_PREFIXSPANDLG_H__9AD5218A_2BE2_41DB_B3C4_EB3B14C3F107__INCLUDED_) 
#define AFX_PREFIXSPANDLG_H__9AD5218A_2BE2_41DB_B3C4_EB3B14C3F107__INCLUDED_ 
 
#if _MSC_VER > 1000 
#pragma once 
#endif // _MSC_VER > 1000 
 
///////////////////////////////////////////////////////////////////////////// 
// CPrefixSpanDlg dialog 
#include "Resource.h" 
void compute(void* p); 
class CPrefixSpanDlg : public CDialog 
{ 
// Construction 
public: 
    CButton m_BeginButton; 
    int m_kind; 
    int m_Support;  //連넣똑 
    CListBox m_ResultDisplay; 
    CString filename;   //匡숭츰 
    CPrefixSpanDlg(CWnd* pParent = NULL);   // standard constructor 
 
// Dialog Data 
    //{{AFX_DATA(CPrefixSpanDlg) 
    enum { IDD = IDD_PREFIXSPAN_DIALOG }; 
        // NOTE: the ClassWizard will add data members here 
    //}}AFX_DATA 
 
    // ClassWizard generated virtual function overrides 
    //{{AFX_VIRTUAL(CPrefixSpanDlg) 
    protected: 
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support 
    //}}AFX_VIRTUAL 
 
// Implementation 
protected: 
    HICON m_hIcon; 
 
    // Generated message map functions 
    //{{AFX_MSG(CPrefixSpanDlg) 
    virtual BOOL OnInitDialog(); 
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam); 
    afx_msg void OnPaint(); 
    afx_msg HCURSOR OnQueryDragIcon(); 
    afx_msg void OnOpenFile(); 
    afx_msg void OnSaveResult(); 
    afx_msg void OnBeginMining(); 
    //}}AFX_MSG 
    DECLARE_MESSAGE_MAP() 
}; 
 
//{{AFX_INSERT_LOCATION}} 
// Microsoft Visual C++ will insert additional declarations immediately before the previous line. 
 
#endif // !defined(AFX_PREFIXSPANDLG_H__9AD5218A_2BE2_41DB_B3C4_EB3B14C3F107__INCLUDED_)
