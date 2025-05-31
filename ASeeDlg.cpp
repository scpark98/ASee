
// ASeeDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ASee.h"
#include "ASeeDlg.h"
#include "afxdialogex.h"

#include "RoiInputDlg.h"
#include "../Common/messagebox/Win32InputBox/Win32InputBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CASeeDlg 대화 상자



CASeeDlg::CASeeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ASEE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CASeeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CASeeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CASeeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CASeeDlg::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_ERASEBKGND()
	ON_WM_DROPFILES()
	ON_WM_CONTEXTMENU()
	ON_WM_GETMINMAXINFO()
//	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_MENU_ALWAYS_ON_TOP, &CASeeDlg::OnMenuAlwaysOnTop)
	ON_COMMAND(ID_MENU_OPEN_FOLDER, &CASeeDlg::OnMenuOpenFolder)
	ON_COMMAND(ID_MENU_ZOOM_IN, &CASeeDlg::OnMenuZoomIn)
	ON_COMMAND(ID_MENU_ZOOM_ORIGIN, &CASeeDlg::OnMenuZoomOrigin)
	ON_COMMAND(ID_MENU_ZOOM_120, &CASeeDlg::OnMenuZoom120)
	ON_COMMAND(ID_MENU_ZOOM_150, &CASeeDlg::OnMenuZoom150)
	ON_COMMAND(ID_MENU_ZOOM_200, &CASeeDlg::OnMenuZoom200)
	ON_COMMAND(ID_MENU_ZOOM_STRETCH, &CASeeDlg::OnMenuZoomStretch)
	ON_COMMAND(ID_MENU_ZOOM_INPUT, &CASeeDlg::OnMenuZoomInput)
	ON_COMMAND(ID_MENU_GOTO_HOME, &CASeeDlg::OnMenuGotoHome)
	ON_COMMAND(ID_MENU_GOTO_END, &CASeeDlg::OnMenuGotoEnd)
	ON_COMMAND(ID_MENU_SELECT_FOLDER, &CASeeDlg::OnMenuSelectFolder)
	ON_COMMAND(ID_MENU_GOTO, &CASeeDlg::OnMenuGoto)
	ON_COMMAND(ID_MENU_REFRESH, &CASeeDlg::OnMenuRefresh)
	ON_COMMAND(ID_MENU_ROTATE_LEFT, &CASeeDlg::OnMenuRotateLeft)
	ON_COMMAND(ID_MENU_ROTATE_RIGHT, &CASeeDlg::OnMenuRotateRight)
	ON_COMMAND(ID_MENU_MIRROR, &CASeeDlg::OnMenuMirror)
	ON_COMMAND(ID_MENU_FLIP, &CASeeDlg::OnMenuFlip)
	ON_COMMAND(ID_MENU_SAVE_AS, &CASeeDlg::OnMenuSaveAs)
	ON_COMMAND(ID_MENU_SLIDE_SHOW, &CASeeDlg::OnMenuSlideShow)
	ON_COMMAND(ID_MENU_SLIDE_SHOW_INTERVAL, &CASeeDlg::OnMenuSlideShowInterval)
	ON_COMMAND(ID_MENU_SLIDE_SHOW_REPEAT, &CASeeDlg::OnMenuSlideShowRepeat)
	ON_COMMAND(ID_MENU_SHOW_PIXEL, &CASeeDlg::OnMenuShowPixel)
	ON_COMMAND(ID_MENU_WALLPAPER, &CASeeDlg::OnMenuWallpaper)
	ON_COMMAND(ID_MENU_COPY_TO_CLIPBOARD, &CASeeDlg::OnMenuCopyToClipboard)
	ON_COMMAND(ID_MENU_DELETE, &CASeeDlg::OnMenuDelete)
	ON_COMMAND(ID_MENU_CLOSE, &CASeeDlg::OnMenuClose)
	ON_COMMAND(ID_MENU_SELECT, &CASeeDlg::OnMenuSelect)
	ON_COMMAND(ID_MENU_SMOOTH, &CASeeDlg::OnMenuSmooth)
	ON_COMMAND(ID_MENU_SHOW_ROI_INFO, &CASeeDlg::OnMenuShowRoiInfo)
	ON_COMMAND(ID_MENU_INPUT_ROI, &CASeeDlg::OnMenuInputRoi)
	ON_COMMAND(ID_MENU_SHOW_INFO, &CASeeDlg::OnMenuShowInfo)
	ON_COMMAND(ID_MENU_SMOOTH_BILINEAR, &CASeeDlg::OnMenuSmoothBilinear)
	ON_COMMAND(ID_MENU_SMOOTH_BICUBIC, &CASeeDlg::OnMenuSmoothBicubic)
	ON_COMMAND(ID_MENU_SMOOTH_LANCZOS, &CASeeDlg::OnMenuSmoothLanczos)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CASeeDlg 메시지 처리기

