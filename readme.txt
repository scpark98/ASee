[������ ����]
- Ctrl+Enter ��ü ����� ũ��� Ȯ��
- notice â�� fade �߿��� �ٷ� ����� �ڵ� �߰�
- thumb context �޴����� theme ����
- recalc_tile_rect()���� ���� ������ ���� ��� tile_gap.cx�� �÷��ְ� �ٽ� ����Ѵ�.

- indexed 8bpp�� ���� �̹����� ��� �ȷ�Ʈ�� ������ �����ؾ� �Ѵ�.
- webp ����
- roi resize, move�ÿ� �ȼ������� �̵��ǵ���.
- show/hide grid line
- 32bit png clipboard copy


[������ ����]
- minimize�� �� shell ���� �� restore �ȵ�
  �۾�ǥ���ٿ��� minimize �� ��(���� ����)�� �ۿ��� minimize �� ��(���� ����) hWnd�� ã��/��ã�� ���� �߻�.
  minimize ��ư�� ���� ���� �۾�ǥ���ٿ��� ����Ͽ� minimize�� ��� �ٸ� ����� ���ϵȴ�.
  ���� ����� ASee.exe�� �ڵ��� �����ϴµ� Gdi+ Window�� ������ ������ �߻��Ͽ�
  caption���� ã���� ������. ��, �� ���õ� ���� ����Ǵ� ASee.exe�� ���� �� �����Ƿ�
  visible�� ASee.exe�� �ڵ��� �������� ������.
- roi move, resize�ÿ� �ȼ������� �̵�.
- thumb view���� drag �� ��ũ��. cur.cy - old.cy��ŭ m_scroll_pos ����
- paste�ϸ� image view�� �ڵ� ��ȯ
- 1 line�� ���� edit�� bottom�� r.bottom�� �����ϰ� ����
- ������Ž���� ��� ���� ����, ����, �̸����� ���� ��ȭ�� ���� ��� �ٽ� �ε��ϵ��� ����. (CSCDirWatcher ���)
- animated gif�� ��� ������ ���� �� slider ǥ�� �ʿ�.


[screen_coord <-> image_coord ��ȯ�� ���� ���ս�]
- ��ǥ�� �������� ��ȯ���� ���ϸ� �Ǽ�. �̸� �ٽ� ��ǥ�� �����ϸ� ������ �ǹǷ� ���ս��� ��� �߻��Ѵ�.
  �̵�, ũ������ ���� ���� �� ��ǥ�� �Ǽ��� ��ȯ�ϰ� �ٽ� ������ ��ȯ�ϴ� �������� ���ս��� ��� �߻���.
- screen_coord, image_coord �� �� Gdiplus::RectF�� �Ͽ� 1000ȸ �ݺ� ��ȯ�ص� ���ս��� ���� ����.

[rubber band ���� �� ���ǻ���]
* OnLButtonDown() :
  - ������ ���� (m_screen_roi)
  - SetCapture() ȣ��

* OnMouseMove() :
  - ���콺 ���� ��ǥ�� ������ ������ ����� �ʵ��� ����.
  - �׸� �� right, bottom�� m_screen_roi�� �����ϰ� invalidate().
  - �� ������ �����ÿ��� ��ǥ�� �����ϰ� invalidate().

* OnLButtonUp() :
  - ReleaseCapture()
  - ���콺 ���� ��ǥ�� ������ ������ ����� �ʵ��� ����.
  - �̹� OnMouseMove()���� ���� ��ǥ�� ���������Ƿ� ���� ������ �ʿ� ����.
  - �׸����� left, right �Ǵ� top, bottom�� �ٲ�� ��쵵 �����Ƿ� ���⼭ nomalize_rect() ȣ��.
  - m_screen_roi�� m_image_roi�� ��ȯ�Ͽ� ����.
  - invalidate() ȣ��.
    
* OnPaint() :
  - m_lbutton_down�̸� m_screen_roi�� �׸���.
  - m_lbutton_down�� �ƴϰ� m_image_roi�� ��ȿ�ϸ�
    m_image_roi�� m_screen_roi�� ��ȯ�� �� draw_rectangle(m_screen_roi)�� �׸���.
    �̷��� �ϴ� ������ �̹����� resize�Ǿ� ǥ�õǾ roi�� ������ �ʾƾ� �ϱ� ����.
  - m_screen_roi�� ���� 9���� ���� �ڵ��� ���ϰ� �̸� �׷��ش�.
