#pragma once
#include "afxdialogex.h"

#include "Common/CSliderCtrl/SCSliderCtrl/SCSliderCtrl.h"
#include "Common/CEdit/CSCStaticEdit/SCStaticEdit.h"

#include <atomic>
#include <thread>

// CExportSvgToGifDlg 대화 상자
// SVG 애니메이션 → 애니메이션 GIF 내보내기. 확인을 누르면 워커 스레드에서 베이킹/인코딩을
// 수행하고, 진행 상황을 슬라이더(진행바 스타일)로 표시한다. 모달이라 부모(메인 dlg)는
// 자동으로 비활성화되어 작업 중 어떤 동작도 받지 않는다.

class CExportSvgToGifDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CExportSvgToGifDlg)

public:
	CExportSvgToGifDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CExportSvgToGifDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXPORT_SVG_TO_GIF };
#endif

	// ── 호출부가 DoModal 전에 설정 ──────────────────────────────
	CString		m_svg_path;			// 원본 SVG 전체 경로(필수)
	CString		m_out_gif_path;		// 출력 GIF 경로(빈 값이면 svg 폴더·이름.gif)
	int			m_out_w = 200;		// 출력 크기(추후 입력 UI 예정). <= 0 이면 SVG 자연 크기
	int			m_out_h = 200;
	int			m_fps   = 50;		// GIF 부드러움(내부 [1,50] 클램프)

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	virtual void OnOK();			// 확인 → 내보내기 시작(닫지 않음)
	virtual void OnCancel();		// 취소/Esc → 실행 중이면 중단 요청, 아니면 닫기

	afx_msg void    OnClose();		// X 버튼도 취소 로직으로
	afx_msg LRESULT on_export_progress(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT on_export_done(WPARAM wParam, LPARAM lParam);
	afx_msg void    on_check_ratio();							// 비율 유지 체크 → 높이 재계산
	afx_msg LRESULT on_message_static_edit(WPARAM wParam, LPARAM lParam);	// width/height 라이브 비율

	DECLARE_MESSAGE_MAP()

public:
	CSCSliderCtrl m_slider_progress;

private:
	std::thread			m_worker;
	std::atomic<bool>	m_cancel{ false };
	bool				m_running = false;	// 워커 실행 중(UI 스레드에서만 접근)
	int					m_result  = 0;		// 0 성공, 1 실패, 2 취소
	double				m_aspect  = 1.0;	// SVG 자연 종횡비(w/h). 비율 유지에 사용
	bool				m_sync    = false;	// width↔height 상호 갱신 재진입 가드
public:
	CButton m_radio_webp;
	CButton m_radio_apng;
	CButton m_radio_gif;
	CSCStaticEdit m_edit_width;
	CSCStaticEdit m_edit_height;
	CButton m_check_ratio;
	CSCStaticEdit m_edit_ratio;
	CSCStaticEdit m_edit_fps;
};
