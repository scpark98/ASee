// ExportSvgToGifDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ASee.h"
#include "afxdialogex.h"
#include "ExportSvgToGifDlg.h"

#include "Common/SCSvg.h"

#include <string>

// 워커 스레드 → UI 스레드 통신용 메시지.
//   WM_EXPORT_PROGRESS : wParam=현재 프레임(1-base), lParam=총 프레임
//   WM_EXPORT_DONE     : wParam=결과(0 성공, 1 실패, 2 취소)
#define WM_EXPORT_PROGRESS  (WM_APP + 10)
#define WM_EXPORT_DONE      (WM_APP + 11)

// 출력 포맷(라디오). APNG 는 아직 미지원(라디오 비활성).
enum { FMT_WEBP = 0, FMT_APNG = 1, FMT_GIF = 2 };


// CExportSvgToGifDlg 대화 상자

IMPLEMENT_DYNAMIC(CExportSvgToGifDlg, CDialogEx)

CExportSvgToGifDlg::CExportSvgToGifDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EXPORT_SVG_TO_GIF, pParent)
{

}

CExportSvgToGifDlg::~CExportSvgToGifDlg()
{
	// 안전망: 파괴 시 워커가 아직 살아있으면 중단 요청 후 합류.
	if (m_worker.joinable())
	{
		m_cancel = true;
		m_worker.join();
	}
}

void CExportSvgToGifDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_PROGRESS, m_slider_progress);
	DDX_Control(pDX, IDC_RADIO_WEBP, m_radio_webp);
	DDX_Control(pDX, IDC_RADIO_APNG, m_radio_apng);
	DDX_Control(pDX, IDC_RADIO_GIF, m_radio_gif);
	DDX_Control(pDX, IDC_STATIC_WIDTH, m_edit_width);
	DDX_Control(pDX, IDC_STATIC_HEIGHT, m_edit_height);
	DDX_Control(pDX, IDC_CHECK_RATIO, m_check_ratio);
	DDX_Control(pDX, IDC_STATIC_RATIO, m_edit_ratio);
	DDX_Control(pDX, IDC_STATIC_FPS, m_edit_fps);
}


BEGIN_MESSAGE_MAP(CExportSvgToGifDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_EXPORT_PROGRESS, &CExportSvgToGifDlg::on_export_progress)
	ON_MESSAGE(WM_EXPORT_DONE,     &CExportSvgToGifDlg::on_export_done)
	ON_BN_CLICKED(IDC_CHECK_RATIO, &CExportSvgToGifDlg::on_check_ratio)
	ON_REGISTERED_MESSAGE(Message_CSCStaticEdit, &CExportSvgToGifDlg::on_message_static_edit)
END_MESSAGE_MAP()


// CExportSvgToGifDlg 메시지 처리기

BOOL CExportSvgToGifDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_slider_progress.set_style(CSCSliderCtrl::style_progress);
	m_slider_progress.set_range(0, 100);
	m_slider_progress.set_pos(0);

	// SVG 자연 크기로 종횡비 산출(비율 유지·기본 높이 계산에 사용).
	double nw = 0.0, nh = 0.0;
	{
		sc_svg probe;
		if (probe.load((LPCWSTR)m_svg_path))
		{
			nw = probe.natural_width();
			nh = probe.natural_height();
		}
	}
	if (nw < 1.0) nw = 1.0;
	if (nh < 1.0) nh = 1.0;
	m_aspect = nw / nh;

	// 포맷: 기본 WebP(풀컬러+알파, 품질 우선). APNG 는 아직 미지원 → 비활성.
	m_radio_webp.SetCheck(BST_CHECKED);
	m_radio_apng.EnableWindow(FALSE);

	// 크기: 가로 320 기준, 세로는 원본 비율로 자동.
	const int wdef = 320;
	int hdef = (int)(wdef / m_aspect + 0.5);
	if (hdef < 1) hdef = 1;
	m_edit_width.set_text(wdef);
	m_edit_height.set_text(hdef);

	m_check_ratio.SetCheck(BST_CHECKED);

	// 비율(w:h = 값:1). 입력 가능 — 사용자가 비율을 바꾸면 가로 기준으로 세로가 재계산된다.
	m_edit_ratio.set_textf(_T("%.2f"), m_aspect);

	m_edit_fps.set_text(25);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CExportSvgToGifDlg::on_check_ratio()
{
	// 비율 유지를 켜면 현재 width 기준으로 height 를 즉시 맞춘다.
	if (m_check_ratio.GetCheck() == BST_CHECKED)
	{
		m_sync = true;
		int wv = m_edit_width.get_int();
		int hv = (m_aspect > 0.0) ? (int)(wv / m_aspect + 0.5) : wv;
		if (hv < 1) hv = 1;
		m_edit_height.set_text(hv);
		m_sync = false;
	}
}

