#include <Windows.h>

// ��ȡ����
//void AllServicesCheck()
//{
//	PrintfLog("\nϵͳ������Ϣ: \n");
//	do {
//		SC_HANDLE SCMan = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
//		if (SCMan == NULL) {
//			break;
//		}
//		LPENUM_SERVICE_STATUS service_status;
//		DWORD cbBytesNeeded = NULL;
//		DWORD ServicesReturned = NULL;
//		DWORD ResumeHandle = NULL;
//
//		service_status = (LPENUM_SERVICE_STATUS)LocalAlloc(LPTR, MAX_SERVICE_SIZE);
//
//
//		BOOL ESS = EnumServicesStatus(SCMan,						// ���
//			SERVICE_WIN32,                                          // ��������
//			SERVICE_STATE_ALL,                                      // �����״̬
//			(LPENUM_SERVICE_STATUS)service_status,                  // ���������ϵͳ����Ľṹ
//			MAX_SERVICE_SIZE,                                       // �ṹ�Ĵ�С
//			&cbBytesNeeded,                                         // ������������շ�������ķ���
//			&ServicesReturned,                                      // ������������շ��ط��������
//			&ResumeHandle);                                         // ���������������һ�ε��ñ���Ϊ0������Ϊ0����ɹ�
//		if (ESS == NULL) {
//			break;
//		}
//		for (int i = 0; i < static_cast<int>(ServicesReturned); i++) {
//			fwprintf(g_pFile, L"ServiceName: %s\t", service_status[i].lpDisplayName);
//			// std::cout << "ServiceName: " << service_status[i].lpDisplayName << "\t";
//			switch (service_status[i].ServiceStatus.dwCurrentState) { // ����״̬
//			case SERVICE_CONTINUE_PENDING:
//				PrintfLog("CONTINUE_PENDING\n");
//				break;
//			case SERVICE_PAUSE_PENDING:
//				PrintfLog("PAUSE_PENDING\n");
//				break;
//			case SERVICE_PAUSED:
//				PrintfLog("PAUSED\n");
//				break;
//			case SERVICE_RUNNING:
//				PrintfLog("RUNNING\n");
//				break;
//			case SERVICE_START_PENDING:
//				PrintfLog("START_PENDING\n");
//				break;
//			case SERVICE_STOPPED:
//				PrintfLog("STOPPED\n");
//				break;
//			default:
//				PrintfLog("UNKNOWN\n");
//				break;
//			}
//			LPQUERY_SERVICE_CONFIG lpServiceConfig = NULL;          // ������ϸ��Ϣ�ṹ
//			SC_HANDLE service_curren = NULL;                        // ��ǰ�ķ�����
//			LPSERVICE_DESCRIPTION lpqscBuf2 = NULL;					// ����������Ϣ
//			service_curren = OpenService(SCMan, service_status[i].lpServiceName, SERVICE_QUERY_CONFIG);        // �򿪵�ǰ����
//			lpServiceConfig = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LPTR, MAX_QUERY_SIZE);                        // �����ڴ棬 ���Ϊ8kb 
//
//			if (NULL == QueryServiceConfig(service_curren, lpServiceConfig, MAX_QUERY_SIZE, &ResumeHandle)) {
//				break;
//			}
//			fwprintf(g_pFile, L"Path: %s\n", lpServiceConfig->lpBinaryPathName);
//			DWORD dwNeeded = 0;
//			if (QueryServiceConfig2(service_curren, SERVICE_CONFIG_DESCRIPTION, NULL, 0,
//				&dwNeeded) == FALSE && GetLastError() == ERROR_INSUFFICIENT_BUFFER)
//			{
//				lpqscBuf2 = (LPSERVICE_DESCRIPTION)LocalAlloc(LPTR, MAX_QUERY_SIZE);
//				if (QueryServiceConfig2(service_curren, SERVICE_CONFIG_DESCRIPTION,
//					(BYTE*)lpqscBuf2, dwNeeded, &dwNeeded))
//				{
//					fwprintf(g_pFile, L" Description: %s\n", lpqscBuf2->lpDescription);
//				}
//				LocalFree(lpqscBuf2);
//			}
//			CloseServiceHandle(service_curren);
//		}
//		CloseServiceHandle(SCMan);
//	} while (0);
//}