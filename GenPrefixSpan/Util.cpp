
// Util.cpp: implementation of the CUtil class.   
//   
//////////////////////////////////////////////////////////////////////   
   
#include "stdafx.h"   
#include "Util.h"   
   
#ifdef _DEBUG   
#undef THIS_FILE   
static char THIS_FILE[]=__FILE__;   
#define new DEBUG_NEW   
#endif   
   
//////////////////////////////////////////////////////////////////////   
// Construction/Destruction   
//////////////////////////////////////////////////////////////////////   
   
CUtil::CUtil()   
{   
   
}   
   
CUtil::~CUtil()   
{   
   
}   
CString CUtil::intToString(int n)   
{   
    CString str;   
    str.Format("%d", n);   
    return str;   
}   
void CUtil::FormatDouble(double dblValue,CString& sOut)   
{   
    _variant_t var(dblValue);   
    BSTR bstrOut = sOut.AllocSysString();   
    ::VarFormat(&var, L"0.##", 0, 0, VAR_FORMAT_NOSUBSTITUTE, &bstrOut);   
    sOut = bstrOut;   
    ::SysFreeString(bstrOut);   
}   
CString CUtil::FormatDouble(double dblValue)   
{   
    CString str;   
    FormatDouble(dblValue, str);   
    return str;   
}   
   
CString CUtil::longToString(LONG l)   
{   
    CString str1="",str="";   
    while(l)   
    {   
        int temp=l%10;   
        str=CUtil::intToString(temp);   
        str=str+str1;   
        str1=str;   
        l=l/10;   
    }   
    return str;   
/*  
    for(int i=3;i>=0;i--)  
    {  
        LONG temp = pow(256,i);  
        if(i>0)  
           str.Format("%d.",l/temp);  
       else  
           str.Format("%d",l/temp);  
       str1 = str1 + str;  
       l = l % temp;  
    }  
    return str1;  
*/   
}   
   
CString CUtil::OpenFileDlg(BOOL open)   
{   
    CFileDialog dlgFileOpen(open,NULL,NULL,OFN_HIDEREADONLY,_T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));    
    int structsize=0;    
    DWORD dwVersion,dwWindowsMajorVersion,dwWindowsMinorVersion;    
    //쇱꿎커품돨꾸鱗溝固，GetVersion야竟痰랬圈숨MSDN    
    dwVersion = GetVersion();    
    dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));    
    dwWindowsMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));    
   
    // 흔벎頓契돨꾸鱗溝固角Windows NT/2000    
    if (dwVersion < 0x80000000)     
        structsize =88;//鞫刻劤돨匡숭뚤뺐움    
    else    
    //頓契돨꾸鱗溝固Windows 95/98     
        structsize =76;//鞫刻일돨匡숭뚤뺐움    
   
    dlgFileOpen.m_ofn.lStructSize=structsize;    
    TCHAR lpstrFilename[MAX_PATH] = "";    
    dlgFileOpen.m_ofn.lpstrFile=lpstrFilename;    
//  dlgFileOpen.SetDefExt(".txt");   
    if(dlgFileOpen.DoModal()==IDOK)   
    {   
        return (CString)dlgFileOpen.m_ofn.lpstrFile;   
    }   
    else   
        return "";   
}   
