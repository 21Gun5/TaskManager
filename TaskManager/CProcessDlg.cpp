// CProcessDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "CProcessDlg.h"
#include "afxdialogex.h"

#include "CModuleDlg.h"
#include "CTools.h"

// 临界区
CRITICAL_SECTION g_critical_section;


// CProcessDlg 对话框

IMPLEMENT_DYNAMIC(CProcessDlg, CDialogEx)

CProcessDlg::CProcessDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PROCESS, pParent)
{

}

CProcessDlg::~CProcessDlg()
{
}

void CProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CProcessDlg, CDialogEx)
	ON_WM_TIMER()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CProcessDlg::OnRclickList1)
	ON_COMMAND(ID_MENU_KILLPROC, &CProcessDlg::OnMenuKillproc)
	ON_COMMAND(ID_MENU_LISTMODULE, &CProcessDlg::OnMenuListmodule)
END_MESSAGE_MAP()


// CProcessDlg 消息处理程序
BOOL CProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 导入菜单资源
	m_menu.LoadMenu(IDR_MENU1);

	// 设置 list 控件的扩展风格
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 显示字段名（插入列
	m_list.InsertColumn(0, L"名称", 0, 250);
	m_list.InsertColumn(1, L"PID", 0, 150);
	m_list.InsertColumn(2, L"CPU", 0, 150);
	m_list.InsertColumn(3, L"内存", 0, 150);

	// 动态更新列表
	SetTimer(0, 1000, NULL);//计时器id、间隔时间、为NULL时本窗口将接收WM_TIMER消息
	// 更新列表（插入行
	//UpdateProcessList();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CProcessDlg::UpdateProcessList()
{
	// TODO: 在此处添加实现代码.

	// 获取进程列表
	std::vector <PROCESSINFO> newProcList;
	GetAllRunningProcess(&newProcList);
	// 若列表为空，则是第一次插入
	if (m_procList.size() == 0)
	{
		// 循环插入进程信息（设置内容
		int index = 0;
		for (auto &i : newProcList) 
		{
			CString  buffer;// 整型转字符串所用缓冲区
			m_list.InsertItem(index, _T(""));// 插入行
			
			m_list.SetItemText(index, 0, i.szExeFile);// 名称
			buffer.Format(_T("%d"), i.th32ProcessID);
			m_list.SetItemText(index, 1, buffer);//PID
			index++;
		}
		m_procList.swap(newProcList);
	}
	// 若不为空，则更新列表
	else
	{
		//MessageBox(NULL, L"123", MB_OK);
		// 删除已退出进程（旧列表元素到新列表中找,没找到即已退出
		int index = 0;
		for (auto it = m_procList.begin(); it != m_procList.end(); )
		{
			// 没找到即已退出, 将其从进程数组和控件中删除
			if (false == IsFindItemInList(newProcList, it->th32ProcessID)) {
				it = m_procList.erase(it);
				m_list.DeleteItem(index);
				continue;
			}
			index++;
			it++;		// 不该放到 for 中（因为有erase操作
		}
		// 插入新创建进程（新列表元素到旧列表中找, 没找到即新建的		
		for (auto&proc : newProcList)
		{
			if (false == IsFindItemInList(m_procList, proc.th32ProcessID))
			{
				// 插入到进程数组中
				m_procList.push_back(proc);
				// 插入到列表控件中
				CString buffer;
				m_list.InsertItem(index, _T(""));
				m_list.SetItemText(index, 0, proc.szExeFile);
				buffer.Format(_T("%d"), proc.th32ProcessID);
				m_list.SetItemText(index, 1, buffer);
			}
		}
	}

	//// 1. 获取最新的进程列表
	//std::vector<PROCESSINFO> newProcList;
	//GetAllRunningProcess(&newProcList);
	//// 2. 进入临界区（删增属于原子操作
	////EnterCriticalSection(&g_critical_section);
	//// 3. 删除已退出进程（旧列表元素到新列表中找,没找到即已退出
	//int index = 0;
	//for (auto it = m_procList.begin(); it != m_procList.end();)
	//{
	//	// 没找到即已退出, 将其从进程数组和控件中删除
	//	if (false == IsFindItemInList(newProcList, it->th32ProcessID))
	//	{
	//		it = m_procList.erase(it);
	//		m_list.DeleteItem(index);
	//		continue;
	//	}
	//	// 刷新内存占用率
	//	CString buffer;
	//	DWORD memUsage = GetProcessMemoryused(it->th32ProcessID);
	//	if (it->dwMemoryUsage != memUsage)
	//	{
	//		buffer.Format(_T("%12dKb"), memUsage);
	//		m_list.SetItemText(index, 3, buffer);
	//		it->dwMemoryUsage = memUsage;
	//	}
	//	// 刷新CPU使用率
	//	double cpuUsage = GetProcessCPUUsed(it->th32ProcessID);
	//	if (abs(it->lfCPUUsage - cpuUsage) >= 0.001)
	//	{
	//		buffer.Format(_T("%.1lf%%"), cpuUsage);
	//		m_list.SetItemText(index, 2, buffer);
	//		it->lfCPUUsage = cpuUsage;
	//	}
	//	// 下一个
	//	++index;
	//	it++;			// 不该放到 for 中（因为有erase操作
	//}
	//// 4. 插入数据到列表中，即插入新创建进程（新列表元素到旧列表中找, 没找到即新建的		
	//index = m_list.GetItemCount();
	//for (auto&i : newProcList) {
	//	if (false == IsFindItemInList(m_procList, i.th32ProcessID))
	//	{
	//		// 添加到列表数组
	//		m_procList.push_back(i);
	//		// 插入到列表控件
	//		CString buffer;
	//		m_list.InsertItem(index, _T(""));// 插入一行
	//		m_list.UpdateWindow();
	//		m_list.SetItemText(index, 0, i.szExeFile);// 名称
	//		buffer.Format(_T("%d"), i.th32ProcessID);
	//		m_list.SetItemText(index, 1, buffer);// PID
	//		m_list.SetItemText(index, 2, L"");// CPU？？
	//		buffer.Format(_T("%12dKb"), (double)i.dwMemoryUsage);
	//		m_list.SetItemText(index, 3, buffer);// 内存
	//		index++;
	//	}
	//}
	//// 5. 离开临界区
	////LeaveCriticalSection(&g_critical_section);
}


