#pragma once

#include "Common/CButton/SCSystemButtons/SCSystemButtons.h"
#include "Common/LayeredWindowHelperST.h"

// CTitleDlg 대화 상자

class CTitleDlg : public CDialog
{
	DECLARE_DYNAMIC(CTitleDlg)

public:
	CTitleDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTitleDlg();

	int					get_titlebar_height() const { return m_titlebar_height; }
	LRESULT				on_message_CSCSystemButtons(WPARAM wParam, LPARAM lParam);
	void				update_title(CString title);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TITLE };
#endif

protected:
	CSCColorTheme		m_theme;
	int					m_titlebar_height = 32;
	CSCSystemButtons	m_sys_buttons;
	HICON				m_hIcon;
	LOGFONT				m_titlebar_lf;
	CFont				m_titlebar_font;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
};
