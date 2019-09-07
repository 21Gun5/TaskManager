#include "stdafx.h"
#include "CTools.h"
#include <Winternl.h>
#include <Psapi.h>


// 获取所有运行进程
void GetAllRunningProcess(std::vector<PROCESSINFO>* processList)
{
	// TODO: 在此处添加实现代码.

		// 获取进程快照句柄
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	// 保存进程信息的变量
	PROCESSENTRY32 proc32 = { sizeof(PROCESSENTRY32) };
	PROCESSINFO pe = *(PROCESSINFO*)&proc32;
	// 遍历进程，添入vector
	Process32First(hSnap, &pe);
	do {
		processList->push_back(pe);
	} while (Process32Next(hSnap, &pe));
	// 关闭快照句柄
	CloseHandle(hSnap);
}
// 获取进程所有模块
void GetProcessAllModule(DWORD dwPid, std::vector<MODULEENTRY32>* moduleList)
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
}
// 获取进程内存占用率
double GetProcessMemoryused(DWORD pid)
{
	//	1. 相关变量设置
	double used = 0;
	PROCESS_MEMORY_COUNTERS pmc = { sizeof(PROCESS_MEMORY_COUNTERS) };
	// 2. 打开进程获取句柄
	HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
	// 3. 获取内存信息
	GetProcessMemoryInfo(hProc, &pmc, sizeof(pmc));
	used = pmc.WorkingSetSize;
	// 4. 关闭句柄并返回利用率
	CloseHandle(hProc);
	return used / 1024;
}
// 获取进程CPU占用率
double GetProcessCPUUsed(DWORD pid)
{
	// 0 . 公式：内存占用率 =  (进程内核时间 + 进程用户时间) / (CPU内核时间 + CPU用户时间)
	// 1. 获取进程时间（内核/用户
	LARGE_INTEGER kernelTime = { 0 };
	LARGE_INTEGER userTime = { 0 };
	getProcessTime(pid, &kernelTime, &userTime);
	// 2. 获取CPU时间（空闲/内核/用户）
	FILETIME CPUIdleTime = {}, CPUKernelTime = {}, CPUUserTime = {};
	GetSystemTimes((FILETIME*)&CPUIdleTime, (FILETIME*)&CPUKernelTime, (FILETIME*)&CPUUserTime);
	// 3. 计算占用率并返回
	double proc = FILETIMEtoDouble(*(FILETIME*)&kernelTime) + FILETIMEtoDouble(*(FILETIME*)&userTime);
	double used = FILETIMEtoDouble(CPUKernelTime) + FILETIMEtoDouble(CPUKernelTime);
	return proc / used * 100.0;
}
// 获取进程相关时间
void getProcessTime(DWORD dwPid, LARGE_INTEGER* kernelTime, LARGE_INTEGER* userTime)
{
	// 1. 时间结构体
	KERNEL_USERTEXTIMES time;
	// 2. 打开进程获取句柄
	HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPid);
	// 3. 获取时间
	decltype(NtQueryInformationProcess)* fnNtQueryInformationProcess;
	fnNtQueryInformationProcess = (decltype(NtQueryInformationProcess)*)
		(GetProcAddress(GetModuleHandle(_T("ntdll.dll")), "NtQueryInformationProcess"));
	DWORD read = 0;
	fnNtQueryInformationProcess(hProc,(PROCESSINFOCLASS)ProcessTimes,&time,sizeof(time),&read);
	// 4. 返回获得的时间（改实参
	*kernelTime = time.KernelTime;
	*userTime = time.UserTime;
	// 5. 关闭句柄
	CloseHandle(hProc);
}
// 转换时间格式
double	FILETIMEtoDouble(const _FILETIME&	fileTime)
{
	return	double(fileTime.dwHighDateTime*4.294967296e9) + double(fileTime.dwLowDateTime);
}


// 遍历窗口时的回调函数
BOOL CALLBACK wndProc(HWND hWnd, LPARAM l)
{
	std::list<HWND>* list = (std::list<HWND>*)l;
	list->push_back(hWnd);
	return 1;
}
// 获取某窗口
void getWindows(std::list<HWND>* wndList)
{
	EnumWindows(wndProc, (LPARAM)wndList);
}