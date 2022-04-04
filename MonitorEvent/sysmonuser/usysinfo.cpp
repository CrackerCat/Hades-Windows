#include <Windows.h>
#include "usysinfo.h"
#include <sysinfo.h>
#include <atlstr.h>



USysBaseInfo::USysBaseInfo()
{
}

USysBaseInfo::~USysBaseInfo()
{
}

// Mem�Ż�
void MemSwap()
{
	CString str, str1;
	str = "һ�����ٳɹ��� ��ʡ�˿ռ䣺  ";
	// 1. ��ȡ��ǰ���������ڴ�״̬
	MEMORYSTATUSEX stcMemStatusEx = { 0 };
	stcMemStatusEx.dwLength = sizeof(stcMemStatusEx);
	GlobalMemoryStatusEx(&stcMemStatusEx);
	DWORDLONG preUsedMem = stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;
	// 2. �����ڴ�
	DWORD dwPIDList[1000] = { 0 };
	DWORD bufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	// EnumProcesses(dwPIDList, bufSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize / sizeof(DWORD); ++i)
	{
		HANDLE hProccess = OpenProcess(PROCESS_SET_QUOTA, false, dwPIDList[i]);
		SetProcessWorkingSetSize(hProccess, -1, -1);
	}
	// 3. ��ȡ�������ڴ�״̬
	GlobalMemoryStatusEx(&stcMemStatusEx);
	DWORDLONG afterCleanUserdMem = stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;
	// 4. ���㲢��������ɹ�
	DWORDLONG CleanofSuccess = preUsedMem - afterCleanUserdMem;
	str1.Format(L"%d", (CleanofSuccess / 1024 / 1024 / 8));
	str = str + str1 + " MB";
}
// ʱ��ת��
double FILETIMEDouble(const _FILETIME& filetime)
{
	return double(filetime.dwHighDateTime * 4.294967296e9) + double(filetime.dwLowDateTime);
}
// CPU
void GetCpuUsage(LPVOID outbuf)
{
	// ��ȡ����ʱ�� �ں� �û�
	_FILETIME idleTime, kernelTime, userTime;
	GetSystemTimes(&idleTime, &kernelTime, &userTime);
	// Creates or opens a named or unnamed event object.
	// �������һ�������Ļ��������¼�����
	// failure 0  | sucess handle
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	// �ȴ�1000���룬�ں˶�������ȷ
	WaitForSingleObject(hEvent, 1000);
	// ��ȡ�µ�ʱ��
	_FILETIME newidleTime, newkernelTime, newuserTime;
	GetSystemTimes(&newidleTime, &newkernelTime, &newuserTime);
	// ת��ʱ��
	double	doldidleTime = FILETIMEDouble(idleTime);
	double	doldkernelTime = FILETIMEDouble(kernelTime);
	double	dolduserTime = FILETIMEDouble(userTime);
	double	dnewidleTime = FILETIMEDouble(newidleTime);
	double	dnewkernelTime = FILETIMEDouble(newkernelTime);
	double	dnewuserTime = FILETIMEDouble(newuserTime);
	double	Times = dnewidleTime - doldidleTime;
	double	Kerneltime = dnewkernelTime - doldkernelTime;
	double	usertime = dnewuserTime - dolduserTime;
	// ����ʹ����
	double Cpurate = (100.0 - Times / (Kerneltime + usertime) * 100.0);
	//m_Cpusyl.Format(L"%0.2lf", Cpurate);
	//m_Cpusyl += "%";
}
// Virtual Memory
void GetMemoryInfo(LPVOID outbuf)
{
	// �����ṹ����� ��ȡ�ڴ���Ϣ����
	MEMORYSTATUS memStatus;
	GlobalMemoryStatus(&memStatus);
	//// ��ǰռ���� Occupancy rate
	//m_MemoryBFB.Format(L"%u", memStatus.dwMemoryLoad);
	//m_MemoryBFB += "%";
	//// ��ʹ�������ڴ��С Physical memory size
	//size_t memPhysize = memStatus.dwTotalPhys - memStatus.dwAvailPhys;
	//m_Pymemory.Format(L"%u", (memPhysize / 1024 / 1024 / 8));
	//m_Pymemory += " MB";
	//// �ļ�������С Size of the file exchange
	//m_Pagesize.Format(L"%u", (memStatus.dwAvailPageFile / 1024 / 1024 / 8));
	//m_Pagesize += " MB";
	//// �����ڴ��С Virtual memory size
	//m_Memorysize.Format(L"%u", (memStatus.dwTotalVirtual / 1024 / 1024 / 8));
	//m_Memorysize += " MB";
	//// ���������ڴ��С Available virtual memory size
	//m_Kymemorysize.Format(L"%d", (memStatus.dwAvailVirtual / 1024 / 1024 / 8));
	//m_Kymemorysize += " MB";
}

bool USysBaseInfo::uf_GetSystemBaseInfo(LPVOID outbuf)
{
	// Ӳ����Ϣ


	// �����Ϣ


	// ϵͳ������Ϣ
	//GetCpuUsage();
	//GetMemoryInfo();

	return true;
}