// CProcessDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "CProcessDlg.h"
#include "afxdialogex.h"


// CProcessDlg 对话框

IMPLEMENT_DYNAMIC(CProcessDlg, CDialogEx)

CProcessDlg::CProcessDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CProcessDlg::~CProcessDlg()
{
}

void CProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CProcessDlg, CDialogEx)
END_MESSAGE_MAP()


// CProcessDlg 消息处理程序