BOOL CASeeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_titleDlg.Create(IDD_TITLE, this);
	m_titleDlg.set_titlebar_movable(false);

	m_imgDlg.create(this);
	m_imgDlg.set_show_info(theApp.GetProfileInt(_T("setting"), _T("show info"), false));
	m_imgDlg.set_show_pixel(theApp.GetProfileInt(_T("setting"), _T("show pixel"), false));
	bool fit = theApp.GetProfileInt(_T("setting"), _T("fit to ctrl"), true);
	m_imgDlg.fit2ctrl(fit);

	if (!fit)
		m_imgDlg.zoom(GetProfileDouble(&theApp, _T("setting"), _T("zoom"), 1.0));

	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0);

	RestoreWindowPosition(&theApp, this);

	if (IsZoomed())
	{
		ModifyStyle(WS_CAPTION, 0);
		SetWindowPos(nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER);
	}
	else
	{
		ModifyStyle(0, WS_CAPTION | WS_THICKFRAME);
	}

	bool bAlwaysOnTop = theApp.GetProfileInt(_T("setting"), _T("always on top"), false);
	SetWindowPos((bAlwaysOnTop ? &wndTopMost : &wndNoTopMost), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	CString sfile = theApp.GetProfileString(_T("setting"), _T("shell parameter"), _T(""));

	if (sfile.IsEmpty() == false && GetFileTypeFromFilename(sfile) == FILE_TYPE_IMAGE)
	{
		m_files.push_back(sfile);
		display_image(0, true);
		theApp.WriteProfileString(_T("setting"), _T("shell parameter"), _T(""));
	}
	else
	{
		sfile = theApp.GetProfileString(_T("setting"), _T("recent file"), _T(""));
		if (PathFileExists(sfile) && sfile.IsEmpty() == false && GetFileTypeFromFilename(sfile) == FILE_TYPE_IMAGE)
		{
			m_files.push_back(sfile);
			display_image(0, true);
		}
		else if (!sfile.IsEmpty())
		{
			m_files = find_all_files(get_part(sfile, fn_folder), _T("*"), FILE_EXTENSION_IMAGE, _T(""), false);
			display_image(0, true);
		}
	}

	DragAcceptFiles();

	/* for test 좌표변환 시 소실되는 값 확인용
	CRect displayed(0, 0, 789, 675);
	Gdiplus::RectF scr(123, 235, 413, 523), img;

	for (int i = 0; i < 1000; i++)
	{
		get_real_coord_from_screen_coord(displayed, 431, scr, &img);
		get_screen_coord_from_real_coord(displayed, 431, img, &scr);
	}
	*/

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CASeeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CASeeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CASeeDlg::OnPaint()에서 특별히 하는 일이 없어도 기본으로 생성된 코드인 CDialogEx::OnPaint(); 코드를 생략해선 안된다.
		//생략할 경우 maximize, restore시에 제대로 이벤트가 발생하지 않는다.
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CASeeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CASeeDlg::OnBnClickedOk()
{
	//m_notice.ShowWindow(SW_HIDE);

	if (!IsZoomed())
	{
		ShowWindow(SW_HIDE);
		ModifyStyle(WS_CAPTION | WS_THICKFRAME, 0);
		//SetWindowPos(nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER);
		//DefWindowProc(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		ShowWindow(SW_MAXIMIZE);
	}
	else
	{
		ModifyStyle(0, WS_CAPTION | WS_THICKFRAME);// | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME);
		//SetWindowPos(nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER);
		//DefWindowProc(WM_SYSCOMMAND, SC_RESTORE, 0);
		//SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME);
		//show_filename(false);
		ShowWindow(SW_RESTORE);
	}
}

void CASeeDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_files.size() && m_index >= 0)
		theApp.WriteProfileString(_T("setting"), _T("recent file"), m_files[m_index]);

	CDialogEx::OnCancel();
}

void CASeeDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_imgDlg.m_hWnd == NULL || m_titleDlg.m_hWnd == NULL)
		return;

	CRect rc;
	GetClientRect(rc);
	m_imgDlg.MoveWindow(rc);

	rc.bottom = rc.top + m_titleDlg.get_titlebar_height();
	m_titleDlg.MoveWindow(rc);
}

void CASeeDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_imgDlg.m_hWnd == NULL || m_titleDlg.m_hWnd == NULL)
		return;

	CRect rc;
	GetClientRect(rc);
	m_imgDlg.MoveWindow(rc);

	CRect rw;
	GetWindowRect(rw);
	rw.bottom = rw.top + m_titleDlg.get_titlebar_height();
	m_titleDlg.MoveWindow(rw);
	ClientToScreen(rw);

	SaveWindowPosition(&theApp, this);
}

BOOL CASeeDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return FALSE;
	return CDialogEx::OnEraseBkgnd(pDC);
}

void CASeeDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	TCHAR sfile[MAX_PATH];

	DragQueryFile(hDropInfo, 0, sfile, MAX_PATH);

	m_files.clear();

	if (IsFolder(sfile))
	{
		m_files = find_all_files(get_part(sfile, fn_folder), _T("*"), FILE_EXTENSION_IMAGE, _T(""), false);
		if (m_files.size() > 0)
			display_image(0, false);
	}
	else if (GetFileTypeFromFilename(sfile) == FILE_TYPE_IMAGE)
	{
		m_files.push_back(sfile);
		if (m_files.size() > 0)
			display_image(0, true);
	}

	CDialogEx::OnDropFiles(hDropInfo);
}

//index : -1(next), -2(previous)
void CASeeDlg::display_image(int index, bool scan_folder)
{
	if (m_files.size() == 0)
	{
		update_title();
		return;
	}

	//m_static_img.show_filename(true);

	if (index == -1)
	{
		if (m_index >= m_files.size() - 1)
		{
			//m_notice.set_notice(_T("맨 마지막 이미지"));
			return;
		}
		m_index++;
	}
	else if (index == -2)
	{
		if (m_index == 0)
		{
			//m_notice.set_notice(_T("맨 처음 이미지"));
			return;
		}
		m_index--;
	}
	else
	{
		m_index = index;
	}

	//m_notice.ShowWindow(SW_HIDE);

	m_imgDlg.load(m_files[m_index]);

	Invalidate();

	if (scan_folder)
	{
		CString cur_file = m_files[m_index];

		m_files.clear();

		m_files = find_all_files(get_part(cur_file, fn_folder), _T("*"), FILE_EXTENSION_IMAGE, _T(""), false, true);
		m_index = find_index(m_files, cur_file);
	}

	update_title();
}

void CASeeDlg::update_title()
{
	CString str = _T("ASee");

	if (m_index >= 0 && m_index < m_files.size())
		str.Format(_T("ASee - %s (%d/%d)"), get_part(m_files[m_index], fn_name), m_index + 1, m_files.size());

	SetWindowText(str);
	m_titleDlg.update_title(str);
}

