[수정할 내용]
- indexed 8bpp와 같은 이미지일 경우 팔레트로 색상을 추출해야 한다.
- webp 지원


[수정된 내용]
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
