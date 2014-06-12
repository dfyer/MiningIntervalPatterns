// PrefixSpanDlg.cpp : implementation file   
//   
   
#include "stdafx.h"   
#include "PrefixSpan.h"   
#include "PrefixSpanDlg.h"   
//#include "Prefix.h"   
#include "GenPrefix.h"   
#include <process.h>   
   
#ifdef _DEBUG   
#define new DEBUG_NEW   
#undef THIS_FILE   
static char THIS_FILE[] = __FILE__;   
#endif   
   
/////////////////////////////////////////////////////////////////////////////   
// CAboutDlg dialog used for App About   
bool IsFinished=true;   
class CAboutDlg : public CDialog   
{   
public:   
    CAboutDlg();   
   
// Dialog Data   
    //{{AFX_DATA(CAboutDlg)   
    enum { IDD = IDD_ABOUTBOX };   
    //}}AFX_DATA   
   
    // ClassWizard generated virtual function overrides   
    //{{AFX_VIRTUAL(CAboutDlg)   
    protected:   
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support   
    //}}AFX_VIRTUAL   
   
// Implementation   
protected:   
    //{{AFX_MSG(CAboutDlg)   
    //}}AFX_MSG   
    DECLARE_MESSAGE_MAP()   
};   
   
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)   
{   
    //{{AFX_DATA_INIT(CAboutDlg)   
    //}}AFX_DATA_INIT   
}   
   
void CAboutDlg::DoDataExchange(CDataExchange* pDX)   
{   
    CDialog::DoDataExchange(pDX);   
    //{{AFX_DATA_MAP(CAboutDlg)   
    //}}AFX_DATA_MAP   
}   
   
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)   
    //{{AFX_MSG_MAP(CAboutDlg)   
        // No message handlers   
    //}}AFX_MSG_MAP   
END_MESSAGE_MAP()   
   
/////////////////////////////////////////////////////////////////////////////   
// CPrefixSpanDlg dialog   
   
CPrefixSpanDlg::CPrefixSpanDlg(CWnd* pParent /*=NULL*/)   
    : CDialog(CPrefixSpanDlg::IDD, pParent)   
{   
    //{{AFX_DATA_INIT(CPrefixSpanDlg)   
        // NOTE: the ClassWizard will add member initialization here   
    //}}AFX_DATA_INIT   
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32   
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);   
    m_kind=0;   
    m_Support=0;   
}   
   
void CPrefixSpanDlg::DoDataExchange(CDataExchange* pDX)   
{   
    CDialog::DoDataExchange(pDX);   
    DDX_Control(pDX, IDC_LIST1, m_ResultDisplay);   
    DDX_Text(pDX, IDC_EDIT3, m_Support);   
    DDX_Radio(pDX,IDC_RADIO1 ,m_kind);   
    DDX_Control(pDX, IDC_BUTTON3, m_BeginButton);   
       
//  DDX_Radio(pDX,IDC_RADIO1,m_kind);   
    //{{AFX_DATA_MAP(CPrefixSpanDlg)   
        // NOTE: the ClassWizard will add DDX and DDV calls here   
    //}}AFX_DATA_MAP   
}   
   
BEGIN_MESSAGE_MAP(CPrefixSpanDlg, CDialog)   
    //{{AFX_MSG_MAP(CPrefixSpanDlg)   
    ON_WM_SYSCOMMAND()   
    ON_WM_PAINT()   
    ON_WM_QUERYDRAGICON()   
    ON_BN_CLICKED(IDC_BUTTON1, OnOpenFile)   
    ON_BN_CLICKED(IDC_BUTTON2, OnSaveResult)   
    ON_BN_CLICKED(IDC_BUTTON3, OnBeginMining)   
    //}}AFX_MSG_MAP   
END_MESSAGE_MAP()   
   
/////////////////////////////////////////////////////////////////////////////   
// CPrefixSpanDlg message handlers   
   
BOOL CPrefixSpanDlg::OnInitDialog()   
{   
    CDialog::OnInitDialog();   
   
    // Add "About..." menu item to system menu.   
   
    // IDM_ABOUTBOX must be in the system command range.   
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);   
    ASSERT(IDM_ABOUTBOX < 0xF000);   
   
    CMenu* pSysMenu = GetSystemMenu(FALSE);   
    if (pSysMenu != NULL)   
    {   
        CString strAboutMenu;   
        strAboutMenu.LoadString(IDS_ABOUTBOX);   
        if (!strAboutMenu.IsEmpty())   
        {   
            pSysMenu->AppendMenu(MF_SEPARATOR);   
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);   
        }   
    }   
   
    // Set the icon for this dialog.  The framework does this automatically   
    //  when the application's main window is not a dialog   
    SetIcon(m_hIcon, TRUE);         // Set big icon   
    SetIcon(m_hIcon, FALSE);        // Set small icon   
       
    // TODO: Add extra initialization here   
       
    return TRUE;  // return TRUE  unless you set the focus to a control   
}   
   
void CPrefixSpanDlg::OnSysCommand(UINT nID, LPARAM lParam)   
{   
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)   
    {   
        CAboutDlg dlgAbout;   
        dlgAbout.DoModal();   
    }   
    else   
    {   
        CDialog::OnSysCommand(nID, lParam);   
    }   
}   
   
