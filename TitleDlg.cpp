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

//CSCThemeDlg에서 타이틀바를 더블클릭하면 maximize 또는 restore 시켜야 하는데
//그 대상은 현재 윈도우가 될 수도 있고 현재 윈도우의 parent가 될 수도 있다.
//따라서 CSCThemeDlg에 이 핸들러를 정의할 수 없다.
//타깃 윈도우에 SC_MAXIMIZE를 보내면 타깃 윈도우에서 상황에 맞게 maximize 또는 restore로 처리되어야 한다.
void CTitleDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_sys_buttons.has_button(SC_MAXIMIZE) && (point.y < m_titlebar_height))
	{
		//실제 maximize or restore를 수행할 윈도우가 이 윈도우라면 target = this지만
		//이 프로젝트에서는 CTitleDlg가 아니라 CASeeDlg이므로 target = GetParent()가 된다.
		CWnd* target = GetParent();

		//프로그램 아이콘을 더블클릭한 경우
		if (point.x < 32)
		{
			target->SendMessage(WM_SYSCOMMAND, SC_CLOSE);
			return;
		}

		//maximize, restore도 캡션바 show/hide 등의 부가적인 처리가 필요하므로
		//이 클래스에서 직접 처리하지말고 CASeeDlg에서 처리해야 한다.
		target->SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
	}

	CSCThemeDlg::OnLButtonDblClk(nFlags, point);
}
