
// UkeyKitDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UkeyKit.h"
#include "UkeyKitDlg.h"
#include "afxdialogex.h"
//personal 
#include <iostream>
using namespace std;
#include <fstream>
#include <direct.h>
#include <shellapi.h>
#include <WinIoCtl.h>


#define DISK_PATH_LEN 20
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CUkeyKitDlg 对话框




CUkeyKitDlg::CUkeyKitDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUkeyKitDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUkeyKitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_pro);
	DDX_Control(pDX, IDC_BUTTON1, m_writeG4);
	DDX_Control(pDX, IDC_BUTTON3, m_bingK);
}

BEGIN_MESSAGE_MAP(CUkeyKitDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CUkeyKitDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CUkeyKitDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CUkeyKitDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CUkeyKitDlg 消息处理程序

BOOL CUkeyKitDlg::OnInitDialog()
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



	//初始化代码
	
	FindAllDrivers();

	/* initial the progress  */
	m_pro.SetRange(0,10);//设置进度条的显示范围
    m_pro.SetPos(0);//进度条初始状态为显示零
	UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUkeyKitDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUkeyKitDlg::OnPaint()
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
HCURSOR CUkeyKitDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUkeyKitDlg::OnBnClickedButton2()
{
	FindAllDrivers();
}

/* install grub4dow and bing key */
void CUkeyKitDlg::OnBnClickedButton1()
{
	m_bingK.EnableWindow(false); /* set button of bingKey is unable */
	addPross(-1);
	CComboBox* Driver=(CComboBox*)GetDlgItem(IDC_COMBO1);
	int nIndex = Driver->GetCurSel();
	char s[40];
	Driver->GetLBText( nIndex, (LPTSTR)s);
	char *name = strstr(s, "(");
	if (!name) {
		m_bingK.EnableWindow(true); /* set button of bingKey is unable */
		return ;
	}
	panfu = name[1];
	writePbr();
	runThread();
	//writeInforToServer();
}
/**
* find all drivers and show removeable drivers
*/
void CUkeyKitDlg::FindAllDrivers()
{
		CComboBox* Driver=(CComboBox*)GetDlgItem(IDC_COMBO1);
		Driver->ResetContent();
		DWORD dwNumBytesForDriveStrings;//实际存储驱动器号的字符串长度
		HANDLE hHeap;
		LPSTR lp;
		CString strLogdrive;

       //获得实际存储驱动器号的字符串长度
		dwNumBytesForDriveStrings=GetLogicalDriveStrings(0,NULL)*sizeof(TCHAR);

       //如果字符串不为空，则表示有正常的驱动器存在
		if (dwNumBytesForDriveStrings!=0) {
		//分配字符串空间
		hHeap=GetProcessHeap();
		lp=(LPSTR)HeapAlloc(hHeap,HEAP_ZERO_MEMORY,dwNumBytesForDriveStrings);

       //获得标明所有驱动器的字符串
		GetLogicalDriveStrings(HeapSize(hHeap,0,lp),lp);

        //将驱动器一个个放到下拉框中
		char s[40];
		while (*lp!=0) {
			UINT nDriveType = GetDriveType(lp);
			if (nDriveType == DRIVE_REMOVABLE){
			   sprintf(s, "%s (%c:) %d MB", "可移动驱动器", lp[0],getTotalSpace(lp) );
			   Driver->AddString(s);
			}
            lp=_tcschr(lp,0)+1;
		}
	}
		
}
/******************************************************************************
* Function: get disk's physical number from its drive letter
*           e.g. C-->0 (C: is on disk0)
* input: letter, drive letter
* output: N/A
* return: Succeed, disk number
*         Fail, -1
******************************************************************************/
DWORD CUkeyKitDlg::GetPhysicalDriveFromPartitionLetter(CHAR letter)
{
    HANDLE hDevice;               // handle to the drive to be examined
    BOOL result;                 // results flag
    DWORD readed;                   // discard results
    STORAGE_DEVICE_NUMBER number;   //use this to get disk numbers
 
    CHAR path[DISK_PATH_LEN];
    sprintf(path, "\\\\.\\%c:", letter);
    hDevice = CreateFile(path, // drive to open
                         GENERIC_READ | GENERIC_WRITE,    // access to the drive
                         FILE_SHARE_READ | FILE_SHARE_WRITE,    //share mode
                         NULL,             // default security attributes
                         OPEN_EXISTING,    // disposition
                         0,                // file attributes
                         NULL);            // do not copy file attribute
    if (hDevice == INVALID_HANDLE_VALUE) // cannot open the drive
    {
        fprintf(stderr, "CreateFile() Error: %ld\n", GetLastError());
        return DWORD(-1);
    }
 
    result = DeviceIoControl(
                hDevice,                // handle to device
                IOCTL_STORAGE_GET_DEVICE_NUMBER, // dwIoControlCode
                NULL,                            // lpInBuffer
                0,                               // nInBufferSize
                &number,           // output buffer
                sizeof(number),         // size of output buffer
                &readed,       // number of bytes returned
                NULL      // OVERLAPPED structure
            );
    if (!result) // fail
    {
        fprintf(stderr, "IOCTL_STORAGE_GET_DEVICE_NUMBER Error: %ld\n", GetLastError());
        (void)CloseHandle(hDevice);
        return (DWORD)-1;
    }
    //printf("%d %d %d\n\n", number.DeviceType, number.DeviceNumber, number.PartitionNumber);
 
    (void)CloseHandle(hDevice);
    return number.DeviceNumber;
}
/*
* get totol  space of disk
* return : MB
*/
int CUkeyKitDlg::getTotalSpace(const char* lpRootPathName)
{
	unsigned long long available,total,free;
    if(GetDiskFreeSpaceEx(lpRootPathName,(ULARGE_INTEGER*)&available,(ULARGE_INTEGER*)&total,(ULARGE_INTEGER*)&free)){
      return total>>20;
    }else{
       return -1;
    }
}

/*  writing pbr to the UKey */  
void CUkeyKitDlg::writePbr(){
	
	ofstream file_run; 
    char dir[300];
    sprintf(dir, "%s%s", getcwd(NULL, 0),"\\config\\rum.bat");
    file_run.open(dir);
    file_run <<  getcwd(NULL, 0)
		 << "\\config\\grubinst.exe --install-partition=0  --pause (hd"
         << GetPhysicalDriveFromPartitionLetter(panfu)
		 << ") 2>" <<  getcwd(NULL, 0)
		 << "\\config\\writePbr.txt"
		<< endl;
	 file_run.flush();
	 file_run.close();
	 ShellExecute(NULL,"open",dir,"",NULL,SW_HIDE); 
	 addPross(2);
}

/**
* checking writePbr success or failed
*/
void CUkeyKitDlg::checking()
{   
	 ofstream file_run; 
	 /* kill  thread of grubinst.exe */
	 char dir[300];
     sprintf(dir, "%s%s", getcwd(NULL, 0),"\\config\\rum_close.bat");
     file_run.open(dir);
	 file_run << "taskkill  /f /t /im grubinst.exe" << endl;
     file_run.flush();
	 file_run.close();
	 ShellExecute(NULL,"open",dir,"",NULL,SW_HIDE); 
	/* checking writePbr.txt that includeing the result of ShellExecute */

    sprintf(dir, "%s%s", getcwd(NULL, 0),"\\config\\writePbr.txt");
    ifstream file_wr; 
	file_wr.open(dir);
	char content[100];
	file_wr.get(content, 100, '\n');

	char *sub = "successfully";
	if (!strstr(content, sub)) {
		MessageBox( "write pbr", "write pbr try again!", MB_ICONHAND);
	file_wr.close();
		file_wr.close();
	  return ;
	}
	 file_wr.close();
	 addPross(-1);
	 m_bingK.EnableWindow(true); /* set button of bingKey is able */
 
}


/* writing info of the UKey to web server */
/*void CUkeyKitDlg:: writeInforToServer(){
	char dir[300];
    sprintf(dir, "%s%s", getcwd(NULL, 0),"\\config\\usbkey.exe ");
	   ShellExecute(NULL,"open",dir,"",NULL,SW_HIDE); 
	
	 addPross(1);
}*/
/* copying grldr to the root directory of the UKey  */ 
UINT copyGrldr_(LPVOID lpParameter){
	
	CUkeyKitDlg* t = (CUkeyKitDlg *)lpParameter;
	   //write 1.bat file
    ofstream file; 
	char dir[300];
	sprintf(dir, "%s%s", getcwd(NULL, 0),"\\config\\run_copy.bat");
    file.open(dir);
	file <<"xcopy  "<< getcwd(NULL, 0) <<"\\config\\grldr  " << t->panfu << ":\\  /Y " << endl;
	file <<"xcopy  "<< getcwd(NULL, 0) <<"\\config\\menu.lst  " << t->panfu << ":\\  /Y " << endl;
	//file <<"del " << dir << endl;  
	file.flush();
    file.close();  
    //run   1.bat fdled
    SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = dir; 
	ShExecInfo.lpParameters = ""; 
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL; 
	ShellExecuteEx(&ShExecInfo);

	 int i,j;
     t->m_pro.GetRange(i,j);//获取进度条表示的范围
     int m ;  int npos;
	 for(m = 0;m < 8; m++){
	     npos= t->m_pro.GetPos()+ 1;//当前进度条位置
         if (npos >= j)//如果进度条当前的进度比进度条所能表示的进度大
         npos=j;//则进度条进度到最大位置不再前进
	     t->m_pro.SetPos(npos);//设置进度条新的进度      
        // t->UpdateData(FALSE);
	     Sleep(2000);
	}
	WaitForSingleObject(ShExecInfo.hProcess,INFINITE);
	t->checking();
    return 0;
}

/*
*start three threads to handing 
* thread ：copying grldr to the root directory of the UKey 
*/

void CUkeyKitDlg::runThread()
{
 CWinThread      *pMyFirstWorker = AfxBeginThread(copyGrldr_,this, THREAD_PRIORITY_NORMAL,
  0,0,NULL );
// WaitForSingleObject(pMyFirstWorker,INFINITE);
 
}


/*
* setting progress 
* param: length that add the length to the progess
*/
void CUkeyKitDlg::addPross(int length) {
	
	  int i,j;
      m_pro.GetRange(i,j);//获取进度条表示的范围
     int npos=m_pro.GetPos();//当前进度条位置
     if (length == -1) {
		  npos=i;
		  m_pro.SetPos(npos);//设置进度条新的进度      
		  // UpdateData(FALSE);
		  return;
	}
     
     npos += length;//单击此按钮一次使进度条进度自动加5
     if (npos >= j)//如果进度条当前的进度比进度条所能表示的进度大
        npos=j;//则进度条进度到最大位置不再前进
	 m_pro.SetPos(npos);//设置进度条新的进度      
     //UpdateData(FALSE);
}



void CUkeyKitDlg::OnBnClickedButton3()
{
	m_writeG4.EnableWindow(false); /* set button of bingKey is unable */
	char dir[300];
	addPross(1);
    sprintf(dir, "%s%s", getcwd(NULL, 0),"\\config\\usbkey.exe ");
    ShellExecute(NULL,"open",dir,"",NULL,SW_HIDE); 
    addPross(5);
	 Sleep(2000);
	 addPross(5);
	  Sleep(1000);
	 addPross(-1);
	m_writeG4.EnableWindow(true); /* set button of bingKey is able */
}
