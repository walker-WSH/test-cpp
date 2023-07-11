
// MFCDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFC.h"
#include "MFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCDlg 对话框



CMFCDlg::CMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMFCDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMFCDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_KILL_CHILD, &CMFCDlg::OnBnClickedButtonKillChild)
	ON_BN_CLICKED(IDC_BUTTON_CRASH, &CMFCDlg::OnBnClickedButtonCrash)
	ON_BN_CLICKED(IDC_BUTTON_MSGBOX, &CMFCDlg::OnBnClickedButtonMsgbox)
	ON_BN_CLICKED(IDC_BUTTON_WHILE, &CMFCDlg::OnBnClickedButtonWhile)
	END_MESSAGE_MAP()


// CMFCDlg 消息处理程序

BOOL CMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	int argc = 0;
	LPWSTR *argv = CommandLineToArgvW(GetCommandLineW(), &argc);
	if (argv) {
		LocalFree(argv);
	}

	if (argc > 1) {
		SetWindowText(_T("this is a child process"));
	} else {
		SetWindowText(_T("main process"));
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <Windows.h>
#include <vector>

std::vector<HANDLE> jobs;

void CMFCDlg::OnBnClickedOk()
{
	// run child process
	wchar_t szPath[MAX_PATH] = {0};
	::GetModuleFileName(nullptr, szPath, MAX_PATH);

	wchar_t cmd[MAX_PATH] = {0};
	swprintf_s(cmd, _T("%s --subprocess"), szPath);

	HANDLE job_handle = ::CreateJobObject(nullptr, nullptr);

	PROCESS_INFORMATION proc_info = {};
	STARTUPINFO startup_info = {};
	startup_info.cb = sizeof(startup_info);
	startup_info.wShowWindow = SW_SHOW;

	if (!::CreateProcess(nullptr, cmd, nullptr, nullptr, TRUE, CREATE_NEW_CONSOLE, nullptr, nullptr,
			    &startup_info, &proc_info)) {
		::CloseHandle(job_handle);
		return;
	}
	
	::AssignProcessToJobObject(job_handle, proc_info.hProcess);

	::CloseHandle(proc_info.hThread);
	::CloseHandle(proc_info.hProcess);

	JOBOBJECT_EXTENDED_LIMIT_INFORMATION limit_info = {};
	limit_info.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
	::SetInformationJobObject(job_handle, JobObjectExtendedLimitInformation, &limit_info, sizeof(limit_info));

	jobs.push_back(job_handle);
}

void CMFCDlg::OnBnClickedButtonKillChild()
{
	for (auto &item : jobs) {
		::CloseHandle(item); // 如果父进程异常退出，由系统做这个清理 进而清除子进程
	}

	jobs.clear();
}

void CMFCDlg::OnBnClickedCancel()
{
	// to exit process
	OnBnClickedButtonKillChild();
	CDialogEx::OnCancel();
}

void CMFCDlg::OnBnClickedButtonCrash()
{
	int *temp = new int[200];
	temp[900] = 123456;
	delete[] temp;
	delete[] temp;
}

void CMFCDlg::OnBnClickedButtonMsgbox()
{
	MessageBox(0, 0, 0);
}

void CMFCDlg::OnBnClickedButtonWhile()
{
	while (1) {
		Sleep(200);
	}
}
