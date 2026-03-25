#pragma once
#include "afxdialogex.h"
#include <gdiplus.h>
#include "Common/CStatic/SCStatic/SCStatic.h"

// CZigzagColorDlg 대화 상자

class CZigzagColorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CZigzagColorDlg)

public:
	CZigzagColorDlg(CWnd* pParent = nullptr, Gdiplus::Color cr_fore = 0, Gdiplus::Color cr_back = 0);
	virtual ~CZigzagColorDlg();

	Gdiplus::Color		get_fore_color();
	Gdiplus::Color		get_back_color();

	void				set_fore_color(Gdiplus::Color cr_fore);
	void				set_back_color(Gdiplus::Color cr_back);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ZIGZAG_COLOR };
#endif

protected :
	//default, white, back을 선택한 경우는 이미 명확하므로 m_cr_fore, m_cr_back은 갱신하지 않는다.
	//이 두 변수는 custom일 경우에만 변경된다.
	Gdiplus::Color		m_cr_fore;
	Gdiplus::Color		m_cr_back;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CButton m_radio_default;
	CButton m_radio_white;
	CButton m_radio_black;
	CButton m_radio_custom;
	afx_msg void OnBnClickedRadioDefault();
	afx_msg void OnBnClickedRadioWhite();
	afx_msg void OnBnClickedRadioBlack();
	afx_msg void OnBnClickedRadioCustom();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CSCStatic m_static_cr_fore;
	CSCStatic m_static_cr_back;
	afx_msg void OnStnClickedStaticCrFore();
	afx_msg void OnStnClickedStaticCrBack();
};
