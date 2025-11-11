#pragma once
#include "afxdialogex.h"

#include "Common/CEdit/SCEdit/SCEdit.h"

// CRoiInputDlg 대화 상자

class CRoiInputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRoiInputDlg)

public:
	CRoiInputDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CRoiInputDlg();

	void		init(int img_width, int img_height, Gdiplus::RectF roi);

	Gdiplus::RectF m_roi;
	CSize		m_sz_img;	//이미지 크기

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
	virtual BOOL OnInitDialog();
	CSCEdit m_edit_img_cx;
	CSCEdit m_edit_img_cy;
	CSCEdit m_edit_roi_x1;
	CSCEdit m_edit_roi_y1;
	CSCEdit m_edit_roi_x2;
	CSCEdit m_edit_roi_y2;
	CSCEdit m_edit_roi_cx;
	CSCEdit m_edit_roi_cy;
	afx_msg void OnEnChangeEditRoiX1();
	afx_msg void OnEnChangeEditRoiY1();
	afx_msg void OnEnChangeEditRoiX2();
	afx_msg void OnEnChangeEditRoiY2();
	afx_msg void OnEnChangeEditRoiCx();
	afx_msg void OnEnChangeEditRoiCy();
};
