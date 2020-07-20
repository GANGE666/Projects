
// HookWindowDlg.h : ͷ�ļ�
//

#pragma once


// CHookWindowDlg �Ի���
class CHookWindowDlg : public CDialogEx
{
// ����
public:
	CHookWindowDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_HOOKWINDOW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CListCtrl m_list; // Hook �����б�

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnLogHookWindows(WPARAM wParam,LPARAM lParam); // ��Ϣ��Ӧ����
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonMsga();
	afx_msg void OnBnClickedButtonMsgw();
};
