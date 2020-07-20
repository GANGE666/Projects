
// HookWindowDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HookWindow.h"
#include "HookWindowDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  WM_HOOKMSG WM_USER + 106	// �Լ�������Ϣ


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


// CHookWindowDlg �Ի���




CHookWindowDlg::CHookWindowDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHookWindowDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHookWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST_HOOKWINDOW,m_list);
}

BEGIN_MESSAGE_MAP(CHookWindowDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CHookWindowDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CHookWindowDlg::OnBnClickedButtonStop)
	//ON_BN_CLICKED(IDC_BUTTON_MSGA, &CHookWindowDlg::OnBnClickedButtonMsga)
	//ON_BN_CLICKED(IDC_BUTTON_MSGW, &CHookWindowDlg::OnBnClickedButtonMsgw)

	ON_MESSAGE(WM_HOOKMSG,CHookWindowDlg::OnLogHookWindows) //��Ϣӳ��

END_MESSAGE_MAP()


// CHookWindowDlg ��Ϣ�������

BOOL CHookWindowDlg::OnInitDialog()
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	// �б��ʼ��
	m_list.InsertColumn(0,_T("Hook����"));
	m_list.SetColumnWidth(0,250);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CHookWindowDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CHookWindowDlg::OnPaint()
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
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CHookWindowDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ��Ϣ��Ӧ����
LRESULT CHookWindowDlg::OnLogHookWindows(WPARAM wParam,LPARAM lParam)
{
	static CString str;
	static PMSLLHOOKSTRUCT mouseHookStruct;

	if (wParam)
	{
		TCHAR* str = (TCHAR*)wParam;
		CString msg;
		msg.Format(_T("%s"),str);
		m_list.InsertItem(m_list.GetItemCount(),msg);
		m_list.EnsureVisible(m_list.GetItemCount()-1,FALSE);
	}

	if (lParam)
	{
		HWND hwnd = (HWND)lParam;
		TCHAR title[MAX_PATH];
		::GetWindowText(hwnd,title,MAX_PATH);
		m_list.InsertItem(m_list.GetItemCount(),title);
		m_list.EnsureVisible(m_list.GetItemCount()-1,FALSE);
	}
	

	return 1;
}


HINSTANCE g_hinstDll = NULL;
//
// ��ʼ HOOK
//
void CHookWindowDlg::OnBnClickedButtonStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	g_hinstDll = LoadLibrary(_T("HookDll.dll"));
	if ( NULL == g_hinstDll)
	{
		AfxMessageBox(_T("���� HookDll.dll ʧ��"));
	}

	typedef BOOL (CALLBACK *HookStart)(HWND hwnd);
	HookStart hookStart = NULL;
	hookStart = (HookStart)::GetProcAddress(g_hinstDll,"StartHook");
	if ( NULL == hookStart)
	{
		AfxMessageBox(_T("��ȡ StartHook ����ʧ��"));
		return;
	}
	bool ret = hookStart(m_hWnd);
	if (ret)
	{
		m_list.InsertItem(m_list.GetItemCount(),_T("�������ӳɹ�"));
		m_list.EnsureVisible(m_list.GetItemCount()-1,FALSE);
	}
	else
	{
		m_list.InsertItem(m_list.GetItemCount(),_T("��������ʧ��"));
		m_list.EnsureVisible(m_list.GetItemCount()-1,FALSE);
	}
}

//
// ��ֹ HOOK
//
void CHookWindowDlg::OnBnClickedButtonStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	typedef BOOL (CALLBACK* HookStop)();
	HookStop hookStop = NULL;
	if (NULL ==  g_hinstDll) // һ��Ҫ������жϣ�����Ϊ�յĻ��Ͳ���Ҫ�����¼��أ�������ǲ�ͬ��ʵ��
	{
		g_hinstDll = LoadLibrary(_T("HookDll.dll"));
		if (g_hinstDll == NULL)
		{
			AfxMessageBox(_T("���� HookDll.dll ʧ��"));
			return;
		}

	}
	
	hookStop = ::GetProcAddress(g_hinstDll,"StopHook");
	if (hookStop == NULL)
	{
		AfxMessageBox(_T("��ȡ StopHook ʧ��"));
		FreeLibrary(g_hinstDll);  
		g_hinstDll=NULL;  
		return;
	}

	hookStop();
	if (g_hinstDll!= NULL)
	{
		::FreeLibrary(g_hinstDll);
	}
	m_list.InsertItem(m_list.GetItemCount(),_T("��ֹHOOK�ɹ�"));

}

// MessageBoxA
void CHookWindowDlg::OnBnClickedButtonMsga()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	MessageBoxA(m_hWnd,"����������MessageBoxA...","����",0);
}

// MessageBoxW
void CHookWindowDlg::OnBnClickedButtonMsgw()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MessageBoxW(_T("����������MessageBoxW..."),_T("�Ǻ�"),0);
}
