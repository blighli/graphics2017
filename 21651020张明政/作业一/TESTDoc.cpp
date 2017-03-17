// TESTDoc.cpp : implementation of the CTESTDoc class
//

#include "stdafx.h"
#include "TEST.h"

#include "TESTDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTESTDoc

IMPLEMENT_DYNCREATE(CTESTDoc, CDocument)

BEGIN_MESSAGE_MAP(CTESTDoc, CDocument)
	//{{AFX_MSG_MAP(CTESTDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTESTDoc construction/destruction

CTESTDoc::CTESTDoc()
{
	// TODO: add one-time construction code here

}

CTESTDoc::~CTESTDoc()
{
}

BOOL CTESTDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTESTDoc serialization

void CTESTDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTESTDoc diagnostics

#ifdef _DEBUG
void CTESTDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTESTDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTESTDoc commands
