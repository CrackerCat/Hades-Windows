#pragma once
#include <Windows.h>

const ULONG64 MESSAGE_BUFFER_SIZE = 65536 + 1024;

class HlprMiniPortIpc
{
public:
	HlprMiniPortIpc();
	~HlprMiniPortIpc();

	// �ȴ����������˿� - ����
	void GetMsgNotifyWork();
	void MiniPortActiveCheck();
	void StartMiniPortWaitConnectWork();
	bool SetRuleProcess(PVOID64 rulebuffer, unsigned int buflen, unsigned int processnamelen);

private:
};


