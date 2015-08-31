
// IpcClientDemoDlg.h : ͷ�ļ�
//

#pragma once
#include "DlgRender.h"

// CIpcClientDemoDlg �Ի���
class CIpcClientDemoDlg : public CDialogEx
{
// ����
public:
	CIpcClientDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CIpcClientDemoDlg();

	void	SetWindowId(int _windowId);
	void	SetShownToScale(int shownToScale);
	void	SetURL(char *url);

	bool	bDrag;

	int		m_WindowId;
	int		m_ChannelId;
	CDlgRender	*pDlgRender;
	CEdit	*pEdtURL;		//IDC_EDIT_RTSP_URL
	CEdit	*pEdtUsername;	//IDC_EDIT_USERNAME
	CEdit	*pEdtPassword;	//IDC_EDIT_PASSWORD
	CButton	*pChkOSD;		//IDC_CHECK_OSD
	CSliderCtrl	*pSliderCache;	//IDC_SLIDER_CACHE
	CButton	*pBtnPreview;	//IDC_BUTTON_PREVIEW
	void	InitialComponents();
	void	CreateComponents();
	void	UpdateComponents();
	void	DeleteComponents();

// �Ի�������
	enum { IDD = IDD_IPCCLIENTDEMO_DIALOG };

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
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnBnClickedButtonPreview();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedCheckOsd();

};
