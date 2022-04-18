#include "MainWindow.h"
#include "../Systeminfolib.h"
#include <usysinfo.h>
#include <TlHelp32.h>
#include <mutex>
#include <WinUser.h>

std::mutex g_hadesStatuscs;
// ��̬��ʱ����Ҫ
USysBaseInfo g_DynSysBaseinfo;

std::wstring GetWStringByChar(const char* szString)
{
	std::wstring wstrString;
	if (szString != NULL)
	{
		std::string str(szString);
		wstrString.assign(str.begin(), str.end());
	}
	return wstrString;
}
LPCTSTR MainWindow::GetWindowClassName() const
{
	return _T("HadesMainWindow");
}
CDuiString MainWindow::GetSkinFile()
{
	return _T("MainWindow.xml");
}
CDuiString MainWindow::GetSkinFolder()
{
	return _T("");
}

// HadesSvc���̣���ֹ������Svc�ҵ�������û�и�֪
void MainWindow::GetHadesSvctStatus()
{
	//���HadesSvc�Ƿ����
	for (;;)
	{
		auto active_event = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Global\\HadesSvc_EVENT");
		if (0 >= (int)active_event)
		{
			if (true == m_hadesSvcStatus)
			{
				// HadesSvc���ߣ�δ����
				// ���½���״̬
				m_pImage_lab = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("ServerConnectImg")));
				m_pImage_lab->SetBkImage(L"img/normal/winmain_connectfailuer.png");
				m_pConnectSvc_lab->SetText(L"δ����ƽ̨");
				g_hadesStatuscs.lock();
				m_hadesSvcStatus = false;
				g_hadesStatuscs.unlock();
			}	
		}
		CloseHandle(active_event);
		Sleep(1000);
	}
}
static DWORD WINAPI HadesSvcActiveEventNotify(LPVOID lpThreadParameter)
{
	(reinterpret_cast<MainWindow*>(lpThreadParameter))->GetHadesSvctStatus();
	return 0;
}

// HadesSvc�Ƿ�����GRPC�ɹ�����
void MainWindow::GetHadesSvcConnectStatus()
{
	m_pImage_lab = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("ServerConnectImg")));
	m_pConnectSvc_lab = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("ServerConnectStatus")));
	for (;;)
	{
		WaitForSingleObject(m_HadesConnectStatus, INFINITE);
		// ���½���״̬
		m_pImage_lab->SetBkImage(L"img/normal/winmain_connectsuccess.png");
		m_pConnectSvc_lab->SetText(L"������ƽ̨");
		g_hadesStatuscs.lock();
		m_hadesSvcStatus = true;
		g_hadesStatuscs.unlock();
	}
}
static DWORD WINAPI HadesConnectEventNotify(LPVOID lpThreadParameter)
{
	//�ȴ�Grpc���� - ���򲻼���
	(reinterpret_cast<MainWindow*>(lpThreadParameter))->GetHadesSvcConnectStatus();
	return 0;
}

// HadesSvc�ػ�����
void MainWindow::HadesSvcDaemon()
{
	while (true)
	{
		if (false == m_hadesSvcStatus)
		{
			//CreateProcessAsUser();
		}
		Sleep(2000);
	}
}

