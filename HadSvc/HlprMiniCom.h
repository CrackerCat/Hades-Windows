#pragma once
#include <Windows.h>

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


