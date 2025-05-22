// RoiInputDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ASee.h"
#include "afxdialogex.h"
#include "RoiInputDlg.h"

#include "../Common/Functions.h"

// CRoiInputDlg 대화 상자

IMPLEMENT_DYNAMIC(CRoiInputDlg, CDialogEx)

CRoiInputDlg::CRoiInputDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ROI_INPUT, pParent)
{

}

CRoiInputDlg::~CRoiInputDlg()
{
}

void CRoiInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CX, m_edit_cx);
	DDX_Control(pDX, IDC_SPIN_CX, m_spin_cx);
	DDX_Control(pDX, IDC_EDIT_CY, m_edit_cy);
	DDX_Control(pDX, IDC_SPIN_CY, m_spin_cy);
}


BEGIN_MESSAGE_MAP(CRoiInputDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRoiInputDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRoiInputDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CRoiInputDlg 메시지 처리기
BOOL CRoiInputDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_spin_cx.SetRange(1, m_img_sz.cx);
	m_spin_cy.SetRange(1, m_img_sz.cy);
	m_edit_cx.SetWindowText(i2S(m_roi_sz.cx));
	m_edit_cy.SetWindowText(i2S(m_roi_sz.cy));
	m_spin_cx.SetPos(m_roi_sz.cx);
	m_spin_cy.SetPos(m_roi_sz.cy);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CRoiInputDlg::OnBnClickedOk()
{
	CString str;
	m_edit_cx.GetWindowText(str);
	m_roi_sz.cx = _ttoi(str);
	m_edit_cy.GetWindowText(str);	
	m_roi_sz.cy = _ttoi(str);

	CDialogEx::OnOK();
}

void CRoiInputDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}

void CRoiInputDlg::init(int img_width, int img_height, int cx, int cy)
{
	m_img_sz = CSize(img_width, img_height);
	m_roi_sz = CSize(cx, cy);
}
