// PrefixSpan.h : main header file for the PREFIXSPAN application 
// 
 
#if !defined(AFX_PREFIXSPAN_H__D8445B22_4FFC_46C2_8D02_E8371159EC1E__INCLUDED_) 
#define AFX_PREFIXSPAN_H__D8445B22_4FFC_46C2_8D02_E8371159EC1E__INCLUDED_ 
 
#if _MSC_VER > 1000 
#pragma once 
#endif // _MSC_VER > 1000 
 
#ifndef __AFXWIN_H__ 
    #error include 'stdafx.h' before including this file for PCH 
#endif 
 
#include "resource.h"       // main symbols 
 
///////////////////////////////////////////////////////////////////////////// 
// CPrefixSpanApp: 
// See PrefixSpan.cpp for the implementation of this class 
// 
 
class CPrefixSpanApp : public CWinApp 
{ 
    public: 
            CPrefixSpanApp(); 
             
            // Overrides 
                // ClassWizard generated virtual function overrides 
                //{{AFX_VIRTUAL(CPrefixSpanApp) 
                public: 
                virtual BOOL InitInstance(); 
                    //}}AFX_VIRTUAL 
                 
                // Implementation 
                 
                    //{{AFX_MSG(CPrefixSpanApp) 
                        // NOTE - the ClassWizard will add and remove member functions here. 
                        //    DO NOT EDIT what you see in these blocks of generated code ! 
                    //}}AFX_MSG 
                    DECLARE_MESSAGE_MAP() 
}; 
 
 
///////////////////////////////////////////////////////////////////////////// 
 
//{{AFX_INSERT_LOCATION}} 
// Microsoft Visual C++ will insert additional declarations immediately before the previous line. 
 
#endif // !defined(AFX_PREFIXSPAN_H__D8445B22_4FFC_46C2_8D02_E8371159EC1E__INCLUDED_) 
