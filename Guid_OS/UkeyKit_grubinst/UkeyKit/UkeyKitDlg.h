
// UkeyKitDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CUkeyKitDlg �Ի���
class CUkeyKitDlg : public CDialogEx
{
// ����
public:
	CUkeyKitDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_UKEYKIT_DIALOG };

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
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();

	char panfu;
	void FindAllDrivers();
	DWORD GetPhysicalDriveFromPartitionLetter(CHAR letter);
	int getTotalSpace(const  char* lpRootPathName);
	//void runBat();
	int showError(char * content, char *tittle);
	CProgressCtrl m_pro;
    void runThread();
	void addPross(int length);
	void checking();
	//void  writeInforToServer();
	void bindKey();
/*  writing pbr to the UKey */
void writePbr();
afx_msg void OnBnClickedButton3();
CButton m_writeG4;
CButton m_bingK;
};
