// IpcClientDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IpcClientDemo.h"
#include "IpcClientDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "../libEasyPlayer/libEasyPlayerAPI.h"
#pragma comment(lib, "../Debug/libEasyPlayer.lib")

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

// CIpcClientDemoDlg 对话框

CIpcClientDemoDlg::CIpcClientDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIpcClientDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_WindowId	=	-1;
	m_ChannelId	=	-1;
	bDrag		=	false;

	InitialComponents();
}

CIpcClientDemoDlg::~CIpcClientDemoDlg()
{
}

void CIpcClientDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CIpcClientDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_PREVIEW, &CIpcClientDemoDlg::OnBnClickedButtonPreview)
	ON_BN_CLICKED(IDC_CHECK_OSD, &CIpcClientDemoDlg::OnBnClickedCheckOsd)
END_MESSAGE_MAP()

// CIpcClientDemoDlg 消息处理程序

BOOL CIpcClientDemoDlg::OnInitDialog()
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	int isWidth = GetSystemMetrics ( SM_CXSCREEN );
	int isHeight= GetSystemMetrics ( SM_CYSCREEN );
	int idWidth = 800;
	int idHeight = 600;

	::SetWindowPos(this->m_hWnd,HWND_BOTTOM,(isWidth-idWidth)/2,(isHeight-idHeight)/2,idWidth,idHeight,SWP_NOZORDER);

	CreateComponents();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CIpcClientDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

//  消息处理程序
LRESULT CIpcClientDemoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (WM_PAINT == message || WM_SIZE==message)
	{
		UpdateComponents();
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}

bool MByteToWChar(LPCSTR lpcszStr, LPWSTR lpwszStr, DWORD dwSize)
{
	// Get the required size of the buffer that receives the Unicode
	// string.
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar (CP_ACP, 0, lpcszStr, -1, NULL, 0);

	if(dwSize < dwMinSize)
	{
		return false;
	}

	// Convert headers from ASCII to Unicode.
	MultiByteToWideChar (CP_ACP, 0, lpcszStr, -1, lpwszStr, dwMinSize);
	return true;
}

BOOL CIpcClientDemoDlg::DestroyWindow()
{
	DeleteComponents();
	EasyPlayer_Release();

	return CDialogEx::DestroyWindow();
}

void CIpcClientDemoDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	HWND hWnd = ::GetParent(GetSafeHwnd());
	::PostMessageW(hWnd, WM_WINDOW_MAXIMIZED, m_WindowId, 0);

	CDialogEx::OnLButtonDblClk(nFlags, point);
}

void CIpcClientDemoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	bDrag = true;

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CIpcClientDemoDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	bDrag = false;

	CDialogEx::OnLButtonUp(nFlags, point);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CIpcClientDemoDlg::OnPaint()
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
HCURSOR CIpcClientDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CIpcClientDemoDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (bDrag)
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		if (! rcClient.IsRectEmpty())
		{
			int nX = (int)(((float)point.x / (float)rcClient.Width() * 100.0f));
			int nY = (int)(((float)point.y / (float)rcClient.Height() * 100.0f));

			TRACE("X: %d\tY: %d\n", nX, nY);
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void	CIpcClientDemoDlg::SetWindowId(int _windowId)
{
	m_WindowId = _windowId;

	if (m_WindowId == 0)
	{
		//if (NULL != pEdtURL)		pEdtURL->SetWindowText(TEXT("rtsp://121.15.129.227"));
		if (NULL != pEdtURL)		pEdtURL->SetWindowText(TEXT("rtsp://218.204.223.237:554/live/1/66251FC11353191F/e7ooqwcfbqjoo80j.sdp"));
	}
}

void	CIpcClientDemoDlg::SetURL(char *url)
{
	wchar_t wszURL[128] = {0,};
	MByteToWChar(url, wszURL, sizeof(wszURL)/sizeof(wszURL[0]));
	if (NULL != pEdtURL)		pEdtURL->SetWindowText(wszURL);
}

void	CIpcClientDemoDlg::SetShownToScale(int shownToScale)
{
	if (m_ChannelId > 0)
	{
		EasyPlayer_SetShownToScale(m_ChannelId, shownToScale);
	}
}

void	CIpcClientDemoDlg::InitialComponents()
{
	pDlgRender	=	NULL;
	pEdtURL		=	NULL;
	pEdtUsername=	NULL;
	pEdtPassword=	NULL;
	pChkOSD		=	NULL;
	pSliderCache=	NULL;
	pBtnPreview	=	NULL;

	EasyPlayer_Init();
}

void	CIpcClientDemoDlg::CreateComponents()
{
	if (NULL == pDlgRender)
	{
		pDlgRender = new CDlgRender();
		pDlgRender->Create(IDD_DIALOG_RENDER, this);
		pDlgRender->ShowWindow(SW_SHOW);
	}

	__CREATE_WINDOW(pEdtURL,		CEdit,		IDC_EDIT_RTSP_URL);
	__CREATE_WINDOW(pEdtUsername,	CEdit,		IDC_EDIT_USERNAME);
	__CREATE_WINDOW(pEdtPassword,	CEdit,		IDC_EDIT_PASSWORD);
	__CREATE_WINDOW(pChkOSD,		CButton,	IDC_CHECK_OSD);
	__CREATE_WINDOW(pSliderCache,	CSliderCtrl,IDC_SLIDER_CACHE);
	__CREATE_WINDOW(pBtnPreview,	CButton,	IDC_BUTTON_PREVIEW);

	if (NULL != pEdtURL)		pEdtURL->SetWindowText(TEXT("rtsp://218.204.223.237:554/live/1/0547424F573B085C/gsfp90ef4k0a6iap.sdp"));
	if (NULL != pEdtUsername)	pEdtUsername->SetWindowText(TEXT("admin"));
	if (NULL != pEdtPassword)	pEdtPassword->SetWindowText(TEXT("12345"));
	if (NULL != pSliderCache)	pSliderCache->SetRange(1, 10);
	if (NULL != pSliderCache)	pSliderCache->SetPos(3);

	if (NULL != pBtnPreview)		pBtnPreview->SetWindowText(TEXT("Play"));
}

void	CIpcClientDemoDlg::UpdateComponents()
{
	CRect	rcClient;
	GetClientRect(&rcClient);
	if (rcClient.IsRectEmpty())		return;

	CRect	rcRender;
	rcRender.SetRect(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom-200);
	__MOVE_WINDOW(pDlgRender, rcRender);
	if (NULL != pDlgRender)		pDlgRender->Invalidate();

	CRect	rcURL;
	rcURL.SetRect(rcClient.left, rcRender.bottom+2, rcClient.right-280, rcRender.bottom+25);
	__MOVE_WINDOW(pEdtURL, rcURL);
	if (NULL != pEdtURL)		pEdtURL->Invalidate();

	CRect	rcUsername;
	rcUsername.SetRect(rcURL.right+2, rcURL.top, rcURL.right+2+50, rcURL.bottom);
	__MOVE_WINDOW(pEdtUsername, rcUsername);
	if (NULL != pEdtUsername)		pEdtUsername->Invalidate();

	CRect	rcPassword;
	rcPassword.SetRect(rcUsername.right+2, rcUsername.top, rcUsername.right+2+rcUsername.Width(), rcUsername.bottom);
	__MOVE_WINDOW(pEdtPassword, rcPassword);
	if (NULL != pEdtPassword)		pEdtPassword->Invalidate();

	CRect	rcOSD;
	rcOSD.SetRect(rcPassword.right+2, rcPassword.top, rcPassword.right+2+38, rcPassword.bottom);
	__MOVE_WINDOW(pChkOSD, rcOSD);
	if (NULL != pChkOSD)		pChkOSD->Invalidate();
	CRect	rcCache;
	rcCache.SetRect(rcOSD.right+2, rcOSD.top, rcOSD.right+2+60, rcOSD.bottom);
	__MOVE_WINDOW(pSliderCache, rcCache);
	if (NULL != pSliderCache)		pSliderCache->Invalidate();

	CRect	rcPreview;
	rcPreview.SetRect(rcCache.right+2, rcURL.top, rcClient.right, rcURL.bottom);
	__MOVE_WINDOW(pBtnPreview, rcPreview);
	if (NULL != pBtnPreview)		pBtnPreview->Invalidate();
}

void	CIpcClientDemoDlg::DeleteComponents()
{
	if (m_ChannelId > 0)
	{
		EasyPlayer_CloseStream(m_ChannelId);
		m_ChannelId = -1;
	}
	__DELETE_WINDOW(pDlgRender);
}

bool __WCharToMByte(LPCWSTR lpcwszStr, LPSTR lpszStr, DWORD dwSize)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,NULL,0,NULL,FALSE);
	if(dwSize < dwMinSize)
	{
		return false;
	}
	WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,lpszStr,dwSize,NULL,FALSE);
	return true;
}

