#pragma once
#include <windows.h>
#include <vector>
#include <TlHelp32.h>


// 获取进程
void GetAllRunningProcess(std::vector<PROCESSENTRY32> * processList);

