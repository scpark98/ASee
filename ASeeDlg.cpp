
// ASeeDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ASee.h"
#include "ASeeDlg.h"
#include "afxdialogex.h"

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
	m_imgDlg.create(this);
	m_imgDlg.fit2ctrl(true);// theApp.GetProfileInt(_T("setting"), _T("fit to dlg"), true));
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
	CDialogEx::OnCancel();
}

void CASeeDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_imgDlg.m_hWnd == NULL)
		return;

	CRect rc;
	GetClientRect(rc);
	m_imgDlg.MoveWindow(rc);
}

void CASeeDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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
		return;

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

	if (m_imgDlg.m_img.is_valid())
	{
		//if (m_static_img.m_show_pixel)
			//m_static_img.m_img.get_raw_data();

		//투명색이 포함된 이미지일 경우는 배경에 grid pattern을 표시한 후 그림을 그려줘야 한다.
		if (m_imgDlg.m_img.channel == 4)
		{
			//m_static_img.m_img_grid.release();
			//m_img_grid.create(m_img.width, m_img.height, Gdiplus::PixelFormat)
		}
	}

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
	CString str;

	str.Format(_T("ASee - %s (%d/%d)"), m_files[m_index], m_index + 1, m_files.size());
	SetWindowText(str);
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
	pMenu->CheckMenuItem(ID_MENU_SHOW_FILENAME, (m_imgDlg.get_show_filename() ? MF_CHECKED : MF_UNCHECKED));
	pMenu->CheckMenuItem(ID_MENU_SHOW_PIXEL, (m_imgDlg.get_show_pixel() ? MF_CHECKED : MF_UNCHECKED));
	//pMenu->CheckMenuItem(ID_MENU_SHOW_RESOLUTION, (m_show_resolution ? MF_CHECKED : MF_UNCHECKED));

	//pMenu->CheckMenuItem(ID_MENU_ZOOM_ORIGIN, (m_zoom_type == zoom_original ? MF_CHECKED : MF_UNCHECKED));
	//pMenu->CheckMenuItem(ID_MENU_ZOOM_120, (m_zoom == 1.2 ? MF_CHECKED : MF_UNCHECKED));
	//pMenu->CheckMenuItem(ID_MENU_ZOOM_150, (m_zoom == 1.5 ? MF_CHECKED : MF_UNCHECKED));
	//pMenu->CheckMenuItem(ID_MENU_ZOOM_200, (m_zoom == 2.0 ? MF_CHECKED : MF_UNCHECKED));
	//pMenu->CheckMenuItem(ID_MENU_ZOOM_STRETCH, (m_zoom_type == zoom_stretch ? MF_CHECKED : MF_UNCHECKED));

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
