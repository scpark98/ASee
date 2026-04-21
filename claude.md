# ASee 프로젝트

공통 규칙은 홈 폴더 `~/.claude/claude.md` symlink 를 통해 `Common/claude.md` 에서 자동 로드된다. 이 파일은 ASee **고유 사항**만 기록한다.

## 프로젝트 개요

- 이미지 뷰어 (MFC, C++)
- 경로: `D:\1.projects_c++\ASee` (집·회사 동일)

## 아키텍처 포인트

### 주요 Common 의존

- `Common/CDialog/SCD2ImageDlg/SCD2ImageDlg.h` → `CSCD2ImageDlg m_imgDlg` (이미지 표시 핵심, `ASeeDlg.h:9, 87`)
- `Common/CDialog/SCShapeDlg/SCShapeDlg.h`
- `Common/ThumbCtrl/SCThumbCtrl.h`
- `Common/file_system/SCDirWatcher/SCDirWatcher.h`
- `Common/CStatic/SCStatic`, `Common/CEdit/SCEdit`, `Common/CSliderCtrl/SCSliderCtrl`
- `Common/CButton/SCSystemButtons`, `Common/LayeredWindowHelperST.h` 등

### zoom 영속화 흐름

1. `CASeeDlg` 의 키·메뉴 (`VK_ADD`, `OnMenuZoom120/150/200`, `OnMouseWheel`) → `m_imgDlg.zoom(...)` 호출
2. `CSCD2ImageDlg::zoom()` 내부에서 `fit2ctrl(false, false)` 호출
3. `CSCD2ImageDlg::fit2ctrl()` 이 `m_zoom` 을 레지스트리 `setting\CSCD2ImageDlg` 에 저장

## 결정사항 / 최근 이슈

- **2026-04-21**: zoom 배율 레지스트리 복원 버그 해결. 원인은 프로그램 시작 시에도 `fit2ctrl` 이 호출되면서 저장값을 덮어쓴 것. 시작 시점 저장 호출 제거로 수정됨.
