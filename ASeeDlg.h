
// ASeeDlg.h: 헤더 파일
//

#pragma once

#include <deque>
#include "../Common/Functions.h"
#include "../Common/GdiplusBitmap.h"
#include "../Common/CDialog/SCImageDlg/SCImageDlg.h"

// CASeeDlg 대화 상자
class CASeeDlg : public CDialogEx
{
// 생성입니다.
public:
	CASeeDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ASEE_DIALOG };
#endif

protected:
	std::deque<CString> m_files;
	int m_index;

	CSCImageDlg		m_imgDlg;
	void			display_image(int index, bool scan_folder = false);
	//현재 파일을 비롯해서 폴더를 다시 검사한다.
	void			reload_image();
	
	void			update_title();
	void			execute_video();

	//단일 인스턴스 허용시 쉘에서 넘어온 파라미터 처리
	LRESULT			OnMessageShellCommandParameter(WPARAM, LPARAM);

	//타이틀바 및 시스템 버튼
	CRect			m_rTitle;
	CRect			m_rSysButton[3];
	int				m_button_hover_index = -1;
	bool			m_button_pressed;
	void			draw_system_buttons(CDC& dc);

	Gdiplus::InterpolationMode m_interplationMode = Gdiplus::InterpolationModeHighQualityBicubic;


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
};
