// MyDatabase.cpp : implementation file
//

#include "stdafx.h"
#include "Clock.h"
#include "MyDatabase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyDatabase

IMPLEMENT_DYNAMIC(CMyDatabase, CDaoRecordset)

CMyDatabase::CMyDatabase(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CMyDatabase)
	m_Month = 0;
	m_Day = 0;
	m_Hour = 0;
	m_Minute = 0;
	m_Memo = _T("");
	m_nFields = 5;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString CMyDatabase::GetDefaultDBName()
{
	return _T("Clock.mdb");
}

CString CMyDatabase::GetDefaultSQL()
{
	return _T("[Schedule]");
}

void CMyDatabase::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CMyDatabase)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Short(pFX, _T("[Month]"), m_Month);
	DFX_Short(pFX, _T("[Day]"), m_Day);
	DFX_Short(pFX, _T("[Hour]"), m_Hour);
	DFX_Short(pFX, _T("[Minute]"), m_Minute);
	DFX_Text(pFX, _T("[Memo]"), m_Memo);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CMyDatabase diagnostics

#ifdef _DEBUG
void CMyDatabase::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void CMyDatabase::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
