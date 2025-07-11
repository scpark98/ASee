
// ASee.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "ASee.h"
#include "ASeeDlg.h"

#include "../Common/Functions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CASeeApp

BEGIN_MESSAGE_MAP(CASeeApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CASeeApp 생성

CASeeApp::CASeeApp()
{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
	m_hMutex = NULL;
}

CASeeApp::~CASeeApp()
{
	if (m_hMutex)
	{
		::ReleaseMutex(m_hMutex);
		m_hMutex = NULL;
	}
}

// 유일한 CASeeApp 개체입니다.

CASeeApp theApp;


// CASeeApp 초기화

BOOL CASeeApp::InitInstance()
{
	SetRegistryKey(_T("Legends Software"));

	//AfxMessageBox(i2S(__argc));
	if (__argc > 1)
	{
		//AfxMessageBox(__targv[1]);
		WriteProfileString(_T("setting"), _T("shell parameter"), __targv[1]);
	}

	m_hMutex = ::CreateMutex(NULL, FALSE, _T("MUTEX_OF_ASee"));
	if (::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		HWND	hWnd;

		//중복실행 방지 및 기존 실행되는 프로그램이 minimized되어 있거나
		//inactive 상태이면 activate시키는 코드인데
		//GetHWndByExeFilename으로도 잘 얻어오지 못하는 듯하다.
		//따라서 dialog의 캡션 끝에 공백을 붙이고 FindWindow로 해당 윈도우를 찾아서
		//위의 액션을 수행하는 것이 가장 안전한듯하다.
		//참고로 공백을 붙이는 이유는 만약 윈도우 탐색기에서 현재 프로젝트 폴더를 열었다면
		//윈도우 탐색기의 타이틀도 이 프로젝트의 이름으로 표시되는 윈도우 버전이 있다.
		//따라서 이를 구분하기 위해 공백을 추가한다.
		// 
		//20250710 get_hwnd_by_exe_file() 힘수를 이용해서 이 실행파일명의 윈도우 핸들을 얻어오고자 했으나
		//나중 실행된 ASee.exe는 배제시켰지만 또 다른 문제가 발생함.
		//minimize 버튼을 누른 경우와 작업표시줄에서 토글하여 minimize한 경우 다른 결과가 리턴된다.
		//먼저 실행된 ASee.exe의 핸들을 얻어야하는데 Gdi+ Window를 얻어오는 오류가 발생하여
		//caption으로 찾도록 수정함. 단, 이 역시도 나중 실행되는 ASee.exe를 얻어올 수 있으므로
		//visible인 ASee.exe의 핸들을 얻어오도록 수정함.
		//hWnd = get_hwnd_by_exe_file(get_exe_filename(false), GetCurrentProcessId());
		//hWnd = ::FindWindow(_T("CASeeApp"), _T("ASee - "));
		hWnd = FindWindowByCaption(_T("ASee - "))->m_hWnd;
		//msgbox(_T("hWnd = %p"), hWnd);
		//CString str;
		//str.Format(_T("%p"), hWnd);
		//AfxMessageBox(str);

		//IsIconic()을 빼면 Gdi+ Window라는 이상한 창이 뜨는 현상이 있고
		//SetForegroundWindow(hWnd);
		//SetActiveWindow(hWnd);

		if (true)//::IsIconic(hWnd))
		{
			//msgbox(_T("before SendMessage, Message_CASeeApp = %d"), Message_CASeeApp);
			//최소화 버튼을 눌러서 최소화 시킨 경우는 왜 restore가 안될까...
			//작업표시줄에서 토글로 최소화 시킨 경우는 잘 동작한다.
			//::ShowWindow(hWnd, SW_RESTORE);
			::SendMessage(hWnd, Message_CASeeApp, 0, 0);
			//msgbox(_T("after SendMessage, Message_CASeeApp = %d"), Message_CASeeApp);
		}
		else
		{
			msgbox(_T("not iconic"));
			::SendMessage(hWnd, Message_CASeeApp, 0, 0);
			msgbox(_T("after SendMessage, Message_CASeeApp = %d"), Message_CASeeApp);
		}

		m_hMutex = NULL;

		return false;
	}

	// Windows XP에서는 InitCommonControlsEx()를 필요로 합니다.
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다.
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 대화 상자에 셸 트리 뷰 또는
	// 셸 목록 뷰 컨트롤이 포함되어 있는 경우 셸 관리자를 만듭니다.
	CShellManager *pShellManager = new CShellManager;

	// MFC 컨트롤의 테마를 사용하기 위해 "Windows 원형" 비주얼 관리자 활성화
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));


	CASeeDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 여기에 [확인]을 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 여기에 [취소]를 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "경고: 대화 상자를 만들지 못했으므로 애플리케이션이 예기치 않게 종료됩니다.\n");
		TRACE(traceAppMsg, 0, "경고: 대화 상자에서 MFC 컨트롤을 사용하는 경우 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS를 수행할 수 없습니다.\n");
	}

	// 위에서 만든 셸 관리자를 삭제합니다.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고 응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
	return FALSE;
}

