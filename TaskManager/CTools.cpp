#include "stdafx.h"
#include "CTools.h"
#include <Psapi.h>

bool GetProcessAllModule(DWORD dwPid, std::vector<MODULEENTRY32>* moduleList)
{
	// 获取快照句柄（遍历模块时需指定pid
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
	// 存储模块信息
	MODULEENTRY32 mInfo = { sizeof(MODULEENTRY32) };
	// 遍历模块
	Module32First(hSnap, &mInfo);
	do
	{
		moduleList->push_back(mInfo);
	} while (Module32Next(hSnap, &mInfo));
	return true;
}