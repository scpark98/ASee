// ZigzagColorDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ASee.h"
#include "ASeeDlg.h"
#include "afxdialogex.h"
#include "ZigzagColorDlg.h"


// CZigzagColorDlg 대화 상자

IMPLEMENT_DYNAMIC(CZigzagColorDlg, CDialogEx)

CZigzagColorDlg::CZigzagColorDlg(CWnd* pParent, COLORREF cr_back, COLORREF cr_fore)
	: CDialogEx(IDD_ZIGZAG_COLOR, pParent)
{
	m_cr_back = cr_back;
	m_cr_fore = cr_fore;
}

CZigzagColorDlg::~CZigzagColorDlg()
{
}

void CZigzagColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_BACK_COLOR, m_button_back_color);
	DDX_Control(pDX, IDC_BUTTON_FORE_COLOR, m_button_fore_color);
	DDX_Control(pDX, IDC_RADIO_DEFAULT, m_radio_default);
	DDX_Control(pDX, IDC_RADIO_WHITE, m_radio_white);
	DDX_Control(pDX, IDC_RADIO_BLACK, m_radio_black);
	DDX_Control(pDX, IDC_RADIO_CUSTOM, m_radio_custom);
}


BEGIN_MESSAGE_MAP(CZigzagColorDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CZigzagColorDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CZigzagColorDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_BACK_COLOR, &CZigzagColorDlg::OnBnClickedButtonBackColor)
	ON_BN_CLICKED(IDC_BUTTON_FORE_COLOR, &CZigzagColorDlg::OnBnClickedButtonForeColor)
	ON_BN_CLICKED(IDC_RADIO_DEFAULT, &CZigzagColorDlg::OnBnClickedRadioDefault)
	ON_BN_CLICKED(IDC_RADIO_WHITE, &CZigzagColorDlg::OnBnClickedRadioWhite)
	ON_BN_CLICKED(IDC_RADIO_BLACK, &CZigzagColorDlg::OnBnClickedRadioBlack)
	ON_BN_CLICKED(IDC_RADIO_CUSTOM, &CZigzagColorDlg::OnBnClickedRadioCustom)
END_MESSAGE_MAP()


// CZigzagColorDlg 메시지 처리기

BOOL CZigzagColorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_cr_back = theApp.GetProfileInt(_T("setting"), _T("zigzag cr_back"), CGdiplusBitmap::m_cr_zigzag_back.ToCOLORREF());
	m_cr_fore = theApp.GetProfileInt(_T("setting"), _T("zigzag cr_fore"), CGdiplusBitmap::m_cr_zigzag_fore.ToCOLORREF());
	m_button_back_color.SetColor(m_cr_back);
	m_button_fore_color.SetColor(m_cr_fore);

	int option = theApp.GetProfileInt(_T("setting"), _T("zigzag option"), 3);
	switch (option)
	{
		case 0 :
			CheckDlgButton(IDC_RADIO_DEFAULT, BST_CHECKED);
			OnBnClickedRadioDefault();
			break;
		case 1:
			CheckDlgButton(IDC_RADIO_WHITE, BST_CHECKED);
			OnBnClickedRadioWhite();
			break;
		case 2:
			CheckDlgButton(IDC_RADIO_BLACK, BST_CHECKED);
			OnBnClickedRadioBlack();
			break;
		case 3:
			CheckDlgButton(IDC_RADIO_CUSTOM, BST_CHECKED);
			OnBnClickedRadioCustom();
			break;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CZigzagColorDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_cr_back = m_button_back_color.GetColor();
	m_cr_fore = m_button_fore_color.GetColor();
	((CASeeDlg*)(AfxGetApp()->GetMainWnd()))->set_zigzag_color(m_cr_back, m_cr_fore);
	ShowWindow(SW_HIDE);
}

void CZigzagColorDlg::OnBnClickedCancel()
{
	ShowWindow(SW_HIDE);
}

void CZigzagColorDlg::OnBnClickedButtonBackColor()
{
	m_cr_back = m_button_back_color.GetColor();
	((CASeeDlg*)(AfxGetApp()->GetMainWnd()))->set_zigzag_color(m_cr_back, m_cr_fore);
}

void CZigzagColorDlg::OnBnClickedButtonForeColor()
{
	m_cr_fore = m_button_fore_color.GetColor();
	((CASeeDlg*)(AfxGetApp()->GetMainWnd()))->set_zigzag_color(m_cr_back, m_cr_fore);
}

Gdiplus::Color CZigzagColorDlg::get_back_color()
{
	Gdiplus::Color cr;
	cr.SetFromCOLORREF(m_cr_back);
	return cr;
}

Gdiplus::Color CZigzagColorDlg::get_fore_color()
{
	Gdiplus::Color cr;
	cr.SetFromCOLORREF(m_cr_fore);
	return cr;
}

void CZigzagColorDlg::set_back_color(COLORREF cr_back)
{
	m_cr_back = cr_back;
	m_button_back_color.SetColor(m_cr_back);
}

void CZigzagColorDlg::set_fore_color(COLORREF cr_fore)
{
	m_cr_fore = cr_fore;
	m_button_fore_color.SetColor(m_cr_fore);
}

void CZigzagColorDlg::OnBnClickedRadioDefault()
{
	m_cr_back = RGB(255, 255, 255);
	m_cr_fore = RGB(200, 200, 200);
	m_button_back_color.EnableWindow(false);
	m_button_fore_color.EnableWindow(false);
	((CASeeDlg*)(AfxGetApp()->GetMainWnd()))->set_zigzag_color(m_cr_back, m_cr_fore);
	theApp.WriteProfileInt(_T("setting"), _T("zigzag option"), 0);
}

void CZigzagColorDlg::OnBnClickedRadioWhite()
{
	m_cr_back = RGB(255, 255, 255);
	m_cr_fore = RGB(255, 255, 255);
	m_button_back_color.EnableWindow(false);
	m_button_fore_color.EnableWindow(false);
	((CASeeDlg*)(AfxGetApp()->GetMainWnd()))->set_zigzag_color(m_cr_back, m_cr_fore);
	theApp.WriteProfileInt(_T("setting"), _T("zigzag option"), 1);
}

void CZigzagColorDlg::OnBnClickedRadioBlack()
{
	m_cr_back = RGB(0, 0, 0);
	m_cr_fore = RGB(0, 0, 0);
	m_button_back_color.EnableWindow(false);
	m_button_fore_color.EnableWindow(false);
	((CASeeDlg*)(AfxGetApp()->GetMainWnd()))->set_zigzag_color(m_cr_back, m_cr_fore);
	theApp.WriteProfileInt(_T("setting"), _T("zigzag option"), 2);
}

void CZigzagColorDlg::OnBnClickedRadioCustom()
{
	m_cr_back = m_button_back_color.GetColor();
	m_cr_fore = m_button_fore_color.GetColor();
	m_button_back_color.EnableWindow(true);
	m_button_fore_color.EnableWindow(true);
	((CASeeDlg*)(AfxGetApp()->GetMainWnd()))->set_zigzag_color(m_cr_back, m_cr_fore);
	theApp.WriteProfileInt(_T("setting"), _T("zigzag option"), 3);
}
