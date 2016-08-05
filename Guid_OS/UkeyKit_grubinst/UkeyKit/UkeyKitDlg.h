
// UkeyKitDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CUkeyKitDlg 对话框
class CUkeyKitDlg : public CDialogEx
{
// 构造
public:
	CUkeyKitDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_UKEYKIT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
