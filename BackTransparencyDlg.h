#pragma once
#include "afxdialogex.h"

#include "Common/CStatic/SCStatic/SCStatic.h"
#include "Common/CSliderCtrl/SCSliderCtrl/SCSliderCtrl.h"

// CBackTransparencyDlg 대화 상자

class CBackTransparencyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBackTransparencyDlg)

public:
	CBackTransparencyDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CBackTransparencyDlg();

	void			set_target_index(int index) { m_target_index = index; }
	void			show_window(int nCmdShow);

	LRESULT			on_message_CSCSliderCtrl(WPARAM wParam, LPARAM lParam);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BACK_TRANSPARENCY };
#endif

protected:
	//배경 자동 감지 or 수동
	//bool			m_auto_detect = true;
	Gdiplus::Color	m_cr_back = Gdiplus::Color::Transparent;

	int				m_target_index = 0;
	int				m_inner_threshold = 30;
	int				m_outer_threshold = 120;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	CSCSliderCtrl m_slider_inner_threshold;
	CSCSliderCtrl m_slider_outer_threshold;
	CSCStatic m_static_inner_threshold;
	CSCStatic m_static_outer_threshold;
	CButton m_check_auto_detect;
	CMFCColorButton m_button_back_color;
	afx_msg void OnBnClickedCheckAutoDetect();
	afx_msg void OnBnClickedButtonBackColor();
	afx_msg void OnBnClickedButtonApplyToAllFrames();
	afx_msg void OnBnClickedButtonApplyCurFrame();
};
