#pragma once
#include <vector>
#include <TlHelp32.h>

bool GetProcessAllModule(DWORD dwPid, std::vector<MODULEENTRY32>* moduleList);
