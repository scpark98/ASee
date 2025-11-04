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
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CTitleDlg 메시지 처리기

BOOL CTitleDlg::OnInitDialog()
{
	CSCThemeDlg::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//m_theme.cr_back = Gdiplus::Color::White;
	set_color_theme(CSCColorTheme::color_theme_dark_gray);
	set_system_buttons(this, SC_MINIMIZE, SC_MAXIMIZE, SC_CLOSE);
	//set_target_wnd(GetParent());
	//set_title_font_bold();
	set_titlebar_font_size(12);
	set_titlebar_height(48);
	set_titlebar_icon(IDR_MAINFRAME, 24, 24);
	set_draw_border(false);

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
	::PostMessage(GetParent()->GetSafeHwnd(), WM_SYSCOMMAND, SC_CLOSE, 0);
}

void CTitleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default
	//CSCSystemButtons에서 전달한 메시지를 실제 적용할 dlg에게 그냥 전달하고
	//실제 적용할 dlg에서 모든 처리를 한다.
	//단, 이 프로그램 특성 상 CTitleDlg라는 중간 매개체를 사용하고 있고
	//maximize든, restore든 일단 이 CTitleDlg는 감춰줘야 한다.
	if (nID == SC_MAXIMIZE)
		ShowWindow(SW_HIDE);

	::PostMessage(GetParent()->GetSafeHwnd(), WM_SYSCOMMAND, nID, lParam);
}

void CTitleDlg::update_title(CString title)
{
	SetWindowText(title);
}

void CTitleDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_sys_buttons.has_button(SC_MAXIMIZE) && (point.y < m_titlebar_height))
	{
		CWnd* parent = GetParent();

		//프로그램 아이콘을 더블클릭한 경우
		if (point.x < 32)
		{
			parent->SendMessage(WM_SYSCOMMAND, SC_CLOSE);
			return;
		}

		//maximize, restore도 캡션바 show/hide 등의 부가적인 처리가 필요하므로
		//이 클래스에서 직접 처리하지말고 CASeeDlg에서 처리한다.
		if (parent->IsZoomed())
		{
			parent->SendMessage(WM_SYSCOMMAND, SC_RESTORE);
		}
		else
		{
			parent->SendMessage(WM_SYSCOMMAND, SW_MAXIMIZE);
		}

		//m_sys_buttons.Invalidate();
	}

	CSCThemeDlg::OnLButtonDblClk(nFlags, point);
}
