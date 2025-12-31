#pragma once
#include "afxdialogex.h"


// CZigzagColorDlg 대화 상자

class CZigzagColorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CZigzagColorDlg)

public:
	CZigzagColorDlg(CWnd* pParent = nullptr, COLORREF cr_back = 0, COLORREF cr_fore = 0);   // 표준 생성자입니다.
	virtual ~CZigzagColorDlg();

	Gdiplus::Color		get_back_color();
	Gdiplus::Color		get_fore_color();

	void				set_back_color(COLORREF cr_back);
	void				set_fore_color(COLORREF cr_fore);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ZIGZAG_COLOR };
#endif

protected :
	COLORREF		m_cr_back;
	COLORREF		m_cr_fore;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CMFCColorButton m_button_back_color;
	CMFCColorButton m_button_fore_color;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonBackColor();
	afx_msg void OnBnClickedButtonForeColor();
	CButton m_radio_default;
	CButton m_radio_white;
	CButton m_radio_black;
	afx_msg void OnBnClickedRadioDefault();
	afx_msg void OnBnClickedRadioWhite();
	afx_msg void OnBnClickedRadioBlack();
	CButton m_radio_custom;
	afx_msg void OnBnClickedRadioCustom();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
