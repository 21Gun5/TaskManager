// CVSCleanDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "CVSCleanDlg.h"
#include "afxdialogex.h"
#include "CTools.h"


// CVSCleanDlg 对话框

#define MY_MSG_SCANFILE (WM_USER+100)

IMPLEMENT_DYNAMIC(CVSCleanDlg, CDialogEx)

CVSCleanDlg::CVSCleanDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_VSCLEAN, pParent)
	, m_dir(_T(""))
	, m_filter(_T(""))
{

}

CVSCleanDlg::~CVSCleanDlg()
{
}

void CVSCleanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT2, m_dir);
	DDX_Text(pDX, IDC_EDIT1, m_filter);
}


BEGIN_MESSAGE_MAP(CVSCleanDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CVSCleanDlg::OnBnClickedButton1)
//	ON_MESSAGE(MY_MSG_SCANFILE, &CVSCleanDlg::OnMyMsgScanfile)
//ON_REGISTERED_MESSAGE(MY_MSG_SCANFILE, &CVSCleanDlg::OnMyMsgScanfile)

ON_MESSAGE(MY_MSG_SCANFILE, &CVSCleanDlg::OnMyMsgScanfile)
END_MESSAGE_MAP()


// CVSClean 消息处理程序


DWORD CALLBACK ScanFileProc(VOID* arg)
{
	// 在创建线程时:
	//  CreateThread(0, 0, ScanFileProc, this, 0, 0);
	//  已将this指针通过附加参数传递过来了, 因此,可以将
	//  参数强转成this指针来使用.
	CVSCleanDlg* _this = (CVSCleanDlg*)arg;
	findFiles(_this->m_dir,
		_this->m_filter,
		NULL,
		_this->m_hWnd,/*接收消息的窗口*/
		MY_MSG_SCANFILE /*接收的消息,这个宏是自定义的,
						该消息会在CVsCleanDlg类中有一
						个消息响应函数:OnScanFile,这
						个函数在消息映射中和
						MY_MSG_SCANFILE绑定*/);
	_this->MessageBox(L"扫描完毕");
	_this->m_isScanBegin = false;
	return 0;
}

void CVSCleanDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_isScanBegin)
	{
		SetDlgItemText(IDC_BUTTON1, _T("开始扫描"));
		TerminateThread(m_hScanThread, 0);
		m_isScanBegin = false;
		return;
	}

	// 获取控件的数据
	UpdateData(TRUE);

	// 如果目录为空,则弹出文件夹选择对话框
	if (m_dir.IsEmpty()) {

		TCHAR			szFolderPath[MAX_PATH] = { 0 };
		BROWSEINFO		sInfo;
		::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
		sInfo.pidlRoot = 0;
		sInfo.lpszTitle = _T("请选择一个文件夹：");
		sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_USENEWUI | BIF_BROWSEINCLUDEFILES;
		sInfo.lpfn = NULL;

		// 显示文件夹选择对话框
		LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
		if (lpidlBrowse != NULL)
		{
			// 取得文件夹名
			if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
			{
				m_dir = szFolderPath;
				UpdateData(FALSE);
			}
		}
		if (lpidlBrowse != NULL)
		{
			::CoTaskMemFree(lpidlBrowse);
		}
		else
		{
			return;
		}
		SetDlgItemText(IDC_BUTTON1, _T("开始扫描"));
		return;
	}

	m_isScanBegin = true;
	SetDlgItemText(IDC_BUTTON1, _T("停止扫描"));


	// 删除所有节点
	m_list.DeleteAllItems();
	m_totalSize = 0;

	// 遍历文件, 添加到列表控件
	// 开启线程扫描垃圾文件
	m_hScanThread = CreateThread(0, 0, ScanFileProc, this, 0, 0);
}


BOOL CVSCleanDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_list.InsertColumn(0, L"路径", 0, 250);
	m_list.InsertColumn(1, L"大小", 0, 100);

	m_filter = L".ilk .pdb .obj .log .pch .tlog .lastbuildstate .sdf .idb .ipch .res .o .lst .knl .img .bin .db";
	UpdateData(FALSE);

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


afx_msg LRESULT CVSCleanDlg::OnMyMsgScanfile(WPARAM wParam, LPARAM lParam)
{
	if (!m_isScanBegin)return 1;
	FILEINFO* fInfo = (FILEINFO*)wParam;

	m_totalSize += (fInfo->nFileSizeHigh << 32 | fInfo->nFileSizeLow);

	int row = m_list.GetItemCount();
	m_list.InsertItem(row, fInfo->path);
	CString buffer;
	buffer.Format(_T("%dKb"), (fInfo->nFileSizeHigh << 32 | fInfo->nFileSizeLow) / 1024);
	m_list.SetItemText(row, 1, buffer);
	//滚动到最后一行
	m_list.Scroll(CSize(0, 100000));
	return 1;
}