void WindlgShow(HWND& hWnd)
{
	typedef void    (WINAPI* PROCSWITCHTOTHISWINDOW)    (HWND, BOOL);
	PROCSWITCHTOTHISWINDOW    SwitchToThisWindow;
	HMODULE    hUser32 = GetModuleHandle(L"user32");
	SwitchToThisWindow = (PROCSWITCHTOTHISWINDOW)GetProcAddress(hUser32, "SwitchToThisWindow");
	SwitchToThisWindow(hWnd, TRUE);
}
void killProcess(const wchar_t* const processname)
{

	HANDLE hSnapshort = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshort == INVALID_HANDLE_VALUE)
	{
		return;
	}

	// ����߳��б�  
	PROCESSENTRY32 stcProcessInfo;
	stcProcessInfo.dwSize = sizeof(stcProcessInfo);
	BOOL  bRet = Process32First(hSnapshort, &stcProcessInfo);
	while (bRet)
	{
		if (lstrcatW(stcProcessInfo.szExeFile, processname) == 0)
		{
			HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, stcProcessInfo.th32ProcessID);
			::TerminateProcess(hProcess, 0);
			CloseHandle(hProcess);
			break;
		}
		bRet = Process32Next(hSnapshort, &stcProcessInfo);
	}

	CloseHandle(hSnapshort);
}
//LRESULT MainWindow::OnTrayIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//	//�����ͼ���е��������ԭ
//	if (lParam == WM_LBUTTONDOWN)
//	{
//		//ɾ������ͼ��
//		Shell_NotifyIcon(NIM_DELETE, &m_trayIcon);
//		//��ʾ������
//		ShowWindow(SW_SHOWNORMAL);
//	}
//	//�����ͼ���е����Ҽ��򵯳�����ʽ�˵�
//	if (lParam == WM_RBUTTONDOWN)
//	{
//		//��ȡ�������
//		POINT pt; GetCursorPos(&pt);
//		//�һ�����ؿ���������һ������Ĳ˵���
//		SetForegroundWindow(m_hWnd);
//		//���̲˵�    win32����ʹ�õ���HMENU�������MFC�������ʹ��CMenu
//		HMENU hMenu;
//		//�������̲˵�
//		hMenu = CreatePopupMenu();
//		//��Ӳ˵�,�ؼ��������õ�һ����ʶ��  WM_ONCLOSE �������õ�
//		AppendMenu(hMenu, MF_STRING, WM_ONCLOSE, _T("Exit"));
//		//�����˵�,�����û���ѡ�˵���ı�ʶ������
//		int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD, pt.x, pt.y, NULL, m_hWnd, NULL);
//		//�����ʶ����WM_ONCLOSE��ر�
//		if (cmd == WM_ONCLOSE)
//		{
//			m_trayIcon.hIcon = NULL;
//			Shell_NotifyIcon(NIM_DELETE, &m_trayIcon);
//			//�˳�����
//			::PostQuitMessage(0);
//		}
//	}
//	bHandled = true;
//	return 0;
//}
//void MainWindow::AddTrayIcon() {
//	memset(&m_trayIcon, 0, sizeof(NOTIFYICONDATA));
//	m_trayIcon.cbSize = sizeof(NOTIFYICONDATA);
//	m_trayIcon.hIcon = ::LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_SMALL));
//	m_trayIcon.hWnd = m_hWnd;
//	lstrcpy(m_trayIcon.szTip, _T("Msg"));
//	m_trayIcon.uCallbackMessage = WM_SHOWTASK;
//	m_trayIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
//	Shell_NotifyIcon(NIM_ADD, &m_trayIcon);
//	ShowWindow(SW_HIDE);
//}
LRESULT MainWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = __super::OnCreate(uMsg, wParam, lParam, bHandled);
	
	// Create Meue
	m_pMenu = new Menu();
	m_pMenu->Create(m_hWnd, _T(""), WS_POPUP, WS_EX_TOOLWINDOW);
	m_pMenu->ShowWindow(false);

	//��ʼ������
	Systeminfolib libobj;
	CLabelUI* pCurrentUser_lab = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("mainwin_currentuser_lab")));
	pCurrentUser_lab->SetText(GetWStringByChar(SYSTEMPUBLIC::sysattriinfo.currentUser.c_str()).c_str());
	CLabelUI* pCpu_lab = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("mainwin_cpu_lab")));
	pCpu_lab->SetText(GetWStringByChar(SYSTEMPUBLIC::sysattriinfo.cpuinfo.c_str()).c_str());
	CLabelUI* pSysver_lab = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("mainwin_sysver_lab")));
	pSysver_lab->SetText(GetWStringByChar(SYSTEMPUBLIC::sysattriinfo.verkerlinfo.c_str()).c_str());
	CLabelUI* pMainbocard_lab = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("mainwin_mainbocard_lab")));
	pMainbocard_lab->SetText(GetWStringByChar(SYSTEMPUBLIC::sysattriinfo.mainboard[0].c_str()).c_str());
	CLabelUI* pBattery_lab = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("mainwin_battery_lab")));
	pBattery_lab->SetText(GetWStringByChar(SYSTEMPUBLIC::sysattriinfo.battery[0].c_str()).c_str());

	pMainOptemp = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("MainOptemperature_VLayout")));
	pMainOpcpu = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("MainOpCpu_VLayout")));
	pMainOpbox = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("MainOpBox_VLayout")));
	pMainOptemp->SetVisible(false);
	pMainOpbox->SetVisible(false);
	pMainOpcpu->SetVisible(true);

	// ��������֮ǰHadesSvc����������Ҫǿ���˳�Svc
	do {
		auto active_event = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Global\\HadesSvc_EVENT");
		if (active_event)
		{
			CloseHandle(active_event);
			auto exithandSvc = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Global\\HadesSvc_EVNET_EXIT");
			if (exithandSvc)
			{
				SetEvent(exithandSvc);
				CloseHandle(exithandSvc);
				Sleep(100);
			}
			const wchar_t killname[] = L"HadesSvc.exe";
			killProcess(killname);
			active_event = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Global\\HadesSvc_EVENT");
			if (active_event)
			{
				OutputDebugString(L"HadesSvc�Ѿ����������ֶ�����������������");
				CloseHandle(active_event);
				return lRes;
			}
		}
	} while (false);
	

	// ����������ִ��HadesSvc
	m_HadesConnectStatus = CreateEvent(NULL, FALSE, FALSE, L"Global\\HadesSvcConnect_Event");
	// ����һ���̵߳ȴ�HadesSvc���� 
	CreateThread(NULL, NULL, HadesConnectEventNotify, this, 0, 0);
	// ���HadesSvc�Ƿ�һֱ��Ծ
	CreateThread(NULL, NULL, HadesSvcActiveEventNotify, this, 0, 0);

	// Start HadesSvc.exe
	//CreateProcessAsUser();
	
	//���ö�ʱ��
	SetTimer(m_hWnd, 1, 1000, NULL);
	return lRes;
}
LRESULT MainWindow::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	KillTimer(m_hWnd, 1);
	auto exithandSvc = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Global\\HadesSvc_EVNET_EXIT");
	if (exithandSvc)
	{
		SetEvent(exithandSvc);
		CloseHandle(exithandSvc);
	}
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

