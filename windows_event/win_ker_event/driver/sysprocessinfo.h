/*
	����ö�ٷ�ʽ - �������ݶ�ͳһ�ɼ�
		1. �����ں�api(���ʺ�2һ��)
		2. ���ڽ�������
		3. ���ھ����
		4. �����ڴ�ö��
*/
#ifndef _SYSPROCESSINFO_H
#define _SYSPROCESSINFP_H


int nf_KillProcess(PEPROCESS Process);
int nf_DumpProcess();

int nf_GetSysProcess_SearchMemory();
int nf_GetSysProcess_Api();
int nf_GetSysProcess_List();
int nf_GetSysProcess_CidHandle();
int nf_GetSysProcess_Module();

#endif