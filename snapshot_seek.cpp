#include "pch.h"
#include "snapshot_seek.h"
#include <map>
#include <Shlwapi.h>
#include "Common/Functions.h"
#include "Common/SCGdiplusBitmap.h"
#include "Common/directx/CSCD2Image/SCD2Image.h"
#include "Common/ocr/SCWinOcr.h"

//Elysium 의 getSnapshotImageFileList 역방향: image_path 가 몽타주 규약(<media>.jpg|.png) 인지 판정.
//"<media>.<ext>" 에서 마지막 확장자 제거 후 그 이름의 파일이 존재해야 미디어로 인정한다.
bool sc_is_snapshot_pair(const CString& image_path, CString& out_media)
{
	CString ext = get_part(image_path, fn_ext);
	ext.MakeLower();
	if (ext != _T("jpg") && ext != _T("png"))
		return false;

	//"foo.mp4.jpg" 에서 ".jpg" 제거 → "foo.mp4".
	CString candidate = image_path.Left(image_path.GetLength() - ext.GetLength() - 1);
	if (!PathFileExists(candidate))
		return false;

	out_media = candidate;
	return true;
}

CString sc_recognize_cell_time(CSCD2Image& img, CRect cell)
{
	if (!img.is_valid())
		return _T("");
	if (cell.Width() <= 0 || cell.Height() <= 0)
		return _T("");

	//시간값은 셀 하단 중앙에 박힌다. 하단 띠만 대상으로 한다.
	CRect band(cell.left, cell.top + (int)(cell.Height() * 0.78), cell.right, cell.bottom);

	Gdiplus::Bitmap* sub = img.get_sub_gdiplus(band);
	if (sub == nullptr)
		return _T("");
	CSCGdiplusBitmap base(sub);
	delete sub;

	//배경 밝기가 셀마다 천차만별이라(어두운 장면~흰 침대시트~검정 트랙) 단일 임계로는 안 된다.
	//여러 밴드패스로 이진화해 OCR → 유효 시각(HH<24/MM<60/SS<60) 중 최빈값 채택.
	const int bands[][2] = { {150,255}, {170,255}, {185,255}, {165,225}, {180,235}, {195,245} };

	std::map<CString, int> votes;
	for (const auto& bnd : bands)
	{
		CSCGdiplusBitmap crop;
		base.deep_copy(&crop);
		crop.binarize(bnd[0], bnd[1]);
		crop.resize_canvas(crop.width + 48, crop.height + 48, DT_CENTER | DT_VCENTER, Gdiplus::Color::White);
		crop.resize(2.0f, 2.0f);

		CString text = sc_win_ocr(crop.m_pBitmap).c_str();

		CString digits;
		for (int i = 0; i < text.GetLength(); ++i)
		{
			TCHAR ch = text[i];
			if (ch >= '0' && ch <= '9')
				digits += ch;
		}
		//6자리 = 구형 HH:MM:SS, 9자리 = 신형 HH:MM:SS.mmm.
		int n = digits.GetLength();
		if (n != 6 && n != 9)
			continue;

		int hh = _ttoi(digits.Left(2));
		int mm = _ttoi(digits.Mid(2, 2));
		int ss = _ttoi(digits.Mid(4, 2));
		if (hh >= 24 || mm >= 60 || ss >= 60)
			continue;

		CString tm;
		if (n == 9)
			tm.Format(_T("%s:%s:%s.%s"), digits.Left(2), digits.Mid(2, 2), digits.Mid(4, 2), digits.Mid(6, 3));
		else
			tm.Format(_T("%s:%s:%s"), digits.Left(2), digits.Mid(2, 2), digits.Mid(4, 2));
		votes[tm]++;
	}

	CString best;
	int best_count = 0;
	for (const auto& kv : votes)
	{
		if (kv.second > best_count)
		{
			best_count = kv.second;
			best = kv.first;
		}
	}
	return best;
}

CRect sc_cell_rect(const std::vector<int>& vsep, const std::vector<int>& hsep, int r, int c)
{
	return CRect(vsep[c], hsep[r], vsep[c + 1], hsep[r + 1]);
}

int sc_hms_to_sec(const CString& hms)
{
	int h = _ttoi(hms.Left(2));
	int m = _ttoi(hms.Mid(3, 2));
	int s = _ttoi(hms.Mid(6, 2));
	return h * 3600 + m * 60 + s;
}

int sc_hms_to_ms(const CString& hms)
{
	int ms = 0;
	int dot = hms.Find(_T('.'));
	if (dot >= 0)
		ms = _ttoi(hms.Mid(dot + 1, 3));
	return sc_hms_to_sec(hms) * 1000 + ms;
}
