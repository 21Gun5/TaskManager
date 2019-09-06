#pragma once


// COtherDlg 对话框

class COtherDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COtherDlg)

public:
	COtherDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~COtherDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