void CIpcClientDemoDlg::OnBnClickedButtonPreview()
{
	RenderFormat	=	DISPLAY_FORMAT_YV12;

	if (m_ChannelId > 0)
	{
		EasyPlayer_CloseStream(m_ChannelId);
		m_ChannelId = -1;

		if (NULL != pDlgRender)	pDlgRender->SetChannelId(m_ChannelId);

		if (NULL != pDlgRender)			pDlgRender->Invalidate();
		if (NULL != pBtnPreview)		pBtnPreview->SetWindowText(TEXT("Play"));
	}
	else
	{
		wchar_t wszURL[128] = {0,};
		if (NULL != pEdtURL)	pEdtURL->GetWindowTextW(wszURL, sizeof(wszURL));
		if (wcslen(wszURL) < 1)		return;

		wchar_t wszUsername[32] = {0,};
		wchar_t wszPassword[32] = {0,};
		if (NULL != pEdtUsername)	pEdtUsername->GetWindowText(wszUsername, sizeof(wszUsername));
		if (NULL != pEdtPassword)	pEdtPassword->GetWindowText(wszPassword, sizeof(wszPassword));

		char szURL[128] = {0,};
		__WCharToMByte(wszURL, szURL, sizeof(szURL)/sizeof(szURL[0]));
		char szUsername[32] = {0,};
		char szPassword[32] = {0,};
		__WCharToMByte(wszUsername, szUsername, sizeof(szUsername)/sizeof(szUsername[0]));
		__WCharToMByte(wszPassword, szPassword, sizeof(szPassword)/sizeof(szPassword[0]));

		HWND hWnd = NULL;
		if (NULL != pDlgRender)	hWnd = pDlgRender->GetSafeHwnd();
		m_ChannelId = EasyPlayer_OpenStream(szURL, hWnd, (RENDER_FORMAT)RenderFormat, 0x01, szUsername, szPassword);

		if (m_ChannelId > 0)
		{
			int iPos = pSliderCache->GetPos();
			EasyPlayer_SetFrameCache(m_ChannelId, iPos);		//设置缓存
			EasyPlayer_PlaySound(m_ChannelId);
			if (NULL != pDlgRender)	pDlgRender->SetChannelId(m_ChannelId);

			if (NULL != pBtnPreview)		pBtnPreview->SetWindowText(TEXT("Stop"));
		}
	}
}

void CIpcClientDemoDlg::OnBnClickedCheckOsd()
{
	int nShow = 0x00;

	if (NULL != pChkOSD)		nShow = pChkOSD->GetCheck();

	if (m_ChannelId > 0)
	{
		EasyPlayer_ShowStatisticalInfo(m_ChannelId, nShow);
	}
}

void CIpcClientDemoDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	CDialogEx::OnRButtonUp(nFlags, point);
}

void CIpcClientDemoDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( NULL != pScrollBar && NULL != pSliderCache &&
		pSliderCache->GetDlgCtrlID() == pScrollBar->GetDlgCtrlID())
	{
		int iPos = pSliderCache->GetPos();

		if (m_ChannelId > 0)
		{
			EasyPlayer_SetFrameCache(m_ChannelId, iPos);
		}
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}