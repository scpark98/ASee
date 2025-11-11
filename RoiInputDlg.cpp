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
	DDX_Control(pDX, IDC_EDIT_IMG_CX, m_edit_img_cx);
	DDX_Control(pDX, IDC_EDIT_IMG_CY, m_edit_img_cy);
	DDX_Control(pDX, IDC_EDIT_ROI_X1, m_edit_roi_x1);
	DDX_Control(pDX, IDC_EDIT_ROI_Y1, m_edit_roi_y1);
	DDX_Control(pDX, IDC_EDIT_ROI_X2, m_edit_roi_x2);
	DDX_Control(pDX, IDC_EDIT_ROI_Y2, m_edit_roi_y2);
	DDX_Control(pDX, IDC_EDIT_ROI_CX, m_edit_roi_cx);
	DDX_Control(pDX, IDC_EDIT_ROI_CY, m_edit_roi_cy);
}


BEGIN_MESSAGE_MAP(CRoiInputDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRoiInputDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRoiInputDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT_ROI_X1, &CRoiInputDlg::OnEnChangeEditRoiX1)
	ON_EN_CHANGE(IDC_EDIT_ROI_Y1, &CRoiInputDlg::OnEnChangeEditRoiY1)
	ON_EN_CHANGE(IDC_EDIT_ROI_X2, &CRoiInputDlg::OnEnChangeEditRoiX2)
	ON_EN_CHANGE(IDC_EDIT_ROI_Y2, &CRoiInputDlg::OnEnChangeEditRoiY2)
	ON_EN_CHANGE(IDC_EDIT_ROI_CX, &CRoiInputDlg::OnEnChangeEditRoiCx)
	ON_EN_CHANGE(IDC_EDIT_ROI_CY, &CRoiInputDlg::OnEnChangeEditRoiCy)
END_MESSAGE_MAP()


// CRoiInputDlg 메시지 처리기
BOOL CRoiInputDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_edit_img_cx.set_text(m_sz_img.cx);
	m_edit_img_cy.set_text(m_sz_img.cy);

	m_edit_roi_x1.set_text((int)m_roi.X);
	m_edit_roi_y1.set_text((int)m_roi.Y);
	m_edit_roi_x2.set_text((int)m_roi.GetRight());
	m_edit_roi_y2.set_text((int)m_roi.GetBottom());
	m_edit_roi_cx.set_text((int)m_roi.Width);
	m_edit_roi_cy.set_text((int)m_roi.Height);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CRoiInputDlg::OnBnClickedOk()
{
	m_roi.X = _ttof(m_edit_roi_x1.get_text());
	m_roi.Y = _ttof(m_edit_roi_y1.get_text());
	m_roi.Width = _ttof(m_edit_roi_cx.get_text());
	m_roi.Height = _ttof(m_edit_roi_cy.get_text());

	CDialogEx::OnOK();
}

void CRoiInputDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}

void CRoiInputDlg::init(int img_width, int img_height, Gdiplus::RectF roi)
{
	m_sz_img = CSize(img_width, img_height);
	m_roi = roi;
}

void CRoiInputDlg::OnEnChangeEditRoiX1()
{
	//x1을 변경하면 width를 유지하고 x2를 변경한다?
	//x1을 변경하면 x2를 유지하고 width를 변경한다?
}

void CRoiInputDlg::OnEnChangeEditRoiY1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// ENM_CHANGE가 있으면 마스크에 ORed를 플래그합니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CRoiInputDlg::OnEnChangeEditRoiX2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// ENM_CHANGE가 있으면 마스크에 ORed를 플래그합니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CRoiInputDlg::OnEnChangeEditRoiY2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// ENM_CHANGE가 있으면 마스크에 ORed를 플래그합니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CRoiInputDlg::OnEnChangeEditRoiCx()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// ENM_CHANGE가 있으면 마스크에 ORed를 플래그합니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CRoiInputDlg::OnEnChangeEditRoiCy()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// ENM_CHANGE가 있으면 마스크에 ORed를 플래그합니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