// If you add a minimize button to your dialog, you will need the code below   
//  to draw the icon.  For MFC applications using the document/view model,   
//  this is automatically done for you by the framework.   
   
void CPrefixSpanDlg::OnPaint()    
{   
    if (IsIconic())   
    {   
        CPaintDC dc(this); // device context for painting   
   
        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);   
   
        // Center icon in client rectangle   
        int cxIcon = GetSystemMetrics(SM_CXICON);   
        int cyIcon = GetSystemMetrics(SM_CYICON);   
        CRect rect;   
        GetClientRect(&rect);   
        int x = (rect.Width() - cxIcon + 1) / 2;   
        int y = (rect.Height() - cyIcon + 1) / 2;   
   
        // Draw the icon   
        dc.DrawIcon(x, y, m_hIcon);   
    }   
    else   
    {   
        CDialog::OnPaint();   
    }   
}   
   
// The system calls this to obtain the cursor to display while the user drags   
//  the minimized window.   
HCURSOR CPrefixSpanDlg::OnQueryDragIcon()   
{   
    return (HCURSOR) m_hIcon;   
}   
   
void CPrefixSpanDlg::OnOpenFile()    
{   
    // TODO: Add your control notification handler code here   
    filename=CUtil::OpenFileDlg(TRUE);   
    ((CEdit*)GetDlgItem(IDC_EDIT2))->SetWindowText(filename);   
}   
   
void CPrefixSpanDlg::OnSaveResult()    
{   
    // TODO: Add your control notification handler code here   
    CString fileName=CUtil::OpenFileDlg(FALSE);   
    if(fileName=="")   
    {   
        return;   
    }   
    if(fileName.Find(".txt", 0)==-1)   
    {   
        fileName+=".txt";   
    }   
    CFile file(fileName, CFile::modeReadWrite|CFile::modeCreate);   
    int count=m_ResultDisplay.GetCount();   
    CString str;   
    for(int i=0; i<count; i++)   
    {   
        m_ResultDisplay.GetText(i, str);   
        str+="\n";   
        file.Write(str, str.GetLength());   
    }   
    file.Close();   
}   
   
void CPrefixSpanDlg::OnBeginMining()    
{   
    // TODO: Add your control notification handler code here   
    UpdateData();   
       
//  CPrefixSpanDlg* _this=(CPrefixSpanDlg*)p;   
    if(!IsFinished)   
    {   
        IsFinished=true;   
        MessageBox("   界岺셕炬!   ","Prefix",MB_ICONINFORMATION);   
        this->m_BeginButton.SetWindowText("역迦셕炬");   
    }   
    else   
    {   
        IsFinished=false;   
        UpdateData(1);   
        this->m_ResultDisplay.ResetContent();   
        this->m_BeginButton.SetWindowText("界岺셕炬");   
   
        _beginthread(compute, 0, (void *)this);   
        UpdateData(0);   
    }   
}   
   
void compute(void* p)   
{   
    CPrefixSpanDlg* _this=(CPrefixSpanDlg*)p;   
    if(_this->m_Support==0)   
    {   
        IsFinished=true;   
        _this->m_BeginButton.SetWindowText("역迦셕炬");   
        return ;   
    }   
/*  int time=1; //獵契늴鑒  
    CString outDir="D:\\왁넋\\DM\\鑒앴섞\\result\\";  
    ofstream resultFile(outDir+"result.txt", ios::out);  
    double t=0;  
//  int support[]={500, 450, 400, 350, 300, 250, 200, 150, 100, 50, 40,30, 20, 10};  
    int support[]={10,50};  
    for(int i=0; i<2; i++)  
    {  
        t=0;  
        resultFile<<"Support= "<<support[i]<<endl;  
        for(int j=0; j<time; j++)  
        {  
//          if(j<4 && support[i]==20)  
//              continue;  
            ofstream out(outDir+"result"+CUtil::intToString(support[i])+"-"+CUtil::intToString(j+1)+".txt", ios::out);  
//          CGenPrefix prefixSpan(_this->m_Support ,_this->m_kind, out,  _this);  prefixSpan.SetGap(0);  
            CGenPrefix prefixSpan(support[i] ,_this->m_kind, out);   prefixSpan.SetGap(0);  
  
//          Prefix prefixSpan(_this->m_Support,_this->m_kind,_this);  
            string file=string(_this->filename);  
            prefixSpan.initialize(file);  
            prefixSpan.mine();  
            resultFile<<prefixSpan.GetRunTime()<<"\t";  
            t+=prefixSpan.GetRunTime();  
            out.close();  
        }  
        resultFile<<endl<<"average: "<<t/time<<endl;  
    }  
    resultFile.close();  
*/   
   
    CGenPrefix prefixSpan(_this->m_Support ,_this->m_kind,  _this);   prefixSpan.SetGap(0);   
//  prefixSpan.SetGap(1);   
//  Prefix prefixSpan(_this->m_Support,_this->m_kind,_this);   
    string file=string(_this->filename);   
    prefixSpan.initialize(file);   
    prefixSpan.mine();   
   
    if(!IsFinished)   
    {   
        _this->MessageBox("  供냥셕炬！  ","Prefix",MB_ICONINFORMATION);   
        IsFinished=true;   
    }   
   
    _this->m_BeginButton.SetWindowText("역迦셕炬");   
}   
