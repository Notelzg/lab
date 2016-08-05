
// UkeyKitDlg.cpp : ʵ���ļ�
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


// CUkeyKitDlg �Ի���




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


// CUkeyKitDlg ��Ϣ�������

BOOL CUkeyKitDlg::OnInitDialog()
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



	//��ʼ������
	
	FindAllDrivers();

	/* initial the progress  */
	m_pro.SetRange(0,10);//���ý���������ʾ��Χ
    m_pro.SetPos(0);//��������ʼ״̬Ϊ��ʾ��
	UpdateData(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUkeyKitDlg::OnPaint()
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
		DWORD dwNumBytesForDriveStrings;//ʵ�ʴ洢�������ŵ��ַ�������
		HANDLE hHeap;
		LPSTR lp;
		CString strLogdrive;

       //���ʵ�ʴ洢�������ŵ��ַ�������
		dwNumBytesForDriveStrings=GetLogicalDriveStrings(0,NULL)*sizeof(TCHAR);

       //����ַ�����Ϊ�գ����ʾ������������������
		if (dwNumBytesForDriveStrings!=0) {
		//�����ַ����ռ�
		hHeap=GetProcessHeap();
		lp=(LPSTR)HeapAlloc(hHeap,HEAP_ZERO_MEMORY,dwNumBytesForDriveStrings);

       //��ñ����������������ַ���
		GetLogicalDriveStrings(HeapSize(hHeap,0,lp),lp);

        //��������һ�����ŵ���������
		char s[40];
		while (*lp!=0) {
			UINT nDriveType = GetDriveType(lp);
			if (nDriveType == DRIVE_REMOVABLE){
			   sprintf(s, "%s (%c:) %d MB", "���ƶ�������", lp[0],getTotalSpace(lp) );
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
     t->m_pro.GetRange(i,j);//��ȡ��������ʾ�ķ�Χ
     int m ;  int npos;
	 for(m = 0;m < 8; m++){
	     npos= t->m_pro.GetPos()+ 1;//��ǰ������λ��
         if (npos >= j)//�����������ǰ�Ľ��ȱȽ��������ܱ�ʾ�Ľ��ȴ�
         npos=j;//����������ȵ����λ�ò���ǰ��
	     t->m_pro.SetPos(npos);//���ý������µĽ���      
        // t->UpdateData(FALSE);
	     Sleep(2000);
	}
	WaitForSingleObject(ShExecInfo.hProcess,INFINITE);
	t->checking();
    return 0;
}

/*
*start three threads to handing 
* thread ��copying grldr to the root directory of the UKey 
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
      m_pro.GetRange(i,j);//��ȡ��������ʾ�ķ�Χ
     int npos=m_pro.GetPos();//��ǰ������λ��
     if (length == -1) {
		  npos=i;
		  m_pro.SetPos(npos);//���ý������µĽ���      
		  // UpdateData(FALSE);
		  return;
	}
     
     npos += length;//�����˰�ťһ��ʹ�����������Զ���5
     if (npos >= j)//�����������ǰ�Ľ��ȱȽ��������ܱ�ʾ�Ľ��ȴ�
        npos=j;//����������ȵ����λ�ò���ǰ��
	 m_pro.SetPos(npos);//���ý������µĽ���      
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
