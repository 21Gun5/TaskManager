#pragma once

#include <vector>
#include <TlHelp32.h>

// CProcessDlg 对话框

class CProcessDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessDlg)

public:
	CProcessDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CProcessDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PROCESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	virtual BOOL OnInitDialog();
	void UpdateProcessList();
	void GetAllRunningProcess(std::vector<PROCESSENTRY32>* processList);
};
