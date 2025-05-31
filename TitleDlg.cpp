// TitleDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ASee.h"
#include "afxdialogex.h"
#include "TitleDlg.h"


// CTitleDlg 대화 상자

IMPLEMENT_DYNAMIC(CTitleDlg, CDialogEx)

CTitleDlg::CTitleDlg(CWnd* pParent /*=nullptr*/)
	: CSCThemeDlg(IDD_TITLE, pParent)
{

}

CTitleDlg::~CTitleDlg()
{
}

void CTitleDlg::DoDataExchange(CDataExchange* pDX)
{
	CSCThemeDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTitleDlg, CSCThemeDlg)
	ON_BN_CLICKED(IDOK, &CTitleDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTitleDlg::OnBnClickedCancel)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CTitleDlg 메시지 처리기

BOOL CTitleDlg::OnInitDialog()
{
	CSCThemeDlg::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//m_theme.cr_back = Gdiplus::Color::White;
	set_color_theme(CSCColorTheme::color_theme_dark_gray);
	set_system_buttons(SC_MINIMIZE, SC_MAXIMIZE, SC_CLOSE);
	//set_title_font_bold();
	set_titlebar_font_size(14);
	set_titlebar_height(32);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CTitleDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CTitleDlg::OnBnClickedOk()
{
}

void CTitleDlg::OnBnClickedCancel()
{
}

void CTitleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default
	if (nID == SC_MAXIMIZE)
	{
		if (GetParent()->IsZoomed())
			::PostMessage(GetParent()->GetSafeHwnd(), WM_SYSCOMMAND, SC_RESTORE, lParam);
		else
			::PostMessage(GetParent()->GetSafeHwnd(), WM_SYSCOMMAND, SC_MAXIMIZE, lParam);
	}
	else
	{
		::PostMessage(GetParent()->GetSafeHwnd(), WM_SYSCOMMAND, nID, lParam);
	}
}

void CTitleDlg::update_title(CString title)
{
	SetWindowText(_T("  ") + title);
}
