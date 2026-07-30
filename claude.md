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
  - **알려진 한계**: 픽셀 색상 읽기(get_pixel)는 논리 좌표 기준이라 고해상도 래스터와 미세 오차 가능. 테스트 파일: `ASee/test_sample.svg`.

- **2026-07-30**: SVG 텍스트 폰트 온디맨드 등록 (`Common/SCSvg.cpp`).
  - 시스템 폰트 전체 로드(`LUNASVG_DISABLE_LOAD_SYSTEM_FONTS` 로 OFF, 릭 방지)는 유지하되, **텍스트가 있는 SVG 를 로드할 때 그 파일이 참조하는 `font-family` 만** 추출해 등록한다. 릭은 필요한 소수 폰트만 로드하므로 무시할 수준(1-block).
  - 흐름: `sc_svg::load` 성공 → `register_svg_fonts(data)`. `<text>/<tspan>/<textPath>` 없으면 **폰트 미등록(zero-leak 유지)**. 있으면 `font-family` 속성/CSS 에서 이름 수집 → **DirectWrite**(`IDWriteFontCollection::FindFamilyName` → GetFont → GetFontFace → GetFiles → GetFilePath)로 실제 파일 경로 해석 → `lunasvg_add_font_face_from_file(family, bold, italic, path)`. generic(sans-serif→Arial 등)·미지정은 구체 폰트명으로 매핑. 프로세스 전역 `std::set` dedup, `std::mutex` 로 썸네일 워커 스레드 대비 직렬화. `dwrite.lib` 는 `#pragma comment` 로 링크(vcxproj 무수정).
  - **한계**: 한글 텍스트는 SVG 에 한글 폰트가 명시(`font-family="Malgun Gothic"`)돼야 표시. 미지정 텍스트는 Arial 폴백이라 한글 글리프 없음. `.ttc` 컬렉션은 face 0 만. 웹폰트/JS 미지원.

- **2026-07-30**: **애니메이션 SVG(SMIL) 지원 — 프레임 베이킹 방식 A** (`Common/SCSvg.cpp`, `SCD2Image.cpp`).
  - lunasvg 는 애니메이션을 재생 못 함(파싱 후 무시). 그래서 **SMIL 애니메이션을 우리가 직접 해석**해 시각 t 마다 대상 속성을 그 시점 값으로 써넣은 정적 스냅샷 SVG 를 만들고 lunasvg 로 한 장씩 래스터화 → **GIF 처럼 프레임 시퀀스로 재생**. 브라우저 의존 0.
  - **DOM 파서**: `Common/xml/pugixml`(이미 벤더링, `PUGIXML_WCHAR_MODE` — char_t=wchar_t). SCSvg 는 경계에서 UTF-8↔UTF-16 변환(`utf8_to_wide`/`wide_to_utf8` 재사용). narrow `ostream` 으로 `save` → UTF-8 바이트로 인코딩해 lunasvg 에 전달.
  - **API**: `sc_svg::is_animated()`(load 시 `<animate`/`<set` 잠정 감지), `sc_svg::build_frames(w,h,frames,delays)`(프레임별 straight BGRA + delay). 매 프레임 원본 재파싱(잔상 방지) → t 시점 값 주입 → 직렬화 → 래스터. 주기 T = max(begin+dur*repeat) 클램프 [0.2,8]s, 25fps, 최대 150프레임.
  - **재생 통합**: `CSCD2Image::load_svg` 가 `is_animated()` 면 `build_frames` 로 굽고 각 프레임을 **`add_frame_from_raw(...,delay)`** 로 넣은 뒤 `play()` — 기존 GIF 다중프레임 경로(`m_img` deque + `m_frame_delay` + `thread_animation`)를 그대로 사용. 굽기 실패 시 정적 1프레임 폴백. **`ensure_svg_raster` 는 애니메이션이면 early-return**(단일 재래스터가 프레임 deque 를 파괴하는 것 방지 — 애니메이션은 GIF 처럼 D2D 스케일, 확대 재래스터 없음).
  - **지원**: `<animate>`/`<animateColor>`/`<animateTransform>`/`<set>`, from·to·by / values;keyTimes, calcMode linear·discrete(spline→선형근사), repeatCount 수치·indefinite, fill freeze·remove, begin 수치 오프셋, href/xlink:href="#id" 또는 부모 대상. 색(fill/stroke 등) RGB 보간, 숫자/리스트/transform 인자 보간.
  - **미지원**: CSS `@keyframes`/`animation`, JS·상호작용 트리거(begin="click", :hover), `begin="other.end"` 동기 체인, `<animateMotion>`, keySplines 이징(선형 근사), additive="sum"/accumulate(부분), inline `style` 로 덮인 애니메이션 속성. 이런 파일은 첫 프레임 정적 표시로 폴백.
  - **다른 프로젝트에서 SVG 켤 때 추가 요건**: 기존 4항목(SC_USE_SVG+4매크로 / include dir 2개 / SCSvg.cpp+lunasvg 10 cpp+plutovg 11 c)에 더해 **(5) `Common/xml/pugixml/src/pugixml.cpp` 를 프로젝트에 추가(PCH NotUsing), (6) include dir `Common/xml/pugixml/src` 추가**. pugiconfig 의 `PUGIXML_WCHAR_MODE` 전제.
  - **빌드**: Debug|x64 컴파일 통과(0 오류). 링크는 실행 중 exe 잠금(LNK1168)이면 인스턴스 종료 후 재링크.

