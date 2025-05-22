#pragma once
#include "afxdialogex.h"


// CRoiInputDlg 대화 상자

class CRoiInputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRoiInputDlg)

public:
	CRoiInputDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CRoiInputDlg();

	void		init(int img_width, int img_height, int roi_cx, int roi_cy);

	CSize		m_img_sz;	//이미지 크기
	CSize 		m_roi_sz;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROI_INPUT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_edit_cx;
	CSpinButtonCtrl m_spin_cx;
	CEdit m_edit_cy;
	CSpinButtonCtrl m_spin_cy;
	virtual BOOL OnInitDialog();
};
