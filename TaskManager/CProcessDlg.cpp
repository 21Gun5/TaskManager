// CProcessDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "CProcessDlg.h"
#include "afxdialogex.h"

#include "Func.h"


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
	DDX_Control(pDX, IDC_LIST_PROCESS, m_processList);
}


BEGIN_MESSAGE_MAP(CProcessDlg, CDialogEx)
END_MESSAGE_MAP()


// CProcessDlg 消息处理程序


BOOL CProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 设置 list 控件的扩展风格
	m_processList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 显示字段名（插入列
	m_processList.InsertColumn(0, L"名称", 0, 250);
	m_processList.InsertColumn(1, L"PID", 0, 100);
	m_processList.InsertColumn(2, L"CPU", 0, 100);
	m_processList.InsertColumn(3, L"内存", 0, 100);

	// 更新列表（插入行
	UpdateProcessList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CProcessDlg::UpdateProcessList()
{
	// TODO: 在此处添加实现代码.

	// 定义进程数组
	std::vector <PROCESSENTRY32> newProcessList;
	// 获取进程列表
	GetAllRunningProcess(&newProcessList);
	// 循环插入进程信息（设置内容
	int index = 0;
	for (auto &i : newProcessList) {
		CString  buffer;// 整型转字符串所用缓冲区
		m_processList.InsertItem(index, _T(""));// 插入行

		m_processList.SetItemText(index, 0, i.szExeFile);// 名称
		buffer.Format(_T("%d"), i.th32ProcessID);
		m_processList.SetItemText(index, 1, buffer);//PID

		index++;
	}
}
