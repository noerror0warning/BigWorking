
// damoDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "damo.h"
#include "damoDlg.h"
#include "afxdialogex.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CdamoDlg 对话框



CdamoDlg::CdamoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DAMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CdamoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CdamoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CdamoDlg::OnBnClickedButtonOpen)
END_MESSAGE_MAP()


// CdamoDlg 消息处理程序

BOOL CdamoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CComboBox* cmb_open = ((CComboBox*)GetDlgItem(IDC_COMBO_PICTURE));
	cmb_open->InsertString(0, _T("上图"));
	cmb_open->InsertString(1, _T("下图"));
	cmb_open->SetCurSel(0);

	CWnd* pWnd1 = GetDlgItem(IDC_PICTURE1);//CWnd是MFC窗口类的基类,提供了微软基础类库中所有窗口类的基本功能。
	pWnd1->GetClientRect(&rect1);//GetClientRect为获得控件相自身的坐标大小
	namedWindow("src1", WINDOW_AUTOSIZE);//设置窗口名
	cvResizeWindow("src1", rect1.Width(), rect1.Height());
	HWND hWndl1 = (HWND)cvGetWindowHandle("src1");//hWnd 表示窗口句柄,获取窗口句柄
	HWND hParent11 = ::GetParent(hWndl1);//GetParent函数一个指定子窗口的父窗口句柄
	::SetParent(hWndl1, GetDlgItem(IDC_PICTURE1)->m_hWnd);
	::ShowWindow(hParent11, SW_HIDE);

	CWnd* pWnd12 = GetDlgItem(IDC_PICTURE2);//CWnd是MFC窗口类的基类,提供了微软基础类库中所有窗口类的基本功能。
	pWnd12->GetClientRect(&rect2);//GetClientRect为获得控件相自身的坐标大小
	namedWindow("src2", WINDOW_AUTOSIZE);//设置窗口名
	cvResizeWindow("src2", rect2.Width(), rect2.Height());
	HWND hWndl2 = (HWND)cvGetWindowHandle("src2");//hWnd 表示窗口句柄,获取窗口句柄
	HWND hParent12 = ::GetParent(hWndl2);//GetParent函数一个指定子窗口的父窗口句柄
	::SetParent(hWndl2, GetDlgItem(IDC_PICTURE2)->m_hWnd);
	::ShowWindow(hParent12, SW_HIDE);

	CWnd* pWnd13 = GetDlgItem(IDC_PICTURE3);//CWnd是MFC窗口类的基类,提供了微软基础类库中所有窗口类的基本功能。
	pWnd13->GetClientRect(&rect3);//GetClientRect为获得控件相自身的坐标大小
	namedWindow("src3", WINDOW_AUTOSIZE);//设置窗口名
	cvResizeWindow("src3", rect3.Width(), rect3.Height());//调整窗口大小
	HWND hWndl3 = (HWND)cvGetWindowHandle("src3");//hWnd 表示窗口句柄,获取窗口句柄
	HWND hParent13 = ::GetParent(hWndl3);//GetParent函数一个指定子窗口的父窗口句柄
	::SetParent(hWndl3, GetDlgItem(IDC_PICTURE3)->m_hWnd);
	::ShowWindow(hParent13, SW_HIDE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CdamoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CdamoDlg::OnPaint()
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
HCURSOR CdamoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CdamoDlg::OnBnClickedButtonOpen(){
	// TODO: 在此添加控件通知处理程序代码

	CComboBox* cmb_pic = ((CComboBox*)GetDlgItem(IDC_COMBO_PICTURE));
	int pic = cmb_pic->GetCurSel();

	switch (pic){
	case 0: {
		TCHAR szFilter[] = _T("图片文件 (*.jpg)|*.jpg|All Files (*.*)|*.*||");
		// 构造打开文件对话框   
		CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);
		CString strFilePath;

		//显示打开文件对话框   
		if (IDOK == fileDlg.DoModal())
		{
			// 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里   
			strFilePath = fileDlg.GetPathName();
			USES_CONVERSION;
			char* s = T2A(strFilePath);
			image1 = imread(s);
			double w = rect1.Width();
			double h = rect1.Height();
			double scaleRate = h / double(image1.rows);
			if (scaleRate > double(w / image1.cols))
				scaleRate = double(w / image1.cols);

			int rows = int(image1.rows * scaleRate);
			int cols = int(image1.cols * scaleRate);

			if(scaleRate<1)
				resize(image1, image1, Size(cols,rows));
			//Mat big(image1.rows, image1.cols * 2, image1.type());
			//Mat roi = big(Rect(0, 0, image1.rows, image1.cols));
			//image1.copyTo(roi);
			imshow("src1", image1);
		}
		break;
	}
	case 1: {
		TCHAR szFilter[] = _T("图片文件 (*.jpg)|*.jpg|All Files (*.*)|*.*||");
		// 构造打开文件对话框   
		CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);
		CString strFilePath;

		//显示打开文件对话框   
		if (IDOK == fileDlg.DoModal())
		{
			// 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里   
			strFilePath = fileDlg.GetPathName();
			USES_CONVERSION;
			char* s = T2A(strFilePath);
			image2 = imread(s);
			double w = rect2.Width();
			double h = rect2.Height();
			double scaleRate = h / image2.rows;
			if (scaleRate > w / image2.cols)
				scaleRate = w / image2.cols;
			int rows = int(image2.rows * scaleRate);
			int cols = int(image2.cols * scaleRate);
			if (scaleRate < 1)
				resize(image2, image2, Size(cols,rows));

			imshow("src2", image2);
		}
		break;
	}
	default:
		break;
	}

}
