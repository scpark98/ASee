#pragma once

//20260728 by claude. 몽타주 스냅샷 셀 더블클릭 → OCR 시각 → 매핑된 미디어 재생 헬퍼.
//규칙: 스냅샷 파일명은 "<media_full_filename_with_ext>.jpg" 또는 ".png" (Elysium 규약).
//예) video.mpg → video.mpg.jpg / video.mpg.png.

#include <vector>
#include <afxwin.h>

class CSCD2Image;

//image_path 가 "<media>.jpg" 또는 "<media>.png" 형태이고 <media> 파일이 존재하면 true.
//out_media 에 매핑된 미디어 파일 전체 경로.
bool	sc_is_snapshot_pair(const CString& image_path, CString& out_media);

//셀 하단 띠(78% ~ 100%) 를 여러 밴드패스로 이진화하여 sc_win_ocr 로 인식,
//유효 HH<24/MM<60/SS<60 결과 중 최빈값 채택. 실패 시 "".
//입력: img = 몽타주 이미지, cell = 원본 픽셀 좌표계의 셀 rect (right/bottom exclusive).
CString	sc_recognize_cell_time(CSCD2Image& img, CRect cell);

//detect_grid 결과(vsep/hsep, 구분선 좌표) + (r,c) → 원본 픽셀 좌표 셀 사각형.
CRect	sc_cell_rect(const std::vector<int>& vsep, const std::vector<int>& hsep, int r, int c);

//"HH:MM:SS" → 초. Left(2)/Mid(3,2)/Mid(6,2).
int		sc_hms_to_sec(const CString& hms);

//"HH:MM:SS" 또는 "HH:MM:SS.mmm" → 총 ms. '.' 뒤 3자리를 ms 로.
int		sc_hms_to_ms(const CString& hms);
