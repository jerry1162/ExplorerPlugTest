
// ExplorerPlugTestDlg.h : ͷ�ļ�
//

#pragma once


// CExplorerPlugTestDlg �Ի���
class CExplorerPlugTestDlg : public CDialogEx
{
// ����
public:
	CExplorerPlugTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_EXPLORERPLUGTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct);
};
