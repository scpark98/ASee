
// ASeeDlg.h: 헤더 파일
//

#pragma once

#include <deque>
#include "Functions.h"
#include "GdiplusBitmap.h"
#include "CDialog/SCImageDlg/SCImageDlg.h"
#include "CDialog/SCShapeDlg/SCShapeDlg.h"
#include "ThumbCtrl/SCThumbCtrl.h"
#include "file_system/SCDirWatcher/SCDirWatcher.h"

#include "TitleDlg.h"
#include "ZigzagColorDlg.h"

// CASeeDlg 대화 상자
class CASeeDlg : public CDialogEx
{
// 생성입니다.
public:
	CASeeDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	CZigzagColorDlg		m_zigzagColorDlg;

	void				set_zigzag_color(COLORREF cr_back, COLORREF cr_fore);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ASEE_DIALOG };
#endif


protected:
	enum TIMER_ID
	{
		timer_search_scan_folder = 0,
		timer_refresh_navi,
		timer_scroll,
		timer_slide_show,
		timer_cursor_pos,
		timer_rect_info_changed,
		timer_hide_filename,
	};

	enum RECENT_FOLDERS_POPUP_MENU
	{
		menu_recent_folders_start = 8000,
		menu_recent_folders_end = 8100,
	};
	void			on_menu_recent_folders(UINT nID);

	LRESULT			on_message_CASeeApp(WPARAM wParam, LPARAM lParam);

	CSCDirWatcher	m_dir_watcher;
	LRESULT			on_message_CSCDirWatcher(WPARAM wParam, LPARAM lParam);

	std::deque<CString> m_files;
	int m_index;

	CTitleDlg		m_titleDlg;
	//title이 ""이면 현재 표시되는 이미지의 파일명으로 변경한다. 클립보드로부터 붙여 넣은 이미지의 경우는 "paste from clipboard"로 표시한다.
	void			update_title(CString title = _T(""));

	CSCImageDlg		m_imgDlg;
	void			display_image(int index, bool scan_folder = false);

	CSCShapeDlg		m_message;
	void			show_message(CString message);

	enum ENUM_ADJUST_TYPE
	{
		adjust_reset = 0,
		adjust_bright,
		adjust_contrast,
	};
	void			show_adjust_message(int type, int percentage = 100, bool invalidate = true);

	//파일명, 크기정보 등 표시
	//bool			m_show_info = false;

//현재 파일을 비롯해서 폴더를 다시 검사한다.
	void			reload_image();
	
	void			execute_video();

//단일 인스턴스 허용시 쉘에서 넘어온 파라미터 처리
	LRESULT			OnMessageShellCommandParameter(WPARAM, LPARAM);

//타이틀바 및 시스템 버튼
	CRect			m_rTitle;
	CRect			m_rSysButton[3];
	int				m_button_hover_index = -1;
	bool			m_button_pressed;
	void			draw_system_buttons(CDC& dc);

//슬라이드 쇼 관련
	bool			m_slide_show;
	bool			m_slide_show_repeat;
	//단위 = 초
	int				m_slide_show_interval;
	//start : 1(start), 0(stop), -1(toggle)
	void			start_slide_show(int start);

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
//	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMenuAlwaysOnTop();
	afx_msg void OnMenuOpenFolder();
	afx_msg void OnMenuZoomIn();
	afx_msg void OnMenuZoomOrigin();
	afx_msg void OnMenuZoom120();
	afx_msg void OnMenuZoom150();
	afx_msg void OnMenuZoom200();
	afx_msg void OnMenuZoomStretch();
	afx_msg void OnMenuZoomInput();
	afx_msg void OnMenuGotoHome();
	afx_msg void OnMenuGotoEnd();
	afx_msg void OnMenuSelectFolder();
	afx_msg void OnMenuGoto();
	afx_msg void OnMenuRefresh();
	afx_msg void OnMenuRotateLeft();
	afx_msg void OnMenuRotateRight();
	afx_msg void OnMenuMirror();
	afx_msg void OnMenuFlip();
	afx_msg void OnMenuSaveAs();
	afx_msg void OnMenuSlideShow();
	afx_msg void OnMenuSlideShowInterval();
	afx_msg void OnMenuSlideShowRepeat();
	afx_msg void OnMenuShowPixel();
	afx_msg void OnMenuWallpaper();
	afx_msg void OnMenuCopyToClipboard();
	afx_msg void OnMenuDelete();
	afx_msg void OnMenuClose();
	afx_msg void OnMenuSelect();
	afx_msg void OnMenuShowRoiInfo();
	afx_msg void OnMenuInputRoi();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMenuShowInfo();
	afx_msg void OnMenuSmoothNone();
	afx_msg void OnMenuSmoothBilinear();
	afx_msg void OnMenuSmoothBicubic();
	afx_msg void OnMenuSmoothLanczos();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMenuRecentFoldersClear();
	afx_msg void OnMenuTransparentBack();
};