LRESULT CExportSvgToGifDlg::on_message_static_edit(WPARAM wParam, LPARAM /*lParam*/)
{
	CSCStaticEditMessage* m = (CSCStaticEditMessage*)wParam;
	if (!m || m->message != CSCStaticEdit::message_scstaticedit_text_changed)
		return 0;
	if (m_sync)
		return 0;

	// 비율 입력 → m_aspect 갱신 후 가로 기준으로 세로 재계산(체크 여부와 무관, 이게 이 edit 의 목적).
	if (m->pThis == &m_edit_ratio)
	{
		double r = m_edit_ratio.get_float();
		if (r > 0.0)
		{
			m_aspect = r;
			m_sync = true;
			int wv = m_edit_width.get_int();
			int hv = (int)(wv / m_aspect + 0.5);
			if (hv < 1) hv = 1;
			m_edit_height.set_text(hv);
			m_sync = false;
		}
		return 0;
	}

	// width/height 상호 갱신은 "비율 유지" 체크 시에만.
	if (m_check_ratio.GetCheck() != BST_CHECKED)
		return 0;

	// width 편집 → height 자동, height 편집 → width 자동(m_aspect 유지). m_sync 로 재진입 방지.
	if (m->pThis == &m_edit_width)
	{
		m_sync = true;
		int wv = m_edit_width.get_int();
		int hv = (m_aspect > 0.0) ? (int)(wv / m_aspect + 0.5) : wv;
		if (hv < 1) hv = 1;
		m_edit_height.set_text(hv);
		m_sync = false;
	}
	else if (m->pThis == &m_edit_height)
	{
		m_sync = true;
		int hv = m_edit_height.get_int();
		int wv = (int)(hv * m_aspect + 0.5);
		if (wv < 1) wv = 1;
		m_edit_width.set_text(wv);
		m_sync = false;
	}
	return 0;
}

void CExportSvgToGifDlg::OnOK()
{
	if (m_running)
		return;								// 이미 진행 중이면 무시(엔터 중복 방지)
	if (m_svg_path.IsEmpty())
	{
		EndDialog(IDCANCEL);
		return;
	}

	// 컨트롤에서 출력 파라미터 수집.
	int w = m_edit_width.get_int();  if (w < 1) w = 1;
	int h = m_edit_height.get_int(); if (h < 1) h = 1;
	if (m_check_ratio.GetCheck() == BST_CHECKED)		// 비율 유지 시 height 안전 재계산
	{
		h = (m_aspect > 0.0) ? (int)(w / m_aspect + 0.5) : h;
		if (h < 1) h = 1;
	}
	int fps = m_edit_fps.get_int(); if (fps < 1) fps = 1;
	const int fmt = (m_radio_gif.GetCheck() == BST_CHECKED) ? FMT_GIF : FMT_WEBP;

	m_running = true;
	m_cancel  = false;

	// 진행 중에는 확인 비활성화(취소만 가능).
	if (CWnd* ok = GetDlgItem(IDOK)) ok->EnableWindow(FALSE);

	// 워커로 넘길 값은 스레드 안전을 위해 std::wstring 으로 복사한다.
	const HWND hwnd = GetSafeHwnd();
	const std::wstring svg = (LPCWSTR)m_svg_path;
	const std::wstring out = (LPCWSTR)m_out_gif_path;	// 비어있으면 포맷별로 svg 폴더·이름 사용

	m_worker = std::thread([this, hwnd, svg, out, w, h, fps, fmt]()
	{
		auto progress_cb = [this, hwnd](int cur, int total) -> bool
		{
			::PostMessage(hwnd, WM_EXPORT_PROGRESS, (WPARAM)cur, (LPARAM)total);
			return !m_cancel;				// 취소 요청 시 export 중단
		};

		const wchar_t* outp = out.empty() ? nullptr : out.c_str();
		bool ok;
		if (fmt == FMT_GIF)
			ok = export_svg_to_animated_gif(svg.c_str(), outp, w, h, fps, progress_cb);
		else
			ok = export_svg_to_animated_webp(svg.c_str(), outp, w, h, fps, progress_cb);

		const int code = m_cancel ? 2 : (ok ? 0 : 1);
		::PostMessage(hwnd, WM_EXPORT_DONE, (WPARAM)code, 0);
	});
}

void CExportSvgToGifDlg::OnCancel()
{
	if (m_running)
	{
		// 진행 중: 닫지 말고 중단만 요청. 워커가 다음 progress 콜백에서 감지해 종료하면
		// WM_EXPORT_DONE 에서 다이얼로그를 닫는다.
		m_cancel = true;
		if (CWnd* c = GetDlgItem(IDCANCEL)) c->EnableWindow(FALSE);
		SetWindowText(_T("애니메이션 내보내기 - 취소 중..."));
		return;
	}
	CDialogEx::OnCancel();
}

void CExportSvgToGifDlg::OnClose()
{
	// X 버튼도 취소 로직을 따른다(진행 중이면 중단 요청, 아니면 닫기).
	OnCancel();
}

LRESULT CExportSvgToGifDlg::on_export_progress(WPARAM wParam, LPARAM lParam)
{
	const int cur   = (int)wParam;
	const int total = (int)lParam;
	if (total > 0)
	{
		m_slider_progress.set_range(0, total);
		m_slider_progress.set_pos(cur);

		CString cap;
		cap.Format(_T("애니메이션 내보내기 - %d%% (%d/%d)"), cur * 100 / total, cur, total);
		SetWindowText(cap);
	}
	return 0;
}

LRESULT CExportSvgToGifDlg::on_export_done(WPARAM wParam, LPARAM /*lParam*/)
{
	m_result = (int)wParam;

	if (m_worker.joinable())
		m_worker.join();				// PostMessage 직후라 즉시 반환
	m_running = false;

	if (m_result == 1)
		AfxMessageBox(_T("내보내기에 실패했습니다."), MB_ICONERROR);

	// 성공은 IDOK, 취소/실패는 IDCANCEL 로 닫는다.
	EndDialog(m_result == 0 ? IDOK : IDCANCEL);
	return 0;
}
