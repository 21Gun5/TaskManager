#include "stdafx.h"
#include "Func.h"

void GetAllRunningProcess(std::vector<PROCESSENTRY32>* processList)
{
	// 获取进程快照句柄
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	// 保存进程信息的变量
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
	// 遍历进程，添入vector
	Process32First(hSnap, &pe);
	do {
		processList->push_back(pe);
	} while (Process32Next(hSnap,&pe));
	// 关闭快照句柄
	CloseHandle(hSnap);
}
