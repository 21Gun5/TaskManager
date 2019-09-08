#pragma once


// CVSCleanDlg 对话框

class CVSCleanDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVSCleanDlg)

public:
	CVSCleanDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CVSCleanDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_VSCLEAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CListCtrl m_list;
	CString m_dir;
	CString m_filter;
	virtual BOOL OnInitDialog();
	DWORD64 m_totalSize;
	bool m_isScanBegin = false;
	HANDLE m_hScanThread;
protected:
//	afx_msg LRESULT OnMyMsgScanfile(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnMyMsgScanfile(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMyMsgScanfile(WPARAM wParam, LPARAM lParam);
};