void MainWindow::FlushData()
{
	//cpu
	const double cpuutilize = g_DynSysBaseinfo.GetSysDynCpuUtiliza();
	CString m_Cpusyl;
	m_Cpusyl.Format(L"CPU: %0.2lf", cpuutilize);
	m_Cpusyl += "%";
	CLabelUI* pCpuut = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("winmain_layout_cpuinfo")));
	pCpuut->SetText(m_Cpusyl.GetBuffer());

	//memory
	const DWORD dwMem = g_DynSysBaseinfo.GetSysDynSysMem();
	// ��ǰռ���� Occupancy rate
	CString m_MemoryBFB;
	m_MemoryBFB.Format(L"�ڴ�: %u", dwMem);
	m_MemoryBFB += "%";
	CLabelUI* pMem = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("winmain_layout_memory")));
	pMem->SetText(m_MemoryBFB.GetBuffer());
}
static DWORD WINAPI ThreadFlush(LPVOID lpThreadParameter)
{
	(reinterpret_cast<MainWindow*>(lpThreadParameter))->FlushData();
	return 0;
}
LRESULT MainWindow::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	QueueUserWorkItem(ThreadFlush, this, WT_EXECUTEDEFAULT);
	bHandled = false;
	return 0;
}

void MainWindow::Notify(TNotifyUI& msg)
{
	CDuiString strClassName = msg.pSender->GetClass();
	CDuiString strControlName = msg.pSender->GetName();

	if (msg.sType == DUI_MSGTYPE_WINDOWINIT);
	else if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (strClassName == DUI_CTR_BUTTON)
		{
			if (strControlName == _T("MainCloseBtn"))
			{
				//AddTrayIcon();
				Close();
			}
			else if (strControlName == _T("MainMenuBtn"))
			{//�˵�
				int xPos = msg.pSender->GetPos().left - 36;
				int yPos = msg.pSender->GetPos().bottom;
				POINT pt = { xPos, yPos };
				ClientToScreen(m_hWnd, &pt);
				m_pMenu->ShowWindow(true);
				::SetWindowPos(m_pMenu->GetHWND(), NULL, pt.x, pt.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
			}
			else if (strControlName == _T("MainMonCveBtn"))
			{//��������

			}
			//��С��
			else if (strControlName == _T("MainMinsizeBtn"))
			{
				::ShowWindow(m_hWnd, SW_MINIMIZE);
			}
		}
		else if (strClassName == DUI_CTR_OPTION)
		{
			if (_tcscmp(static_cast<COptionUI*>(msg.pSender)->GetGroup(), _T("MainOpView")) == 0)
			{

				if (strControlName == _T("MainMontemperatureOpt"))
				{
					// MainOptemperature_VLayout
					pMainOptemp->SetVisible(true);
					pMainOpcpu->SetVisible(false);
					pMainOpbox->SetVisible(false);
				}
				else if (strControlName == _T("MainMonCpuOpt"))
				{
					// MainOpCpu_VLayout
					pMainOptemp->SetVisible(false);
					pMainOpcpu->SetVisible(true);
					pMainOpbox->SetVisible(false);
				}
				else if (strControlName == _T("MainMonBoxOpt"))
				{
					// MainOpBox_VLayout
					pMainOptemp->SetVisible(false);
					pMainOpcpu->SetVisible(false);
					pMainOpbox->SetVisible(true);
				}
			}
			else if (strControlName == _T("MainMonUserBtn"))
			{//�·��û�̬���ָ��
				HWND m_SvcHwnd = FindWindow(L"HadesSvc", L"HadesSvc");
				COPYDATASTRUCT c2_;
				c2_.dwData = 1;
				c2_.cbData = 0;
				c2_.lpData = NULL;
				//������Ϣ
				::SendMessage(m_SvcHwnd, WM_COPYDATA, NULL, (LPARAM)&c2_);
			}
			else if (strControlName == _T("MainMonKerBtn"))
			{//�·��ں�̬���ָ��
				HWND m_SvcHwnd = FindWindow(L"HadesSvc", L"HadesSvc");
				COPYDATASTRUCT c2_;
				c2_.dwData = 2;
				c2_.cbData = 0;
				c2_.lpData = NULL;
				::SendMessage(m_SvcHwnd, WM_COPYDATA, NULL, (LPARAM)&c2_);
			}
		}
	}
}
LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	return __super::HandleMessage(uMsg, wParam, lParam);
}
LRESULT MainWindow::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;
	bHandled = TRUE;

	switch (uMsg) {
	case WM_TIMER: lRes = OnTimer(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
		break;
	}
	if (bHandled) return lRes;
	return __super::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
}