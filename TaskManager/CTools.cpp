#include "stdafx.h"
#include "CTools.h"
#include <Psapi.h>

bool GetProcessAllModule(DWORD dwPid, std::vector<MODULEENTRY32>* moduleList)
{
	// ��ȡ���վ��������ģ��ʱ��ָ��pid
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
	// �洢ģ����Ϣ
	MODULEENTRY32 mInfo = { sizeof(MODULEENTRY32) };
	// ����ģ��
	Module32First(hSnap, &mInfo);
	do
	{
		moduleList->push_back(mInfo);
	} while (Module32Next(hSnap, &mInfo));
	return true;
}