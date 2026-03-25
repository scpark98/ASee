// ZigzagColorDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ASee.h"
#include "ASeeDlg.h"
#include "afxdialogex.h"
#include "ZigzagColorDlg.h"

#include "Common/CDialog/CSCColorPicker/SCColorPicker.h"

// CZigzagColorDlg 대화 상자

IMPLEMENT_DYNAMIC(CZigzagColorDlg, CDialogEx)

CZigzagColorDlg::CZigzagColorDlg(CWnd* pParent, Gdiplus::Color cr_back, Gdiplus::Color cr_fore)
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
	DDX_Control(pDX, IDC_RADIO_DEFAULT, m_radio_default);
	DDX_Control(pDX, IDC_RADIO_WHITE, m_radio_white);
	DDX_Control(pDX, IDC_RADIO_BLACK, m_radio_black);
	DDX_Control(pDX, IDC_RADIO_CUSTOM, m_radio_custom);
	DDX_Control(pDX, IDC_STATIC_CR_BACK, m_static_cr_back);
	DDX_Control(pDX, IDC_STATIC_CR_FORE, m_static_cr_fore);
}


BEGIN_MESSAGE_MAP(CZigzagColorDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CZigzagColorDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CZigzagColorDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO_DEFAULT, &CZigzagColorDlg::OnBnClickedRadioDefault)
	ON_BN_CLICKED(IDC_RADIO_WHITE, &CZigzagColorDlg::OnBnClickedRadioWhite)
	ON_BN_CLICKED(IDC_RADIO_BLACK, &CZigzagColorDlg::OnBnClickedRadioBlack)
	ON_BN_CLICKED(IDC_RADIO_CUSTOM, &CZigzagColorDlg::OnBnClickedRadioCustom)
	ON_STN_CLICKED(IDC_STATIC_CR_BACK, &CZigzagColorDlg::OnStnClickedStaticCrBack)
	ON_STN_CLICKED(IDC_STATIC_CR_FORE, &CZigzagColorDlg::OnStnClickedStaticCrFore)
END_MESSAGE_MAP()


// CZigzagColorDlg 메시지 처리기

BOOL CZigzagColorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_cr_fore = theApp.GetProfileInt(_T("setting"), _T("zigzag cr_fore"), gGRAY(200).GetValue());
	m_cr_back = theApp.GetProfileInt(_T("setting"), _T("zigzag cr_back"), gGRAY(255).GetValue());
	m_static_cr_fore.set_back_color(m_cr_fore);
	m_static_cr_back.set_back_color(m_cr_back);
	m_static_cr_fore.set_round(8, Gdiplus::Color::Gray, get_sys_color(COLOR_3DFACE));
	m_static_cr_back.set_round(8, Gdiplus::Color::Gray, get_sys_color(COLOR_3DFACE));


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
	m_cr_back = m_static_cr_back.get_back_color();
	m_cr_fore = m_static_cr_back.get_back_color();
	((CASeeDlg*)(AfxGetApp()->GetMainWnd()))->set_zigzag_color(m_cr_back, m_cr_fore);
	ShowWindow(SW_HIDE);
}

void CZigzagColorDlg::OnBnClickedCancel()
{
	ShowWindow(SW_HIDE);
}

Gdiplus::Color CZigzagColorDlg::get_back_color()
{
	return m_cr_back;
}

Gdiplus::Color CZigzagColorDlg::get_fore_color()
{
	return m_cr_fore;
}

void CZigzagColorDlg::set_back_color(Gdiplus::Color cr_back)
{
	m_cr_back = cr_back;
	m_static_cr_back.set_back_color(m_cr_back);
}

void CZigzagColorDlg::set_fore_color(Gdiplus::Color cr_fore)
{
	m_cr_fore = cr_fore;
	m_static_cr_fore.set_back_color(m_cr_fore);
}

void CZigzagColorDlg::OnBnClickedRadioDefault()
{
	m_static_cr_fore.set_back_color(gGRAY(255));
	m_static_cr_back.set_back_color(gGRAY(200));

	((CASeeDlg*)(AfxGetApp()->GetMainWnd()))->set_zigzag_color(gGRAY(200), gGRAY(255));
	theApp.WriteProfileInt(_T("setting"), _T("zigzag option"), 0);
}

