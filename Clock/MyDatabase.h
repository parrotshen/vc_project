#if !defined(AFX_MYDATABASE_H__2244DC69_2865_4203_87DA_66FA7A6C02FC__INCLUDED_)
#define AFX_MYDATABASE_H__2244DC69_2865_4203_87DA_66FA7A6C02FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyDatabase.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyDatabase DAO recordset
#include <afxdao.h>

class CMyDatabase : public CDaoRecordset
{
public:
	CMyDatabase(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CMyDatabase)

// Field/Param Data
	//{{AFX_FIELD(CMyDatabase, CDaoRecordset)
	short	m_Month;
	short	m_Day;
	short	m_Hour;
	short	m_Minute;
	CString	m_Memo;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyDatabase)
	public:
	virtual CString GetDefaultDBName();		// Default database name
	virtual CString GetDefaultSQL();		// Default SQL for Recordset
	virtual void DoFieldExchange(CDaoFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYDATABASE_H__2244DC69_2865_4203_87DA_66FA7A6C02FC__INCLUDED_)
