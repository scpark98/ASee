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

static bool ApplyLayeredAlphaViaUpdateLayeredWindow(HWND hWnd, BYTE alpha)
{
	if (!::IsWindow(hWnd))
		return false;

	// WS_EX_LAYERED 보장
	LONG_PTR ex = ::GetWindowLongPtr(hWnd, GWL_EXSTYLE);
	if ((ex & WS_EX_LAYERED) == 0)
		::SetWindowLongPtr(hWnd, GWL_EXSTYLE, ex | WS_EX_LAYERED);

	// 창 크기
	CRect rc;
	::GetWindowRect(hWnd, &rc);
	const int w = rc.Width();
	const int h = rc.Height();
	if (w <= 0 || h <= 0)
		return false;

	HDC hdcScreen = ::GetDC(nullptr);
	HDC hdcMem = ::CreateCompatibleDC(hdcScreen);

	BITMAPINFO bmi = {};
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = -h; // top-down
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	void* bits = nullptr;
	HBITMAP hbmp = ::CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, &bits, nullptr, 0);
	if (!hbmp)
	{
		::DeleteDC(hdcMem);
		::ReleaseDC(nullptr, hdcScreen);
		return false;
	}

	HGDIOBJ old = ::SelectObject(hdcMem, hbmp);

	// 1) 현재 창을 메모리 DC로 렌더링(WM_PRINTCLIENT 이용)
	// PRF_ERASEBKGND/PRF_CLIENT/PRF_CHILDREN등 필요에 따라 조정
	::SendMessage(hWnd, WM_PRINT, (WPARAM)hdcMem, PRF_ERASEBKGND | PRF_CLIENT | PRF_CHILDREN);

	POINT ptDst = { rc.left, rc.top };
	SIZE  szDst = { w, h };
	POINT ptSrc = { 0, 0 };

	BLENDFUNCTION bf = {};
	bf.BlendOp = AC_SRC_OVER;
	bf.SourceConstantAlpha = alpha; // 여기서 전역 알파 적용
	bf.AlphaFormat = 0;             // per-pixel alpha 사용 안 함(우리는 상수 알파만)

	BOOL ok = ::UpdateLayeredWindow(
		hWnd,
		hdcScreen,
		&ptDst,
		&szDst,
		hdcMem,
		&ptSrc,
		0,
		&bf,
		ULW_ALPHA);

	::SelectObject(hdcMem, old);
	::DeleteObject(hbmp);
	::DeleteDC(hdcMem);
	::ReleaseDC(nullptr, hdcScreen);

	return ok == TRUE;
}

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

	MARGINS m = { -1 }; // DWM 확장 비활성화(일반적으로 클라이언트 전체 glass를 끄는 효과)
	DwmExtendFrameIntoClientArea(m_hWnd, &m);
	BOOL disable = TRUE;
	DwmSetWindowAttribute(m_hWnd, DWMWA_NCRENDERING_ENABLED, &disable, sizeof(disable));

	//SetTimer(timer_debug_info, 1000, nullptr);

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

	trace(show);

	m_fade_in = show;
	m_in_fade_in = true;

	m_layered.AddLayeredStyle(m_hWnd);
	LONG_PTR ex = ::GetWindowLongPtr(m_hWnd, GWL_EXSTYLE);
	if ((ex & WS_EX_LAYERED) == 0)
		TRACE(_T("WS_EX_LAYERED not set! ex=0x%08llX\n"), (unsigned long long)ex);

	// owner(소유자) 지정: popup이더라도 main 위에서 안정적으로 뜨게
	if (m_parent && ::IsWindow(m_parent->m_hWnd))
		::SetWindowLongPtr(m_hWnd, GWLP_HWNDPARENT, (LONG_PTR)m_parent->m_hWnd);

	if (m_fade_in)
		m_alpha = 50;
	else
		m_alpha = 255;

	//if (!m_layered.SetTransparent(m_hWnd, (BYTE)m_alpha))
	//	TRACE(_T("SetTransparent failed. gle=%lu\n"), GetLastError());
	ApplyLayeredAlphaViaUpdateLayeredWindow(m_hWnd, (BYTE)m_alpha);

	SetWindowPos(&wndTop, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);

	SetTimer(timer_fade_in, 16, nullptr);
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
				//SetForegroundWindowForce(m_hWnd, true);
			}
		}
		else
		{
			m_alpha -= 25;
			if (m_alpha <= 50)
			{
				KillTimer(timer_fade_in);
				m_alpha = 50;
				m_in_fade_in = false;
				//ShowWindow(SW_HIDE);
			}
		}

		TRACE(_T("m_alpha = %d, visible = %d\n"), m_alpha, IsWindowVisible());

		//m_layered.AddLayeredStyle(m_hWnd);
		//LONG_PTR ex = ::GetWindowLongPtr(m_hWnd, GWL_EXSTYLE);
		//if ((ex & WS_EX_LAYERED) == 0)
		//	TRACE(_T("WS_EX_LAYERED not set! ex=0x%08llX\n"), (unsigned long long)ex);

		//if (!m_layered.SetTransparent(m_hWnd, (BYTE)m_alpha))
		//	TRACE(_T("SetTransparent failed. gle=%lu\n"), GetLastError());
		ApplyLayeredAlphaViaUpdateLayeredWindow(m_hWnd, (BYTE)m_alpha);

		//TRACE(_T("m_alpha = %d, visible = %d\n"), m_alpha, IsWindowVisible());
		//ShowWindow(SW_SHOW);

		//처음엔 sliding 방식으로 구현했으나 CSCThemeDlg의 특성 때문인지 direct2d 때문인지
		//상단의 깜빡임이 계속 발생하여 fade in/out 효과로 변경함.
		//SetWindowPos(nullptr, m_cur_pt.x, m_cur_pt.y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
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
	if (m_in_fade_in)
	{
		CSCThemeDlg::set_titlebar_movable(movable);
		return;
	}

	m_layered.AddLayeredStyle(m_hWnd);
	LONG_PTR ex = ::GetWindowLongPtr(m_hWnd, GWL_EXSTYLE);
	if ((ex & WS_EX_LAYERED) == 0)
		TRACE(_T("WS_EX_LAYERED not set! ex=0x%08llX\n"), (unsigned long long)ex);
	
	m_alpha = movable ? 255 : 50;

	//if (movable)
	//{
	//	KillTimer(timer_fade_in);
	//	m_alpha = 255;
	//}
	//else
	//{
	//	m_alpha = 50;
	//}

	if (!m_layered.SetTransparent(m_hWnd, (BYTE)m_alpha))
		TRACE(_T("SetTransparent failed. gle=%lu\n"), GetLastError());


	CSCThemeDlg::set_titlebar_movable(movable);
}

void CTitleDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CSCThemeDlg::OnMouseMove(nFlags, point);
}