void CZigzagColorDlg::OnBnClickedRadioWhite()
{
	m_static_cr_fore.set_back_color(gGRAY(255));
	m_static_cr_back.set_back_color(gGRAY(255));

	((CASeeDlg*)(AfxGetApp()->GetMainWnd()))->set_zigzag_color(gGRAY(255), gGRAY(255));
	theApp.WriteProfileInt(_T("setting"), _T("zigzag option"), 1);
}

void CZigzagColorDlg::OnBnClickedRadioBlack()
{
	m_static_cr_back.set_back_color(Gdiplus::Color::Black);
	m_static_cr_fore.set_back_color(Gdiplus::Color::Black);

	((CASeeDlg*)(AfxGetApp()->GetMainWnd()))->set_zigzag_color(Gdiplus::Color::Black, Gdiplus::Color::Black);
	theApp.WriteProfileInt(_T("setting"), _T("zigzag option"), 2);
}

void CZigzagColorDlg::OnBnClickedRadioCustom()
{
	//custom을 누르면 registry에 저장된 색상으로 변경한다.
	m_cr_fore = theApp.GetProfileInt(_T("setting"), _T("zigzag cr_fore"), CSCGdiplusBitmap::m_cr_zigzag_fore.GetValue());
	m_cr_back = theApp.GetProfileInt(_T("setting"), _T("zigzag cr_back"), CSCGdiplusBitmap::m_cr_zigzag_back.GetValue());

	m_static_cr_fore.set_back_color(m_cr_fore);
	m_static_cr_back.set_back_color(m_cr_back);

	((CASeeDlg*)(AfxGetApp()->GetMainWnd()))->set_zigzag_color(m_cr_fore, m_cr_back);
	theApp.WriteProfileInt(_T("setting"), _T("zigzag option"), 3);
}

BOOL CZigzagColorDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	TRACE(_T("CZigzagColorDlg::PreTranslateMessage. pMsg->message = %d\n"), pMsg->message);
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CZigzagColorDlg::OnStnClickedStaticCrFore()
{
	CSCColorPicker	dlg;
	if (dlg.DoModal(this, m_cr_fore, _T("Transparent Pattern Fore Color")) == IDCANCEL)
		return;

	m_radio_default.SetCheck(BST_UNCHECKED);
	m_radio_white.SetCheck(BST_UNCHECKED);
	m_radio_black.SetCheck(BST_UNCHECKED);
	m_radio_custom.SetCheck(BST_CHECKED);

	m_cr_fore = dlg.get_selected_color();
	m_static_cr_fore.set_back_color(m_cr_fore);
	m_static_cr_fore.set_text_color(get_distinct_bw_color(m_cr_fore));

	theApp.WriteProfileInt(_T("setting"), _T("zigzag cr_fore"), m_cr_fore.GetValue());
	theApp.WriteProfileInt(_T("setting"), _T("zigzag option"), 3);
	((CASeeDlg*)(AfxGetApp()->GetMainWnd()))->set_zigzag_color(m_cr_back, m_cr_fore);
}

void CZigzagColorDlg::OnStnClickedStaticCrBack()
{
	CSCColorPicker	dlg;
	if (dlg.DoModal(this, m_cr_back, _T("Transparent Pattern Back Color")) == IDCANCEL)
		return;

	//custom color를 선택하면 Custom으로 선택상태를 바꾸고
	m_radio_default.SetCheck(BST_UNCHECKED);
	m_radio_white.SetCheck(BST_UNCHECKED);
	m_radio_black.SetCheck(BST_UNCHECKED);
	m_radio_custom.SetCheck(BST_CHECKED);

	m_cr_back = dlg.get_selected_color();
	m_static_cr_back.set_back_color(m_cr_back);
	m_static_cr_back.set_text_color(get_distinct_bw_color(m_cr_back));

	theApp.WriteProfileInt(_T("setting"), _T("zigzag cr_back"), m_cr_back.GetValue());
	theApp.WriteProfileInt(_T("setting"), _T("zigzag option"), 3);
	((CASeeDlg*)(AfxGetApp()->GetMainWnd()))->set_zigzag_color(m_cr_back, m_cr_fore);
}
