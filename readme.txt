[수정할 내용]
- Ctrl+Enter 전체 모니터 크기로 확대
- notice 창을 fade 중에도 바로 숨기는 코드 추가
- thumb context 메뉴에서 theme 선택
- recalc_tile_rect()에서 우측 여백이 남을 경우 tile_gap.cx를 늘려주고 다시 계산한다.

- indexed 8bpp와 같은 이미지일 경우 팔레트로 색상을 추출해야 한다.
- webp 지원
- roi resize, move시에 픽셀단위로 이동되도록.
- show/hide grid line
- 32bit png clipboard copy


[수정된 내용]
- minimize일 때 shell 실행 시 restore 안됨
  작업표시줄에서 minimize 할 때(정상 동작)와 앱에서 minimize 할 때(반응 없음) hWnd를 찾는/못찾는 차이 발생.
  minimize 버튼을 누른 경우와 작업표시줄에서 토글하여 minimize한 경우 다른 결과가 리턴된다.
  먼저 실행된 ASee.exe의 핸들을 얻어야하는데 Gdi+ Window를 얻어오는 오류가 발생하여
  caption으로 찾도록 수정함. 단, 이 역시도 나중 실행되는 ASee.exe를 얻어올 수 있으므로
  visible인 ASee.exe의 핸들을 얻어오도록 수정함.
- roi move, resize시에 픽셀단위로 이동.
- thumb view에서 drag 시 스크롤. cur.cy - old.cy만큼 m_scroll_pos 보정
- paste하면 image view로 자동 전환
- 1 line일 경우는 edit의 bottom을 r.bottom과 동일하게 조정
- 윈도우탐색기 등에서 파일 생성, 삭제, 이름변경 등의 변화가 있을 경우 다시 로드하도록 수정. (CSCDirWatcher 사용)
- animated gif일 경우 프레임 정보 및 slider 표시 필요.


[screen_coord <-> image_coord 변환에 의한 값손실]
- 좌표는 정수지만 변환식을 통하면 실수. 이를 다시 좌표로 저장하면 정수가 되므로 값손실이 계속 발생한다.
  이동, 크기조정 등의 연산 시 좌표를 실수로 변환하고 다시 정수로 변환하는 과정에서 값손실이 계속 발생함.
- screen_coord, image_coord 둘 다 Gdiplus::RectF로 하여 1000회 반복 변환해도 값손실은 거의 없음.

[rubber band 구현 시 주의사항]
* OnLButtonDown() :
  - 시작점 저장 (m_screen_roi)
  - SetCapture() 호출

* OnMouseMove() :
  - 마우스 현재 좌표가 정해진 영역을 벗어나지 않도록 보정.
  - 그릴 때 right, bottom을 m_screen_roi에 저장하고 invalidate().
  - 각 제어점 조절시에도 좌표를 저장하고 invalidate().

* OnLButtonUp() :
  - ReleaseCapture()
  - 마우스 현재 좌표가 정해진 영역을 벗어나지 않도록 보정.
  - 이미 OnMouseMove()에서 최종 좌표를 저장했으므로 따로 저장할 필요 없음.
  - 그릴때는 left, right 또는 top, bottom이 바뀌는 경우도 있으므로 여기서 nomalize_rect() 호출.
  - m_screen_roi를 m_image_roi로 변환하여 저장.
  - invalidate() 호출.
    
* OnPaint() :
  - m_lbutton_down이면 m_screen_roi를 그린다.
  - m_lbutton_down이 아니고 m_image_roi가 유효하면
    m_image_roi를 m_screen_roi로 변환한 후 draw_rectangle(m_screen_roi)를 그린다.
    이렇게 하는 이유는 이미지가 resize되어 표시되어도 roi가 변하지 않아야 하기 때문.
  - m_screen_roi에 대한 9개의 조절 핸들을 구하고 이를 그려준다.
