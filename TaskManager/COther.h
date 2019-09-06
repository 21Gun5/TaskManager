#pragma once


// COther 对话框

class COther : public CDialogEx
{
	DECLARE_DYNAMIC(COther)

public:
	COther(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~COther();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OTHER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
