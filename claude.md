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

- **2026-07-29**: SVG 파일 지원 추가 (lunasvg v3.5.0 + plutovg 정적 링크).
  - **벤더 위치**: `Common/3rdparty/lunasvg/` (git 동기화). 원본 클론 위치(`D:\0.Install\develop\lunasvg`)는 다른 머신에 없으므로 Common repo 안으로 복사함. lunasvg/source `*.cpp` 10개 + plutovg/source `*.c` 11개.
  - **빌드**: ASee.vcxproj 에 위 소스들을 ClCompile(PCH `NotUsing`) 로 추가. include dir 2개(lunasvg/include, plutovg/include), 전처리기 정의 `SC_USE_SVG;LUNASVG_BUILD_STATIC;PLUTOVG_BUILD_STATIC;LUNASVG_DISABLE_LOAD_SYSTEM_FONTS` (4개 config 모두). Debug|x64 빌드 검증 완료(오류 0).
  - **opt-in 게이트 `SC_USE_SVG` (2026-07-29)**: `CSCGdiplusBitmap`·`CSCD2Image` 는 거의 모든 프로젝트가 쓰는 Common 클래스라, SVG 코드를 무조건 넣으면 *모든* 프로젝트가 lunasvg include 경로·벤더 소스·매크로를 설정해야 한다. 그래서 SVG 관련 코드(멤버 `sc_svg m_svg`, `#include SCSvg.h`, load_svg/rasterize/ensure, get_width/height/size 의 SVG 분기, OnPaint 재래스터 훅, 썸네일 svg 분기)를 전부 **`#ifdef SC_USE_SVG`** 로 감쌌다. **기본 OFF** — `SC_USE_SVG` 미정의 프로젝트는 아무 설정 없이 기존대로 빌드되고 lunasvg 의존이 0이다. `is_svg()` 만 미지원 빌드에서도 `false` 반환 스텁을 둬 호출부가 컴파일된다. **다른 프로젝트에서 SVG 를 켜려면**: (1) 전처리기에 `SC_USE_SVG` + lunasvg 4매크로 추가, (2) include dir 2개 추가, (3) `Common/SCSvg.cpp` + lunasvg 10 cpp + plutovg 11 c 를 프로젝트에 추가(PCH NotUsing). 게이트 파일: `SCD2Image.h/.cpp`, `SCGdiplusBitmap.h/.cpp`, `SCD2ImageDlg.cpp`, `SCThumbCtrl.cpp`.
  - **래스터라이저 래퍼**: `Common/SCSvg.h/.cpp` 의 `sc_svg` — Document 파싱 보관 + `render(w,h)`. lunasvg 는 **premultiplied** ARGB(메모리상 BGRA)를 내는데, `CSCD2Image::load(raw,...,channel=4)` 와 GDI+ 는 **straight** BGRA 를 기대하므로 render() 안에서 un-premultiply 한다. 한글 경로 대응으로 wide-path ifstream 으로 바이트를 읽어 `loadFromData` 사용.
  - **핵심 설계 — 논리/래스터 분리(zoom 재래스터화, 방식 B)**: SVG 는 벡터라 확대 시 선명 유지를 위해 표시 배율마다 재래스터화한다. 단 `CSCD2Image::get_width/height/get_size` 는 **논리(자연) 크기(고정)** 를 반환(`m_svg_logical_w/h`)하여 상위 zoom 수식(`m_zoom = display/get_width`)이 흔들리지 않게 하고, 백킹 비트맵(`m_img[0]`) 해상도만 배율에 맞춰 교체한다. 재래스터 트리거는 `CSCD2ImageDlg::OnPaint` 에서 `m_r_display` 확정 후 `ensure_svg_raster(display px)` 호출(확대 & 현재 대비 1.5배 이상일 때만, 상한 8192px).
  - **stale 방지**: 일반 WIC 디코드 경로와 일반 raw 로드에서 `m_svg.clear()` 호출. SVG 재래스터가 부르는 raw 로드만 `is_svg_raster=true` 로 보존.
  - **썸네일**: `CSCGdiplusBitmap::load_svg(path, long_side)` 추가(고정 크기 1회 렌더 → `Gdiplus::Bitmap`). `SCThumbCtrl::insert` 에 svg 분기.
  - **확장자 필터**: `Functions.h` `FILE_EXTENSION_IMAGE` 에는 이미 `svg` 포함(폴더 탐색 OK)이었음. 열기 대화상자 필터(`ASeeDlg.cpp` `OnMenuOpen`)에만 `*.svg` 추가. 저장은 SVG 출력 미지원이라 Save 필터는 그대로.
  - **메모리 릭 해결 (2026-07-29)**: SVG 파일 1개만 열어도 종료 시 수백 개 블록 + GDEF 폰트 ~2MB 릭 발생. 원인은 lunasvg 의 전역 폰트 캐시 — `graphics.cpp` 의 함수-로컬 static `FontFaceCache cache` 가 생성자에서 `plutovg_font_face_cache_load_sys()` 로 **설치된 시스템 폰트 전체를 로드**하는데, `FontFaceCache` 에 **소멸자가 없어**(graphics.h) 프로세스 종료까지 해제 안 됨(1회성·비증가 릭). 해결: 벤더 라이브러리가 제공하는 컴파일 매크로 **`LUNASVG_DISABLE_LOAD_SYSTEM_FONTS`** 를 vcxproj 4개 config 전처리기에 추가해 시스템 폰트 로드를 끔(우리 SVG 는 아이콘 위주라 텍스트 불필요). 벤더 소스는 수정하지 않음.
  - **알려진 한계**: 텍스트 포함 SVG 는 글자 미표시(시스템 폰트 로드를 끔 — 필요 시 `lunasvg_add_font_face_from_file` 로 특정 폰트만 등록). 픽셀 색상 읽기(get_pixel)는 논리 좌표 기준이라 고해상도 래스터와 미세 오차 가능. 테스트 파일: `ASee/test_sample.svg`.
