#pragma once
#include <windows.h>
#include <vector>
#include <TlHelp32.h>


// ��ȡ����
void GetAllRunningProcess(std::vector<PROCESSENTRY32> * processList);

