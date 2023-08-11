#pragma once


// CMyEdit1

class CMyEdit1 : public CEdit
{
	DECLARE_DYNAMIC(CMyEdit1)

public:
	CMyEdit1();
	virtual ~CMyEdit1();
private:
	int		m_iCurLineCnt;
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnChange();
};


