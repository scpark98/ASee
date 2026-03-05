// BackTransparencyDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ASee.h"
#include "ASeeDlg.h"
#include "afxdialogex.h"
#include "BackTransparencyDlg.h"

#include "Common/Functions.h"

// CBackTransparencyDlg 대화 상자

IMPLEMENT_DYNAMIC(CBackTransparencyDlg, CDialogEx)

CBackTransparencyDlg::CBackTransparencyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BACK_TRANSPARENCY, pParent)
{

}

CBackTransparencyDlg::~CBackTransparencyDlg()
{
}

void CBackTransparencyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_INNER_THRESHOLD, m_slider_inner_threshold);
	DDX_Control(pDX, IDC_SLIDER_OUTER_THRESHOLD, m_slider_outer_threshold);
	DDX_Control(pDX, IDC_STATIC_INNER_THRESHOLD, m_static_inner_threshold);
	DDX_Control(pDX, IDC_STATIC_OUTER_THRESHOLD, m_static_outer_threshold);
	DDX_Control(pDX, IDC_CHECK_AUTO_DETECT, m_check_auto_detect);
	DDX_Control(pDX, IDC_BUTTON_BACK_COLOR, m_button_back_color);
}


BEGIN_MESSAGE_MAP(CBackTransparencyDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CBackTransparencyDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CBackTransparencyDlg::OnBnClickedCancel)
	ON_WM_WINDOWPOSCHANGED()
	ON_REGISTERED_MESSAGE(Message_CSCSliderCtrl, &CBackTransparencyDlg::on_message_CSCSliderCtrl)
	ON_BN_CLICKED(IDC_CHECK_AUTO_DETECT, &CBackTransparencyDlg::OnBnClickedCheckAutoDetect)
	ON_BN_CLICKED(IDC_BUTTON_BACK_COLOR, &CBackTransparencyDlg::OnBnClickedButtonBackColor)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_TO_ALL_FRAMES, &CBackTransparencyDlg::OnBnClickedButtonApplyToAllFrames)
END_MESSAGE_MAP()


// CBackTransparencyDlg 메시지 처리기
BOOL CBackTransparencyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	RestoreWindowPosition(AfxGetApp(), this, _T("BackTransparencyDlg"), false, false);

	m_inner_threshold = AfxGetApp()->GetProfileInt(_T("setting\\remove background"), _T("m_inner_threshold"), 30);
	m_outer_threshold = AfxGetApp()->GetProfileInt(_T("setting\\remove background"), _T("m_outer_threshold"), 120);
	
	m_static_inner_threshold.set_use_edit(true, ES_RIGHT);
	m_static_outer_threshold.set_use_edit();
	m_static_inner_threshold.set_text_value(_T("%d"), m_inner_threshold);
	m_static_outer_threshold.set_text_value(_T("%d"), m_outer_threshold);

	m_slider_inner_threshold.set_style(CSCSliderCtrl::style_thumb_round);
	m_slider_outer_threshold.set_style(CSCSliderCtrl::style_thumb_round);

	m_slider_inner_threshold.SetRange(0, 255);
	m_slider_inner_threshold.SetPos(m_inner_threshold);
	m_slider_outer_threshold.SetRange(0, 255);
	m_slider_outer_threshold.SetPos(m_outer_threshold);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CBackTransparencyDlg::OnBnClickedOk()
{
}

void CBackTransparencyDlg::OnBnClickedCancel()
{
	ShowWindow(SW_HIDE);
}

void CBackTransparencyDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SaveWindowPosition(AfxGetApp(), this, _T("BackTransparencyDlg"));
}

void CBackTransparencyDlg::show_window(int nCmdShow)
{
	ShowWindow(nCmdShow);

	//현재 설정값을 CASeeDlg에 적용
	if (m_auto_detect)
		m_cr_back = Gdiplus::Color::Transparent;
	else
		m_cr_back.SetFromCOLORREF(m_button_back_color.GetColor());
	//((CASeeDlg*)(AfxGetApp()->GetMainWnd()))->set_back_transparency(m_target_index, (float)m_inner_threshold, (float)m_outer_threshold, m_cr_back);
}

LRESULT CBackTransparencyDlg::on_message_CSCSliderCtrl(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSCSliderCtrlMsg* msg = (CSCSliderCtrlMsg*)wParam;
	if (msg->pThis == &m_slider_inner_threshold)
	{
		m_inner_threshold = msg->pos;
		m_static_inner_threshold.set_text_value(_T("%d"), m_inner_threshold);
	}
	else if (msg->pThis == &m_slider_outer_threshold)
	{
		m_outer_threshold = msg->pos;
		m_static_outer_threshold.set_text_value(_T("%d"), m_outer_threshold);
	}

	if (m_auto_detect)
		m_cr_back = Gdiplus::Color::Transparent;
	else
		m_cr_back.SetFromCOLORREF(m_button_back_color.GetColor());
	((CASeeDlg*)(AfxGetApp()->GetMainWnd()))->set_back_transparency(m_target_index, (float)m_inner_threshold, (float)m_outer_threshold, m_cr_back);

	return 0;
}

void CBackTransparencyDlg::OnBnClickedCheckAutoDetect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_check_auto_detect.GetCheck() == BST_CHECKED)
	{
		m_auto_detect = true;
		m_cr_back = Gdiplus::Color::Transparent;

		Gdiplus::Color cr_detected = ((CASeeDlg*)(AfxGetApp()->GetMainWnd()))->detect_back_color();
		m_button_back_color.SetColor(cr_detected.ToCOLORREF());
	}
	else
	{
		m_auto_detect = false;
		m_cr_back.SetFromCOLORREF(m_button_back_color.GetColor());
	}
	((CASeeDlg*)(AfxGetApp()->GetMainWnd()))->set_back_transparency(m_target_index, (float)m_inner_threshold, (float)m_outer_threshold, m_cr_back);
}

void CBackTransparencyDlg::OnBnClickedButtonBackColor()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_auto_detect)
	{
		m_cr_back.SetFromCOLORREF(m_button_back_color.GetColor());
		((CASeeDlg*)(AfxGetApp()->GetMainWnd()))->set_back_transparency(m_target_index, (float)m_inner_threshold, (float)m_outer_threshold, m_cr_back);
	}
}

void CBackTransparencyDlg::OnBnClickedButtonApplyToAllFrames()
{
	((CASeeDlg*)(AfxGetApp()->GetMainWnd()))->set_back_transparency(-1, (float)m_inner_threshold, (float)m_outer_threshold, m_cr_back);
}
