
// ASeeDlg.h: 헤더 파일
//

#pragma once

#include <deque>
#include "Common/Functions.h"
#include "Common/CDialog/SCImage2dDlg/SCImage2dDlg.h"
#include "Common/CDialog/SCShapeDlg/SCShapeDlg.h"
#include "Common/ThumbCtrl/SCThumbCtrl.h"
#include "Common/file_system/SCDirWatcher/SCDirWatcher.h"

#include "TitleDlg.h"
#include "ZigzagColorDlg.h"

// CASeeDlg 대화 상자
class CASeeDlg : public CDialogEx//CSCThemeDlg
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
		timer_refresh_title_area,

		//언제부터인가 dlg 시작 후 m_imgDlg가 갱신되지 않은 상태로 시작된다.
		//아마 CSCD2Context로 on_size_changed()를 옮기면서 발생되는 현상인듯하다.
		//임시로 프로그램 시작 시 타이머로 m_imgDlg.Invalidate()을 강제 호출하며 시작되도록 우선 조치한다.
		timer_refresh_image_area,
	};

	enum RECENT_FOLDERS_POPUP_MENU
	{
		menu_recent_folders_start = 8000,
		menu_recent_folders_end = 8100,
	};
	void			on_menu_recent_folders(UINT nID);

	enum GPS_POPUP_MENU
	{
		menu_gps_start = 8101,
		menu_gps_end = 8105,
	};
	void			on_menu_gps(UINT nID);


	LRESULT			on_message_CASeeApp(WPARAM wParam, LPARAM lParam);

	CSCDirWatcher	m_dir_watcher;
	LRESULT			on_message_CSCDirWatcher(WPARAM wParam, LPARAM lParam);

	LRESULT			on_message_CSCImage2dDlg(WPARAM wParam, LPARAM lParam);


	CTitleDlg		m_titleDlg;

	//title이 ""이면 현재 표시되는 이미지의 파일명으로 변경한다. 클립보드로부터 붙여 넣은 이미지의 경우는 "paste from clipboard"로 표시한다.
	void			update_title(CString title = _T(""));

	CSCImage2dDlg	m_imgDlg;

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

	
	void			execute_video();

//단일 인스턴스 허용시 쉘에서 넘어온 파라미터 처리
	LRESULT			OnMessageShellCommandParameter(WPARAM, LPARAM);

	CRect			m_border_thickness;		//resize 처리를 위한 기본 윈도우 테두리 두께. OnPaint()에서 그리는 테두리는 m_border_width.
	//이 변수는 DwmExtendFrameIntoClientArea()과 관련된 변수로 true/false에 따라 동작하게 하려 했으나 false일 경우는 부작용이 많다. 우선 true로 고정한다.
	//메인에서 WS_THICKFRAME을 주면 
	//bool			m_has_thickframe = true;

	int				m_corner_index = -1;	//커서가 코너의 어느 영역에 있는지


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
	afx_msg void OnMenuProperty();
	afx_msg void OnMenuViewToggle();
	afx_msg void OnMenuCopyToClipboardROI();
	afx_msg void OnMenuCopyToClipboardEXIF();
	afx_msg void OnMenuShowPixelPos();
	afx_msg void OnMenuSmooth();
	afx_msg void OnMenuSaveToRaw();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMenuShowCursorGuideLine();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