//void CProcessDlg::GetAllRunningProcess(std::vector<PROCESSENTRY32>* processList)
//{
//	// TODO: 在此处添加实现代码.
//
//		// 获取进程快照句柄
//	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
//	// 保存进程信息的变量
//	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
//	// 遍历进程，添入vector
//	Process32First(hSnap, &pe);
//	do {
//		processList->push_back(pe);
//	} while (Process32Next(hSnap, &pe));
//	// 关闭快照句柄
//	CloseHandle(hSnap);
//}


void CProcessDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnTimer(nIDEvent);
	// 更新列表（插入行并设置内容
	UpdateProcessList();
}


bool CProcessDlg::IsFindItemInList(std::vector<PROCESSINFO> list, DWORD pid)
{
	// TODO: 在此处添加实现代码.
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i].th32ProcessID == pid)
		{
			return true;
		}
	}
	return false;
}


void CProcessDlg::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CMenu * pSubMenu = m_menu.GetSubMenu(0);
	CPoint pos;
	GetCursorPos(&pos);
	pSubMenu->TrackPopupMenu(0, pos.x, pos.y, this);
}


void CProcessDlg::OnMenuKillproc()
{
	// TODO: 在此添加命令处理程序代码

	// 获取待结束进程（通过光标选择序号，序号从1开始，故-1
	int index = (int)m_list.GetFirstSelectedItemPosition() - 1;
	// 获取进程id（字符串转整形
	CString strPid = m_list.GetItemText(index, 1);// 第1列是pid
	DWORD dwPid = _wtoi(strPid);
	// 获取进程句柄（要通过句柄来结束进程
	HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, dwPid);
	// 结束进程
	TerminateProcess(hProc, 0);
	// 关闭句柄
	CloseHandle(hProc);
}


void CProcessDlg::OnMenuListmodule()
{
	// TODO: 在此添加命令处理程序代码

	// 创建模块对话框
	CModuleDlg moduleDlg(this);
	// 获取被点击的进程（通过光标选择序号，序号从1开始，故-1
	int index = (int)m_list.GetFirstSelectedItemPosition() - 1;
	// 传递进程PID/名字给模块对话框
	moduleDlg.SetProcessID(m_procList[index].th32ProcessID);
	CString buffer;
	buffer.Format(L"%s - 模块列表", m_procList[index].szExeFile);
	moduleDlg.SetProcessName(buffer);
	// 运行对话框
	moduleDlg.DoModal();
}
