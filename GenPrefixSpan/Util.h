FX_UTIL_H__1795C042_A03A_48B0_B35C_75B0AA208CF2__INCLUDED_ 
 
#if _MSC_VER > 1000 
#pragma once 
#endif // _MSC_VER > 1000 
#include   <comdef.h> 
class CUtil   
{ 
    public: 
            static CString OpenFileDlg(BOOL open=TRUE); //茄君댔역샀괏닸匡숭뚤뺐움칵훰槨댔역匡숭，럿쀼供憐돨匡숭쨌쓺츰 
                CUtil(); 
                    virtual ~CUtil(); 
                        static CString intToString(int n); 
                            static void FormatDouble(double dblValue,CString& sOut); 
                                static CString FormatDouble(double dblValue); 
                                    static CString longToString(LONG ); 
}; 
 
 
#endif // !defined(AFX_UTIL_H__1795C042_A03A_48B0_B35C_75B0AA208CF2__INCLUDED_) 