void CASeeDlg::execute_video()
{
	CString sfile = get_part(m_files[m_index], fn_title);
	sfile.Replace(_T("_Snapshot"), _T(""));

	while (GetFileTypeFromExtension(sfile) == FILE_TYPE_VIDEO)
		sfile = get_part(sfile, fn_title);
	sfile += _T("*");

	std::deque<CString> dqFiles;
	dqFiles = find_all_files(get_part(m_files[m_index], fn_folder), sfile, FILE_EXTENSION_VIDEO, _T(""), false);

	if (dqFiles.size())
		SHELL_OPEN(dqFiles[0]);
}

//현재 파일을 비롯해서 폴더를 다시 검사한다.
void CASeeDlg::reload_image()
{
	//현재 파일의 인덱스와 파일명을 기억해두고
	int old_index = m_index;
	CString sfile = m_files[m_index];

	m_imgDlg.m_img.release();
	m_files.clear();

	//현재 폴더의 이미지 파일들을 다시 검색하고
	m_files = find_all_files(get_part(sfile, fn_folder), _T("*"), FILE_EXTENSION_IMAGE, _T(""), false);

	//검색된 파일들에서 현재 이미지의 인덱스를 찾고
	int index = find_dqstring(m_files, sfile);

	if (m_files.size() == 0)
	{
		Invalidate();
		return;
	}

	if (index < 0)
	{
		if (old_index >= 0 && old_index < m_files.size())
			index = old_index;
		else
			index = 0;
	}
	else if (index >= m_files.size())
	{
		index = m_files.size() - 1;
	}

	display_image(index, false);
}

void CASeeDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	CMenu* pMenu;
	CString str;

	menu.LoadMenu(IDR_MENU_CONTEXT);
	pMenu = menu.GetSubMenu(0);

	pMenu->CheckMenuItem(ID_MENU_ALWAYS_ON_TOP, (theApp.GetProfileInt(_T("setting"), _T("always on top"), false) ? MF_CHECKED : MF_UNCHECKED));
	pMenu->CheckMenuItem(ID_MENU_SHOW_INFO, (m_imgDlg.get_show_info() ? MF_CHECKED : MF_UNCHECKED));
	pMenu->CheckMenuItem(ID_MENU_SHOW_PIXEL, (m_imgDlg.get_show_pixel() ? MF_CHECKED : MF_UNCHECKED));

	pMenu->CheckMenuItem(ID_MENU_SMOOTH, (m_imgDlg.get_smooth_interpolation() != CSCImageDlg::interpolation_none ? MF_CHECKED : MF_UNCHECKED));
	pMenu->CheckMenuItem(ID_MENU_SMOOTH_BILINEAR, (m_imgDlg.get_smooth_interpolation() == CSCImageDlg::interpolation_bilinear ? MF_CHECKED : MF_UNCHECKED));
	pMenu->CheckMenuItem(ID_MENU_SMOOTH_BICUBIC, (m_imgDlg.get_smooth_interpolation() == CSCImageDlg::interpolation_bicubic ? MF_CHECKED : MF_UNCHECKED));
	pMenu->CheckMenuItem(ID_MENU_SMOOTH_LANCZOS, (m_imgDlg.get_smooth_interpolation() == CSCImageDlg::interpolation_lanczos ? MF_CHECKED : MF_UNCHECKED));

	pMenu->EnableMenuItem(ID_MENU_SHOW_ROI_INFO, (m_imgDlg.get_image_roi().IsEmptyArea() ? MF_DISABLED : MF_ENABLED));
	pMenu->CheckMenuItem(ID_MENU_SHOW_ROI_INFO, (m_imgDlg.get_show_roi_info() ? MF_CHECKED : MF_UNCHECKED));

	pMenu->CheckMenuItem(ID_MENU_ZOOM_ORIGIN, (m_imgDlg.get_zoom_ratio() == 1.0 ? MF_CHECKED : MF_UNCHECKED));
	pMenu->CheckMenuItem(ID_MENU_ZOOM_120, (m_imgDlg.get_zoom_ratio() == 1.2 ? MF_CHECKED : MF_UNCHECKED));
	pMenu->CheckMenuItem(ID_MENU_ZOOM_150, (m_imgDlg.get_zoom_ratio() == 1.5 ? MF_CHECKED : MF_UNCHECKED));
	pMenu->CheckMenuItem(ID_MENU_ZOOM_200, (m_imgDlg.get_zoom_ratio() == 2.0 ? MF_CHECKED : MF_UNCHECKED));
	pMenu->CheckMenuItem(ID_MENU_ZOOM_STRETCH, (m_imgDlg.get_fit2ctrl() ? MF_CHECKED : MF_UNCHECKED));

	//pMenu->CheckMenuItem(ID_MENU_SLIDE_SHOW, (m_slide_show ? MF_CHECKED : MF_UNCHECKED));
	//pMenu->CheckMenuItem(ID_MENU_SLIDE_SHOW_REPEAT, (m_slide_show_repeat ? MF_CHECKED : MF_UNCHECKED));

	//pMenu->EnableMenuItem(ID_MENU_SAVE_AS, (m_modified ? MF_ENABLED : MF_DISABLED));

	//str.Format(_T("확대 비율 입력(현재 %.2f)..."), m_zoom);
	//pMenu->ModifyMenu(ID_MENU_ZOOM_INPUT, MF_BYCOMMAND, ID_MENU_ZOOM_INPUT, str);

	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CASeeDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CSize sz;
	bool is_auto_hide = get_taskbar_state(ABS_AUTOHIDE, &sz);

	lpMMI->ptMaxSize.y -= (is_auto_hide ? 2 : sz.cy);

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

