
// ExplorerPlugTestDlg.cpp : ʵ���ļ�
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

void CALLBACK RecalcText (HWND hwnd, UINT, UINT_PTR, DWORD)
{
	HWND hwndFind = GetForegroundWindow ();
	g_szPath[0] = TEXT ('/0');
	g_szItem[0] = TEXT ('/0');

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
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CExplorerPlugTestDlg �Ի���



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


// CExplorerPlugTestDlg ��Ϣ�������

BOOL CExplorerPlugTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CExplorerPlugTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

		HDC hdc;
		hdc = ::GetDC (m_hWnd);
		::SetBkMode (hdc, TRANSPARENT);
		TextOut (hdc, 0, 0, g_szPath, lstrlen (g_szPath));
		TextOut (hdc, 0, 20, g_szItem, lstrlen (g_szItem));
		::ReleaseDC (m_hWnd,hdc);
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CExplorerPlugTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CExplorerPlugTestDlg::OnCreate (LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate (lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	::SetTimer (lpCreateStruct->hwndParent, 1, 200, RecalcText);

	return 0;
}
