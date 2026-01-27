#pragma once
#include "afxdialogex.h"

#include "Common/CDialog/SCThemeDlg/SCThemeDlg.h"

// CTitleDlg 대화 상자

class CTitleDlg : public CSCThemeDlg
{
	DECLARE_DYNAMIC(CTitleDlg)

public:
	CTitleDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTitleDlg();

	void		update_title(CString title);
	int			get_titlebar_height();
	bool		is_in_sliding() { return m_sliding; }

	enum TIMER_ID
	{
		timer_sliding_show = 0,
	};
	void		sliding_show(bool show);

	//일반적인 dlg라면 GetParent()로 parent를 얻어오면 되지만
	//이 앱의 경우는 일반 화면일때는 mainDlg가 parent가 되지만
	//전체 화면 모드일 때는 titleDlg를 popup으로 전환하므로 GetParent()로 얻어올 수 없다.
	//popup으로 전환하는 이유는 CSCImage2dDlg가 Direct2D 기반이라 그런지 zorder를 조정해도 항상 titlebar를 가리는 현상이 있다.
	//이 앱은 간단한 구조이므로 AfxGetMainWnd()로 mainDlg를 얻어올 수 있다.
	CWnd*		m_parent = NULL;
	//void		set_parent(CWnd* parent) { m_parent = parent; }

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TITLE };
#endif

protected:
	bool		m_sliding_show = false;
	bool		m_sliding = false;
	CPoint		m_cur_pt = 0;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