BOOL CASeeDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	//Ctrl키를 누르고 wheel을 돌리면 확대/축소되는 기능은 이미 SCImageDlg에 구현되어 있다.
	//메인에서는 그냥 wheel을 돌리면 이전/다음 이미지 표시 기능으로 동작한다.
	TRACE(_T("%d OnMouseWheel\n"), GetTickCount());

	if (IsCtrlPressed())
	{
		m_imgDlg.zoom(zDelta > 0 ? 1 : -1);
		//return TRUE;
	}
	else
	{
		display_image((zDelta < 0 ? -1 : -2));
	}

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CASeeDlg::OnMenuAlwaysOnTop()
{
	bool bAlwaysOnTop = theApp.GetProfileInt(_T("setting"), _T("always on top"), false);
	bAlwaysOnTop = !bAlwaysOnTop;
	theApp.WriteProfileInt(_T("setting"), _T("always on top"), bAlwaysOnTop);
	SetWindowPos((bAlwaysOnTop ? &wndTopMost : &wndNoTopMost), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void CASeeDlg::OnMenuOpenFolder()
{
	CString folder;

	if (m_files.size() == 0)
		ShellExecute(NULL, _T("open"), _T("explorer"), get_exe_directory(), NULL, SW_SHOW);
	else
		ShellExecute(NULL, _T("open"), _T("explorer"), _T("/select,") + m_files[m_index], NULL, SW_SHOW);
}

void CASeeDlg::OnMenuZoomIn()
{
	m_imgDlg.zoom(1);
}

void CASeeDlg::OnMenuZoomOrigin()
{
	m_imgDlg.zoom(0);
}

void CASeeDlg::OnMenuZoom120()
{
	m_imgDlg.zoom(1.2);
}

void CASeeDlg::OnMenuZoom150()
{
	m_imgDlg.zoom(1.5);
}

void CASeeDlg::OnMenuZoom200()
{
	m_imgDlg.zoom(2.0);
}

void CASeeDlg::OnMenuZoomStretch()
{
	m_imgDlg.fit2ctrl(true);
}

void CASeeDlg::OnMenuZoomInput()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CASeeDlg::OnMenuGotoHome()
{
	if (m_files.size() == 0)
		return;

	display_image(0);
}

void CASeeDlg::OnMenuGotoEnd()
{
	if (m_files.size() == 0)
		return;

	display_image(m_files.size() - 1);
}

void CASeeDlg::OnMenuSelectFolder()
{
	CString sSelected;

	if (m_files.size())
		sSelected = get_part(m_files[m_index], fn_folder);
	else
		sSelected = theApp.GetProfileString(_T("setting"), _T("recent folder"), _T(""));

	if (!BrowseForFolder(m_hWnd, _T("폴더 선택"), sSelected, sSelected, false))
		return;

	theApp.WriteProfileString(_T("setting"), _T("recent folder"), sSelected);

	m_files.clear();

	m_files = find_all_files(sSelected, _T("*"), FILE_EXTENSION_IMAGE, _T(""), false);

	if (m_files.size())
	{
		display_image(0, true);
	}
	else
	{
		//m_notice.set_notice(_T("선택된 폴더에는 이미지 파일이 없습니다."));
	}
}

void CASeeDlg::OnMenuGoto()
{
	if (m_files.size() < 2)
		return;

	TCHAR buf[128] = { 0 };
	CString str;

	if (m_index >= 0)
		_stprintf_s(buf, countof(buf), _T("%d"), m_index + 1);

	str.Format(_T("입력 범위 : 1 ~ %d"), m_files.size());

	int r = CWin32InputBox::InputBox(_T("이미지 인덱스 이동"), str, buf, 128, CWin32InputBox::NORMAL);
	if (r == IDCANCEL)
		return;

	int index = _ttoi(buf);
	if (index <= 0 || index > m_files.size())
	{
		str.Format(_T("1 ~ %d 사이의 값을 입력하세요."), m_files.size());
		AfxMessageBox(str, MB_ICONEXCLAMATION);
		OnMenuGoto();
		return;
	}

	display_image(index - 1, true);
}

void CASeeDlg::OnMenuRefresh()
{
	reload_image();
}

void CASeeDlg::OnMenuRotateLeft()
{
	m_imgDlg.rotate(Gdiplus::Rotate270FlipNone);
}

void CASeeDlg::OnMenuRotateRight()
{
	m_imgDlg.rotate(Gdiplus::Rotate90FlipNone);
}

void CASeeDlg::OnMenuMirror()
{
	m_imgDlg.rotate(Gdiplus::RotateNoneFlipX);
}

void CASeeDlg::OnMenuFlip()
{
	m_imgDlg.rotate(Gdiplus::RotateNoneFlipY);
}

void CASeeDlg::OnMenuSaveAs()
{
	CFileDialog dlg(false, 0, m_files[m_index], OFN_HIDEREADONLY /*| OFN_EXPLOPER */ | OFN_OVERWRITEPROMPT);
	if (dlg.DoModal() == IDCANCEL)
		return;

	CString sfile = dlg.GetPathName();
	m_imgDlg.m_img.save(sfile);
}

void CASeeDlg::OnMenuSlideShow()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CASeeDlg::OnMenuSlideShowInterval()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CASeeDlg::OnMenuSlideShowRepeat()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CASeeDlg::OnMenuShowPixel()
{
	m_imgDlg.set_show_pixel(!m_imgDlg.get_show_pixel());
}

void CASeeDlg::OnMenuWallpaper()
{
	set_wallpaper(m_files[m_index]);
}

void CASeeDlg::OnMenuCopyToClipboard()
{
	if (m_imgDlg.m_img.is_empty())
		return;

	m_imgDlg.m_img.copy_to_clipbard();
	MessageBeep(0);

	//m_notice.set_notice(_T("클립보드로 복사됨"));
}

void CASeeDlg::OnMenuDelete()
{
	if (m_imgDlg.get_image_roi().IsEmptyArea() == false)
	{
		m_imgDlg.set_image_roi();
		return;
	}

	if (m_files.size() == 0)
		return;

	delete_file(m_files[m_index], true);

	//현재 이미지 파일을 지우면 해당 폴더를 다시 스캔한다.
	reload_image();
}

void CASeeDlg::OnMenuClose()
{
	OnBnClickedCancel();
}

void CASeeDlg::OnMenuSelect()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CASeeDlg::OnMenuSmooth()
{
	m_imgDlg.set_smooth_interpolation(CSCImageDlg::interpolation_none);
}

void CASeeDlg::OnMenuSmoothBilinear()
{
	m_imgDlg.set_smooth_interpolation(CSCImageDlg::interpolation_bilinear);
}

void CASeeDlg::OnMenuSmoothBicubic()
{
	m_imgDlg.set_smooth_interpolation(CSCImageDlg::interpolation_bicubic);
}

void CASeeDlg::OnMenuSmoothLanczos()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CASeeDlg::OnMenuShowRoiInfo()
{
	m_imgDlg.set_show_roi_info(!m_imgDlg.get_show_roi_info());
}

void CASeeDlg::OnMenuInputRoi()
{
	CRoiInputDlg dlg(this);

	CSize sz = m_imgDlg.m_img.size();
	Gdiplus::RectF roi = m_imgDlg.get_image_roi();

	dlg.init(sz.cx, sz.cy, roi.Width, roi.Height);

	if (dlg.DoModal() == IDCANCEL)
		return;

	roi.Width = dlg.m_roi_sz.cx;
	roi.Height = dlg.m_roi_sz.cy;
	m_imgDlg.set_image_roi(roi);
}

BOOL CASeeDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		CRect rc;
		GetClientRect(rc);

		TRACE(_T("%s : %d\n"), __function__, pMsg->wParam);
		switch (pMsg->wParam)
		{
			case VK_ESCAPE:
				OnBnClickedCancel();
				return true;
			case VK_RETURN:
				if (IsCtrlPressed())
				{
					if (IsShiftPressed())
					{
						execute_video();
					}
				}
				else
				{
					OnBnClickedOk();
				}
				return true;
			case VK_F5:
				reload_image();
				return true;
			case VK_DELETE:
				OnMenuDelete();
				return true;

			case 'F':
				if (IsCtrlPressed())
				{
					OnMenuSelectFolder();
				}
				return true;
			case 'O':
				if (IsCtrlPressed())
					OnMenuOpenFolder();
				return true;
			case VK_HOME:
				start_slide_show(0);
				OnMenuGotoHome();
				return true;
			case VK_END:
				start_slide_show(0);
				OnMenuGotoEnd();
				return true;
			case VK_UP:
				start_slide_show(0);

				if (m_imgDlg.get_fit2ctrl() || RectInRect(rc, m_imgDlg.get_displayed_rect()))
				{
					display_image(-2);
				}
				else
				{
					m_imgDlg.set_offset(0, -m_imgDlg.get_offset_size());
					Invalidate();
					TRACE(_T("offset : %d, %d\n"), m_imgDlg.get_offset().x, m_imgDlg.get_offset().y);
				}
				return true;
			case VK_PRIOR:
				start_slide_show(0);
				display_image(-2);
				return true;
			case VK_DOWN:
				start_slide_show(0);
				if (m_imgDlg.get_fit2ctrl() || RectInRect(rc, m_imgDlg.get_displayed_rect()))
				{
					display_image(-1);
				}
				else
				{
					m_imgDlg.set_offset(0, m_imgDlg.get_offset_size());
					Invalidate();
					TRACE(_T("offset : %d, %d\n"), m_imgDlg.get_offset().x, m_imgDlg.get_offset().y);
				}
				return true;
			case VK_NEXT:
				start_slide_show(0);
				display_image(-1);
				return true;
			case VK_LEFT:
				start_slide_show(0);
				if (m_imgDlg.get_fit2ctrl())
				{
					display_image(-2);
				}
				else
				{
					m_imgDlg.set_offset(-m_imgDlg.get_offset_size(), 0);
					Invalidate();
				}
				return true;
			case VK_RIGHT:
				start_slide_show(0);
				if (m_imgDlg.get_fit2ctrl())
				{
					display_image(-1);
				}
				else
				{
					m_imgDlg.set_offset(m_imgDlg.get_offset_size(), 0);
					Invalidate();
				}
				return true;
			case '1':
				OnMenuZoomOrigin();
				return true;
			case '2':
				OnMenuZoom120();
				return true;
			case '3':
				OnMenuZoom150();
				return true;
			case '4':
				OnMenuZoom200();
				return true;
			case '5':
				OnMenuZoomStretch();
				return true;
			case VK_TAB :
				OnMenuShowInfo();
				break;
		}
	}
	else if (pMsg->message == WM_SYSKEYDOWN)
	{
		switch (pMsg->wParam)
		{
			case VK_SPACE:
				break;
			case VK_LEFT:
				OnMenuRotateLeft();
				break;
			case VK_RIGHT:
				OnMenuRotateRight();
				break;
			case VK_UP:
				OnMenuMirror();
				break;
			case VK_DOWN:
				OnMenuFlip();
				break;
			case '1':
				m_imgDlg.set_smooth_interpolation(CSCImageDlg::interpolation_none);
				break;
			case '2':
				m_imgDlg.set_smooth_interpolation(CSCImageDlg::interpolation_bilinear);
				break;
			case '3':
				m_imgDlg.set_smooth_interpolation(CSCImageDlg::interpolation_bicubic);
				break;
			}
	}
	else if (pMsg->message == WM_LBUTTONDBLCLK)
	{
		TRACE(_T("WM_LBUTTONDBLCLK\n"));
	}
	else if (pMsg->message == WM_MOUSEMOVE)
	{
		TRACE(_T("WM_MOUSEMOVE\n"));
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

//start : 1(start), 0(stop), -1(toggle)
void CASeeDlg::start_slide_show(int start)
{
	if (start == -1)
		m_slide_show = !m_slide_show;
	else
		m_slide_show = start;

	if (m_slide_show)
	{
		if (m_slide_show_interval < 0)
		{
			SetTimer(timer_slide_show, -m_slide_show_interval, NULL);
		}
		else
		{
			SetTimer(timer_slide_show, m_slide_show_interval * 1000, NULL);
		}
	}
	else
	{
		KillTimer(timer_slide_show);
	}
}

void CASeeDlg::OnMenuShowInfo()
{
	bool show = !m_imgDlg.get_show_info();
	m_imgDlg.set_show_info(show);
	theApp.WriteProfileInt(_T("setting"), _T("show info"), show);
}

void CASeeDlg::draw_system_buttons(CDC& dc)
{
	if (m_button_hover_index >= 0 && m_button_hover_index < 3)
		dc.FillSolidRect(m_rSysButton[m_button_hover_index], (m_button_hover_index == 2 ? RGB(255, 0, 0) : (m_button_pressed ? RGB(0, 122, 204) : GRAY(96))));

	//draw minimize
	draw_line(&dc, m_rSysButton[0].left + 20, m_rSysButton[0].bottom - 10,
		m_rSysButton[0].right - 20, m_rSysButton[0].bottom - 10, gGRAY(192), 1);

	//draw restore
	CRect r = make_center_rect(m_rSysButton[1].CenterPoint().x, m_rSysButton[1].CenterPoint().y, 14, 14);
	r.OffsetRect(2, -2);
	draw_rectangle(&dc, r, gGRAY(192), NULL_BRUSH, 1);
	r.OffsetRect(-4, 4);
	draw_rectangle(&dc, r, gGRAY(192), (m_button_hover_index == 1 ? (m_button_pressed ? gRGB(0, 122, 204) : gGRAY(96)) : gGRAY(96)), 1);

	//draw quit
	int size = m_rSysButton[2].Height() / 4;
	r = m_rSysButton[2];
	CPoint cp = r.CenterPoint();
	//r.DeflateRect(4, 4);
	draw_line(&dc, cp.x - size, cp.y - size, cp.x + size, cp.y + size, gGRAY(192), 1);
	draw_line(&dc, cp.x - size, cp.y + size, cp.x + size, cp.y - size, gGRAY(192), 1);

	//for (int i = 0; i < 3; i++)
	//	DrawRectangle(&dc, m_rSysButton[i], red);
}

void CASeeDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (IsZoomed())
	{
		if (/*m_titleDlg.IsWindowVisible() == false && */point.y < m_titleDlg.get_titlebar_height())
			m_titleDlg.ShowWindow(SW_SHOW);
		else
			m_titleDlg.ShowWindow(SW_HIDE);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}