- **2026-07-30**: 애니메이션 SVG 로딩 성능 개선 (큰 파일 로딩 홀드/지연 대응).
  - **원인**: `build_frames` 가 프레임마다 (pugixml 재파싱 + **lunasvg 가 전체 SVG 재파싱** + 래스터)를 최대 150회 반복. 큰/복잡 SVG 는 lunasvg 재파싱·래스터가 프레임당 비싸 총 N배 폭증.
  - **① 파싱 1회화**: pugixml 을 프레임마다 재파싱하던 걸 1회 파싱 + base 값 스냅샷으로. 각 프레임은 애니메이션 대상 속성만 base 로 복원 후 t 값 주입(잔상 방지). (lunasvg 재파싱은 DOM 뮤테이션 API 부재로 프레임마다 불가피 — 그래서 N 축소가 핵심.)
  - **② 적응형 프레임 수**: 프레임 0 을 굽고 `QueryPerformanceCounter` 로 실측 → 총 베이킹 예산(1500ms)에 맞춰 `N = min(round(T*25), budget/frame_ms)`. 무거운 파일은 자동으로 프레임을 줄여 로딩 상한. `delay = T*1000/N` 로 보정해 총 재생시간=T 유지(속도 불변, 프레임만 성김).
  - **③ 래스터 해상도 캡**: `CSCD2Image::load_svg` 애니메이션 분기에서 베이킹 크기 long-side 를 **768px** 로 캡(움직이는 그림이라 소프트닝 무해, 표시는 D2D 업스케일). 픽셀당 래스터 비용 절감 → 확보 가능한 N 증가. 정적 SVG 경로(ensure_svg_raster 재래스터)는 영향 없음.
  - **남은 카드(미적용)**: 점진적 백그라운드 베이킹(프레임 0 즉시 표시·재생 + 워커가 BGRA 만 굽고 UI 스레드로 marshal 해 `add_frame_from_raw`, D2D 는 UI 스레드 유지) → 체감 로딩≈1프레임. 순수 회전류는 정적 배경 1회 렌더+애니 요소만 프레임별 합성. 필요 시 진행.

- **2026-07-30**: 폰트 포함 SVG 릭 해결 — lunasvg 폰트 캐시 소멸자 추가 (벤더 최소 패치).
  - **원인**: `graphics.h`/`graphics.cpp` 의 `FontFaceCache` 는 함수-로컬 static 싱글턴이고 `plutovg_font_face_cache_create()` 로 캐시를 만들지만 **소멸자가 없어** 우리가 온디맨드 등록한 폰트 페이스 + 로드 데이터(CJK 폰트는 수 MB)가 프로세스 종료까지 해제 안 됨 → 릭. 캐시가 **프로세스 전역**이라 "이미지 release 시 삭제"는 구조상 불가(공유 + remove API 없음).
  - **해결**: `FontFaceCache` 에 소멸자 추가 → `plutovg_font_face_cache_destroy(m_cache)` 호출. 함수-로컬 static 이라 프로세스 종료 시 자동 소멸하며 등록된 모든 폰트를 해제 → 릭 0. **벤더 소스 2줄 수정**(graphics.h 선언 + graphics.cpp 정의, ASCII 주석 `Local addition (not upstream)`). 이전 릭 대책(`LUNASVG_DISABLE_LOAD_SYSTEM_FONTS`)은 시스템 폰트 로드를 껐지만, 우리가 *의도적으로* 등록하는 폰트는 이 소멸자가 있어야 해제된다. Common repo 로 동기화되므로 양 머신 일관.
