// BMOfferCalcMotorParamTabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMOfferCalcMotorParamTabDlg.h"


// CBMOfferCalcMotorParamTabDlg dialog

IMPLEMENT_DYNAMIC(CBMOfferCalcMotorParamTabDlg, CDialog)

CBMOfferCalcMotorParamTabDlg::CBMOfferCalcMotorParamTabDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMOfferCalcMotorParamTabDlg::IDD, pParent)
{
	for (int i = 0; i < 4; i++)
	{
		m_pParamTableImages[i] = NULL;
	}
}

CBMOfferCalcMotorParamTabDlg::~CBMOfferCalcMotorParamTabDlg()
{
}

void CBMOfferCalcMotorParamTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBMOfferCalcMotorParamTabDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CBMOfferCalcMotorParamTabDlg message handlers

BOOL CBMOfferCalcMotorParamTabDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	for (int i = 0; i < 4; i++)
	{
		m_pParamTableImages[i] = new CImage();
	}
	m_pParamTableImages[0]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_MOTOR_PARAM1);
	m_pParamTableImages[1]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_MOTOR_PARAM2);
	m_pParamTableImages[2]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_MOTOR_PARAM3);
	m_pParamTableImages[3]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_MOTOR_PARAM4);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBMOfferCalcMotorParamTabDlg::OnDestroy()
{
	CDialog::OnDestroy();

	for (int i = 0; i < 4; i++)
	{
		if (NULL != m_pParamTableImages[i])
		{
			m_pParamTableImages[i]->Destroy();
			delete m_pParamTableImages[i];
			m_pParamTableImages[i] = NULL;
		}
	}
}

void CBMOfferCalcMotorParamTabDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rcClient;
	GetClientRect(&rcClient);	
	if (NULL != m_pParamTableImages[m_iCurParamTableIndex])
	{
		CSize sizeImage(m_pParamTableImages[m_iCurParamTableIndex]->GetWidth(), m_pParamTableImages[m_iCurParamTableIndex]->GetHeight());
		m_pParamTableImages[m_iCurParamTableIndex]->StretchBlt(	dc.m_hDC,	
																(rcClient.Width()-sizeImage.cx)/2, 
																10, 
																sizeImage.cx, 
																sizeImage.cy);
	}
}

void CBMOfferCalcMotorParamTabDlg::SetCurParamTableIndex(int iIndex)
{
	m_iCurParamTableIndex = iIndex;
	Invalidate();
}
