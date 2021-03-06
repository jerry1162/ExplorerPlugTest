
// ExplorerPlugTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ExplorerPlugTest.h"
#include "ExplorerPlugTestDlg.h"
#include "afxdialogex.h"

#include <shlobj.h>
#include <exdisp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



TCHAR g_szPath[MAX_PATH];
TCHAR g_szItem[MAX_PATH];
int g_pItemCount;

void CALLBACK RecalcText (HWND hwnd, UINT, UINT_PTR, DWORD)
{
	HWND hwndFind = GetForegroundWindow ();
	g_szPath[0] = TEXT ('/0');
	g_szItem[0] = TEXT ('/0');
	g_pItemCount = 0;

	IShellWindows *psw;
	if (SUCCEEDED (CoCreateInstance (CLSID_ShellWindows, NULL, CLSCTX_ALL,
		IID_IShellWindows, (void**) &psw))) {
		VARIANT v;
		V_VT (&v) = VT_I4;
		IDispatch  *pdisp;
		BOOL fFound = FALSE;
		for (V_I4 (&v) = 0; !fFound && psw->Item (v, &pdisp) == S_OK; V_I4 (&v)++) {
			IWebBrowserApp *pwba;
			if (SUCCEEDED (pdisp->QueryInterface (IID_IWebBrowserApp, (void**) &pwba))) {
				HWND hwndWBA;
				if (SUCCEEDED (pwba->get_HWND ((LONG_PTR*) &hwndWBA)) &&
					hwndWBA == hwndFind) {
					fFound = TRUE;
					IServiceProvider *psp;
					if (SUCCEEDED (pwba->QueryInterface (IID_IServiceProvider, (void**) &psp))) {
						IShellBrowser *psb;
						if (SUCCEEDED (psp->QueryService (SID_STopLevelBrowser,
							IID_IShellBrowser, (void**) &psb))) {
							IShellView *psv;
							if (SUCCEEDED (psb->QueryActiveShellView (&psv))) {
								IFolderView *pfv;
								if (SUCCEEDED (psv->QueryInterface (IID_IFolderView,
									(void**) &pfv))) {
									int* Focus=&g_pItemCount;
									if (SUCCEEDED (pfv->ItemCount (SVGIO_SELECTION, Focus)))
									{
										g_pItemCount = *Focus;
									}
									
									IPersistFolder2 *ppf2;
									if (SUCCEEDED (pfv->GetFolder (IID_IPersistFolder2,
										(void**) &ppf2))) {
										LPITEMIDLIST pidlFolder;
										if (SUCCEEDED (ppf2->GetCurFolder (&pidlFolder))) {
											if (!SHGetPathFromIDList (pidlFolder, g_szPath)) {
												lstrcpyn (g_szPath, TEXT ("<not a directory>"), MAX_PATH);
											}
											int iFocus;
											if (SUCCEEDED (pfv->GetFocusedItem (&iFocus))) {
												LPITEMIDLIST pidlItem;
												if (SUCCEEDED (pfv->Item (iFocus, &pidlItem))) {
													IShellFolder *psf;
													if (SUCCEEDED (ppf2->QueryInterface (IID_IShellFolder,
														(void**) &psf))) {
														STRRET str;
														if (SUCCEEDED (psf->GetDisplayNameOf (pidlItem,
															SHGDN_INFOLDER,
															&str))) {
															StrRetToBuf (&str, pidlItem, g_szItem, MAX_PATH);
														}
														psf->Release ();
													}
													CoTaskMemFree (pidlItem);
												}
											}
											CoTaskMemFree (pidlFolder);
										}
										ppf2->Release ();
									}
									pfv->Release ();
								}
								psv->Release ();
							}
							psb->Release ();
						}
						psp->Release ();
					}
				}
				pwba->Release ();
			}
			pdisp->Release ();
		}
		psw->Release ();
	}
	InvalidateRect (hwnd, NULL, TRUE);
}
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CExplorerPlugTestDlg 对话框



CExplorerPlugTestDlg::CExplorerPlugTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExplorerPlugTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExplorerPlugTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CExplorerPlugTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE ()
END_MESSAGE_MAP ()


// CExplorerPlugTestDlg 消息处理程序

BOOL CExplorerPlugTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CExplorerPlugTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CExplorerPlugTestDlg::OnPaint()
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

		CString s;
		s.Format (_T("选中项:%d"), g_pItemCount);
		HDC hdc;
		hdc = ::GetDC (m_hWnd);
		::SetBkMode (hdc, TRANSPARENT);
		TextOut (hdc, 0, 0, g_szPath, lstrlen (g_szPath));
		TextOut (hdc, 0, 20, g_szItem, lstrlen (g_szItem));
		TextOut (hdc, 0, 40,s.GetString (),s.GetLength ());
		::ReleaseDC (m_hWnd,hdc);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CExplorerPlugTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CExplorerPlugTestDlg::OnCreate (LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate (lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	::SetTimer (lpCreateStruct->hwndParent, 1, 200, RecalcText);

	return 0;
}
