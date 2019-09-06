// COther.cpp: 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "COther.h"
#include "afxdialogex.h"


// COther 对话框

IMPLEMENT_DYNAMIC(COther, CDialogEx)

COther::COther(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_OTHER, pParent)
{

}

COther::~COther()
{
}

void COther::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COther, CDialogEx)
END_MESSAGE_MAP()


// COther 消息处理程序
