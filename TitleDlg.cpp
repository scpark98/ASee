// TitleDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ASee.h"
#include "afxdialogex.h"
#include "TitleDlg.h"


// CTitleDlg 대화 상자

IMPLEMENT_DYNAMIC(CTitleDlg, CSCThemeDlg)

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
	//ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CTitleDlg 메시지 처리기

BOOL CTitleDlg::OnInitDialog()
{
	CSCThemeDlg::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_layered.AddLayeredStyle(m_hWnd);

	//m_theme.cr_back = Gdiplus::Color::White;
	set_color_theme(CSCColorTheme::color_theme_dark_gray);
	set_system_buttons(this, SC_MINIMIZE, SC_MAXIMIZE, SC_CLOSE);
	//set_target_wnd(GetParent());
	//set_title_font_bold();
	set_titlebar_font_size(9);
	//set_titlebar_height(32);
	set_titlebar_icon(IDR_MAINFRAME, 16, 16);
	set_draw_border(false);

	//SetTimer(timer_debug_info, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CTitleDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN ||
		pMsg->message == WM_KEYUP ||
		pMsg->message == WM_NCHITTEST)
	{
		//TRACE(_T("msg(%d), wParam = %d, on CTitleDlg\n"), pMsg->message, pMsg->wParam);
		return FALSE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CTitleDlg::OnBnClickedOk()
{
}

void CTitleDlg::OnBnClickedCancel()
{
	::PostMessage(m_parent->m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
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

	::PostMessage(m_parent->m_hWnd, WM_SYSCOMMAND, nID, lParam);
}

void CTitleDlg::update_title(CString title)
{
	SetWindowText(title);
	Invalidate();
}

//CSCThemeDlg에서 타이틀바를 더블클릭하면 대상 윈도우를 maximize 또는 restore 시켜야 하는데
//그 대상은 현재 윈도우가 될 수도 있고 현재 윈도우의 parent가 될 수도 있다.
//따라서 CSCThemeDlg에 이 핸들러를 정의할 수 없다.
//하지만 매번 타깃 윈도우가 다를 경우 이러한 처리를 추가하는 것도 번거로우므로
//타깃 윈도우를 지정해서	그 타깃 윈도우에 메시지를 보내도록 하는 방법도 있다.
//타깃 윈도우에 SC_MAXIMIZE를 보내면 타깃 윈도우에서 상황에 맞게 maximize 또는 restore로 처리되어야 한다.
void CTitleDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_sys_buttons.has_button(SC_MAXIMIZE) && (point.y < m_titlebar_height))
	{
		//실제 maximize or restore를 수행할 윈도우가 이 윈도우라면 target = this지만
		//이 프로젝트에서는 CTitleDlg가 아니라 CASeeDlg이므로 target = GetParent()가 된다.
		//프로그램 아이콘을 더블클릭한 경우
		if (point.x < 32)
		{
			m_parent->SendMessage(WM_SYSCOMMAND, SC_CLOSE);
			return;
		}

		//maximize, restore도 캡션바 show/hide 등의 부가적인 처리가 필요하므로
		//이 클래스에서 직접 처리하지말고 CASeeDlg에서 처리해야 한다.
		m_parent->SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
	}

	CDialogEx::OnLButtonDblClk(nFlags, point);
	//CSCThemeDlg::OnLButtonDblClk(nFlags, point);
}

int CTitleDlg::get_titlebar_height()
{
	return CSCThemeDlg::get_titlebar_height();
}

int CTitleDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CSCThemeDlg::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_parent = AfxGetApp()->GetMainWnd();// AfxGetMainWnd();

	return 0;
}

void CTitleDlg::fade_in(bool show)
{
	KillTimer(timer_fade_in);

	m_fade_in = show;

	if (m_fade_in)
	{
		SetParent(nullptr);
		m_layered.AddLayeredStyle(m_hWnd);

		m_alpha = 0;
		m_layered.SetTransparent(m_hWnd, m_alpha);

		ShowWindow(SW_SHOW);
	}
	else
	{
		//m_layered.AddLayeredStyle(m_hWnd);

		m_alpha = 255;
		m_layered.SetTransparent(m_hWnd, m_alpha);

		ShowWindow(SW_SHOW);
	}

	m_in_fade_in = true;
	SetTimer(timer_fade_in, 1, NULL);
}
void CTitleDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == timer_fade_in)
	{
		if (m_fade_in)
		{
			m_alpha += 25;
			if (m_alpha >= 255)
			{
				KillTimer(timer_fade_in);
				m_alpha = 255;
				m_in_fade_in = false;
				SetForegroundWindowForce(m_hWnd, true);
			}
			m_layered.SetTransparent(m_hWnd, m_alpha);
		}
		else
		{
			m_alpha -= 25;
			if (m_alpha <= 0)
			{
				KillTimer(timer_fade_in);
				m_alpha = 0;
				m_in_fade_in = false;
			}
			m_layered.SetTransparent(m_hWnd, m_alpha);
		}

		//TRACE(_T("m_alpha = %d\n"), m_alpha);
		ShowWindow(SW_SHOW);

		//처음엔 sliding 방식으로 구현했으나 CSCThemeDlg의 특성 때문인지 direct2d 때문인지
		//상단의 깜빡임이 계속 발생하여 fade in/out 효과로 변경함.
		//SetWindowPos(NULL, m_cur_pt.x, m_cur_pt.y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
	}
	else if (timer_debug_info)
	{
		static DWORD last_trace = 0;
		DWORD now = GetTickCount();
		if (now - last_trace > 500)  // 0.5초마다 한 번
		{
			TRACE(_T("=== TitleDlg State ===\n"));
			TRACE(_T("  Visible: %d\n"), IsWindowVisible());
			TRACE(_T("  Alpha: %d\n"), m_alpha);
			TRACE(_T("  is_in_fade_in: %d\n"), is_in_fade_in());
			TRACE(_T("  parent = %p\n"), GetParent());
			CRect rw;
			GetWindowRect(rw);
			trace(rw);
			//TRACE(_T("  Mouse Y: %d, Threshold: %d\n"), point.y, m_titleDlg.get_titlebar_height() + 10);
			//TRACE(_T("  IsZoomed: %d\n"), IsZoomed());
			last_trace = now;
		}
	}

	CSCThemeDlg::OnTimer(nIDEvent);
}

//원래 CSCThemeDlg에 정의되어 있었으나 이 프로젝트에서는 별도로 fade in/out 처리가 들어가서
//부가적인 처리를 위해 함수를 override 함.
void CTitleDlg::set_titlebar_movable(bool movable)
{
	//m_layered.AddLayeredStyle(m_hWnd);

	if (movable)
	{
		//KillTimer(timer_fade_in);
		m_alpha = 255;
	}
	else
	{
		m_alpha = 0;
	}

	m_layered.SetTransparent(m_hWnd, m_alpha);

	CSCThemeDlg::set_titlebar_movable(movable);
}

void CTitleDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CSCThemeDlg::OnMouseMove(nFlags, point);
}
