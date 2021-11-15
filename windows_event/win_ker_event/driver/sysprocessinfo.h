/*
	����ö�ٷ�ʽ - �������ݶ�ͳһ�ɼ�
		1. �����ں�api(���ʺ�2һ��)
		2. ���ڽ�������
		3. ���ھ����
		4. �����ڴ�ö��
*/
#ifndef _SYSPROCESSINFO_H
#define _SYSPROCESSINFP_H

typedef struct _HANDLE_INFO {
	ULONG_PTR	ObjectTypeIndex;
	ULONG_PTR	HandleValue;
	ULONG_PTR	ReferenceCount;
	ULONG_PTR	GrantedAccess;
	ULONG_PTR	CountNum;
	ULONG_PTR	Object;
	WCHAR	ProcessName[256 * 2];
	WCHAR	TypeName[256 * 2];
	WCHAR	HandleName[256 * 2];
} HANDLE_INFO, * PHANDLE_INFO;

typedef struct _KERNEL_COPY_MEMORY_OPERATION
{
	INT32 targetProcessId;
	PVOID targetAddress;
	PVOID bufferAddress;
	INT32 bufferSize;
} KERNEL_COPY_MEMORY_OPERATION, * PKERNEL_COPY_MEMORY_OPERATION;

ULONG_PTR nf_EnumProcessHandle(HANDLE pid);
VOID nf_EnumModuleByPid(ULONG pid);
int nf_DumpProcess(PKERNEL_COPY_MEMORY_OPERATION request);
int nf_KillProcess(ULONG pid);

#endif