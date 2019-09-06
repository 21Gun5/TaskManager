#include "stdafx.h"
#include "Func.h"

void GetAllRunningProcess(std::vector<PROCESSENTRY32>* processList)
{
	// ��ȡ���̿��վ��
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	// ���������Ϣ�ı���
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
	// �������̣�����vector
	Process32First(hSnap, &pe);
	do {
		processList->push_back(pe);
	} while (Process32Next(hSnap,&pe));
	// �رտ��վ��
	CloseHandle(hSnap);
}
