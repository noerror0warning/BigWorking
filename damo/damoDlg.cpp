
// damoDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "damo.h"
#include "damoDlg.h"
#include "afxdialogex.h"
#include"ImageProcess.h"


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
	ON_BN_CLICKED(IDC_BUTTON_OPERATE, &CdamoDlg::OnBnClickedButtonOperate)
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
	cmb_open->InsertString(2, _T("视频"));
	cmb_open->SetCurSel(0);

	CComboBox* cmb_operation = ((CComboBox*)GetDlgItem(IDC_COMBO_OPERATION));
	cmb_operation->InsertString(0, _T("验证FERNS"));
	cmb_operation->InsertString(1, _T("点对映射"));
	cmb_operation->InsertString(2, _T("几何矫正"));
	cmb_operation->InsertString(3, _T("图像拼接"));
	cmb_operation->InsertString(4, _T("目标检测"));
	cmb_operation->InsertString(5, _T("视频的实时特征检测与可视化"));
	cmb_operation->SetCurSel(0);

	CComboBox* cmb_detection = ((CComboBox*)GetDlgItem(IDC_COMBO_DETECTION));
	cmb_detection->InsertString(0, _T("SIFT"));
	cmb_detection->InsertString(1, _T("SURF"));
	cmb_detection->SetCurSel(0);

	CComboBox* cmb_mapping = ((CComboBox*)GetDlgItem(IDC_COMBO_MAPPING));
	cmb_mapping->InsertString(0, _T("BruteForce"));
	cmb_mapping->InsertString(1, _T("FLANN"));
	cmb_mapping->InsertString(2, _T("RANSAC"));
	cmb_mapping->InsertString(3, _T("Hough Cluster"));
	cmb_mapping->SetCurSel(0);

	isImage1Exist = false;
	isImage2Exist = false;
	isImage3Exist = false;

	block = Mat(4, 4, CV_8UC3, Scalar(255, 255, 255));

	CWnd* pWnd1 = GetDlgItem(IDC_PICTURE1);//CWnd是MFC窗口类的基类,提供了微软基础类库中所有窗口类的基本功能。
	pWnd1->GetClientRect(&rect1);//GetClientRect为获得控件相自身的坐标大小
	namedWindow("src1", WINDOW_AUTOSIZE);//设置窗口名
	cvResizeWindow("src1", rect1.Width(), rect1.Height());
	HWND hWndl1 = (HWND)cvGetWindowHandle("src1");//hWnd 表示窗口句柄,获取窗口句柄
	HWND hParent11 = ::GetParent(hWndl1);//GetParent函数一个指定子窗口的父窗口句柄
	::SetParent(hWndl1, GetDlgItem(IDC_PICTURE1)->m_hWnd);
	::ShowWindow(hParent11, SW_HIDE);
	resize(block, block, Size(rect1.Width(), rect1.Height()));
	showBlock(1);

	CWnd* pWnd12 = GetDlgItem(IDC_PICTURE2);//CWnd是MFC窗口类的基类,提供了微软基础类库中所有窗口类的基本功能。
	pWnd12->GetClientRect(&rect2);//GetClientRect为获得控件相自身的坐标大小
	namedWindow("src2", WINDOW_AUTOSIZE);//设置窗口名
	cvResizeWindow("src2", rect2.Width(), rect2.Height());
	HWND hWndl2 = (HWND)cvGetWindowHandle("src2");//hWnd 表示窗口句柄,获取窗口句柄
	HWND hParent12 = ::GetParent(hWndl2);//GetParent函数一个指定子窗口的父窗口句柄
	::SetParent(hWndl2, GetDlgItem(IDC_PICTURE2)->m_hWnd);
	::ShowWindow(hParent12, SW_HIDE);
	resize(block, block, Size(rect2.Width(), rect2.Height()));
	showBlock(2);

	CWnd* pWnd13 = GetDlgItem(IDC_PICTURE3);//CWnd是MFC窗口类的基类,提供了微软基础类库中所有窗口类的基本功能。
	pWnd13->GetClientRect(&rect3);//GetClientRect为获得控件相自身的坐标大小
	namedWindow("src3", WINDOW_AUTOSIZE);//设置窗口名
	cvResizeWindow("src3", rect3.Width(), rect3.Height());//调整窗口大小
	HWND hWndl3 = (HWND)cvGetWindowHandle("src3");//hWnd 表示窗口句柄,获取窗口句柄
	HWND hParent13 = ::GetParent(hWndl3);//GetParent函数一个指定子窗口的父窗口句柄
	::SetParent(hWndl3, GetDlgItem(IDC_PICTURE3)->m_hWnd);
	::ShowWindow(hParent13, SW_HIDE);
	resize(block, block, Size(rect3.Width(), rect3.Height()));
	showBlock(3);

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




void CdamoDlg::showBlock(int number){
	String window;
	CRect rect;
	switch (number) {
	case 1: {
		window = "src1";
		rect = rect1;
		break;
	}
	case 2: {
		window = "src2";
		rect = rect2;
		break;
	}
	case 3: {
		window = "src3";
		rect = rect3;
		break;
	}
	default:
		break;
	}
	imshow(window, block);
}

/*
	这是显示照片的函数
	number 代表哪一个窗口，1、2、3分别是上下右3个窗口
	pic 是要显示的图片
	这个函数显示时会自适应缩小过大的图片，但是不会改变原图的大小
*/
void CdamoDlg::showPicture(int number, Mat pic){
	String window;
	CRect rect;
	switch (number){
	case 1: {
		window = "src1";
		rect = rect1;
		break;
	}
	case 2: {
		window = "src2";
		rect = rect2;
		break;
	}
	case 3: {
		window = "src3";
		rect = rect3;
		break;
	}
	default:
		break;
	}

	double w = rect.Width();
	double h = rect.Height();
	double scaleRate = h / pic.rows;
	if (scaleRate > w / pic.cols)
		scaleRate = w / pic.cols;
	int rows = int(pic.rows * scaleRate);
	int cols = int(pic.cols * scaleRate);
	if (scaleRate < 1) {				//如果超出范围就复制到小图再显示
		Mat showP;
		resize(pic, showP, Size(cols, rows));
		imshow(window, showP);
	}
	else {
		imshow(window, pic);
	}

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
			showPicture(1, image1);
			isImage1Exist = true;
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
			showPicture(2, image2);
			isImage2Exist = true;
		}
		break;
	}
	case 2: {//视频
		break;
	}
	default:
		break;
	}

}


void CdamoDlg::OnBnClickedButtonOperate()
{
	// TODO: 在此添加控件通知处理程序代码
	CComboBox* cmb_oper = ((CComboBox*)GetDlgItem(IDC_COMBO_OPERATION));
	int oper = cmb_oper->GetCurSel();
	switch (oper){
	//验证FERNS
	case 0: {
		verifyFERNS();
		break;
	}
	//点对映射
	case 1: {
		pointMapping();
		break;
	}
	//几何矫正
	case 2: {
		geometricCorrection();
		break;
	}
	//图像拼接
	case 3: {
		imageMosaic();
		break;
	}
	//目标检测
	case 4: {
		targetDetection();
		break;
	}
	//视频的实时特征检测与可视化
	case 5: {
		videoFeature();
		break;
	}
	default:
		break;
	}
}

//验证FERNS
void CdamoDlg::verifyFERNS(){
	/*
		可以用isImage1Exist、isImage2Exist来判断图片是否已经打开，避免传入空图
		空图的话可以用下面这个发出警报然后再return
		CString timeStr;
		timeStr.Format(_T("图片未打开"));
		AfxMessageBox(timeStr);
	*/
}

//点对映射
void CdamoDlg::pointMapping(){
	CComboBox* cmb_detection = ((CComboBox*)GetDlgItem(IDC_COMBO_DETECTION));
	int detection = cmb_detection->GetCurSel();
	CComboBox* cmb_mapping = ((CComboBox*)GetDlgItem(IDC_COMBO_MAPPING));
	int mapping = cmb_mapping->GetCurSel();
	/*
		detection 是特征检测方法 0是SIFT，1是SURF（两个都要实现）
		mapping   是特征点匹配方法 0-3分别是BruteForce， FLANN， RANSAC，Hough Cluster（至少实现其二）
	
		可以用isImage1Exist、isImage2Exist来判断图片是否已经打开，避免传入空图
		空图的话可以用下面这个发出警报然后再return
		CString timeStr;
		timeStr.Format(_T("图片未打开"));
		AfxMessageBox(timeStr);
	*/
	//下面开始实现

	if (!isImage1Exist) {
		CString timeStr;
		timeStr.Format(_T("图片1未打开，请先打开上图"));
		AfxMessageBox(timeStr);
		return;
	}

	if (!isImage2Exist) {
		CString timeStr;
		timeStr.Format(_T("图片2未打开，请先打开下图"));
		AfxMessageBox(timeStr);
		return;
	}

	Mat out1, out2;
	pointMappingProcess(image1, image2, out1, out2, image3, detection, mapping);
	isImage3Exist = true;
	showPicture(1, out1);
	showPicture(2, out2);
	showPicture(3, image3);
}

//几何矫正
void CdamoDlg::geometricCorrection(){
	CComboBox* cmb_detection = ((CComboBox*)GetDlgItem(IDC_COMBO_DETECTION));
	int detection = cmb_detection->GetCurSel();
	CComboBox* cmb_mapping = ((CComboBox*)GetDlgItem(IDC_COMBO_MAPPING));
	int mapping = cmb_mapping->GetCurSel();
	/*
		detection 是特征检测方法 0是SIFT，1是SURF（两个都要实现）
		mapping   是特征点匹配方法 0-3分别是BruteForce， FLANN， RANSAC，Hough Cluster（至少实现其二）
	
		可以用isImage1Exist、isImage2Exist来判断图片是否已经打开，避免传入空图
		空图的话可以用下面这个发出警报然后再return
		CString timeStr;
		timeStr.Format(_T("图片未打开"));
		AfxMessageBox(timeStr);
	*/
	//下面开始实现
	if (!isImage1Exist) {
		CString timeStr;
		timeStr.Format(_T("图片1未打开，请先打开上图"));
		AfxMessageBox(timeStr);
		return;
	}

	if (!isImage2Exist) {
		CString timeStr;
		timeStr.Format(_T("图片2未打开，请先打开下图"));
		AfxMessageBox(timeStr);
		return;
	}

	Mat out1, out2;
	geometricCorrectionProcess(image1, image2, out1, out2, image3, detection, mapping);
	isImage3Exist = true;
	showPicture(1, out1);
	showPicture(2, out2);
	showPicture(3, image3);
}

//图像拼接
void CdamoDlg::imageMosaic(){
	CComboBox* cmb_detection = ((CComboBox*)GetDlgItem(IDC_COMBO_DETECTION));
	int detection = cmb_detection->GetCurSel();
	CComboBox* cmb_mapping = ((CComboBox*)GetDlgItem(IDC_COMBO_MAPPING));
	int mapping = cmb_mapping->GetCurSel();
	/*
		detection 是特征检测方法 0是SIFT，1是SURF（两个都要实现）
		mapping   是特征点匹配方法 0-3分别是BruteForce， FLANN， RANSAC，Hough Cluster（至少实现其二）
	
		可以用isImage1Exist、isImage2Exist来判断图片是否已经打开，避免传入空图
		空图的话可以用下面这个发出警报然后再return
		CString timeStr;
		timeStr.Format(_T("图片未打开"));
		AfxMessageBox(timeStr);
	*/
	//下面开始实现
	if (!isImage1Exist) {
		CString timeStr;
		timeStr.Format(_T("图片1未打开，请先打开上图"));
		AfxMessageBox(timeStr);
		return;
	}

	if (!isImage2Exist) {
		CString timeStr;
		timeStr.Format(_T("图片2未打开，请先打开下图"));
		AfxMessageBox(timeStr);
		return;
	}

	Mat out1, out2;
	imageMosaicProcess(image1, image2, out1, out2, image3, detection, mapping);
	isImage3Exist = true;
	showPicture(1, out1);
	showPicture(2, out2);
	showPicture(3, image3);
}

//目标检测
void CdamoDlg::targetDetection(){
	CComboBox* cmb_detection = ((CComboBox*)GetDlgItem(IDC_COMBO_DETECTION));
	int detection = cmb_detection->GetCurSel();
	CComboBox* cmb_mapping = ((CComboBox*)GetDlgItem(IDC_COMBO_MAPPING));
	int mapping = cmb_mapping->GetCurSel();
	/*
		detection 是特征检测方法 0是SIFT，1是SURF（两个都要实现）
		mapping   是特征点匹配方法 0-3分别是BruteForce， FLANN， RANSAC，Hough Cluster（至少实现其二）
	
		可以用isImage1Exist、isImage2Exist来判断图片是否已经打开，避免传入空图
		空图的话可以用下面这个发出警报然后再return
		CString timeStr;
		timeStr.Format(_T("图片未打开"));
		AfxMessageBox(timeStr);
	*/
	//下面开始实现
	if (!isImage1Exist) {
		CString timeStr;
		timeStr.Format(_T("图片1未打开，请先打开上图"));
		AfxMessageBox(timeStr);
		return;
	}

	if (!isImage2Exist) {
		CString timeStr;
		timeStr.Format(_T("图片2未打开，请先打开下图"));
		AfxMessageBox(timeStr);
		return;
	}

	Mat out1, out2;
	targetDetectionProcess(image1, image2, out1, out2, image3, detection, mapping);
	isImage3Exist = true;
	showPicture(1, out1);
	showPicture(2, out2);
	showPicture(3, image3);
}

//视频的实时特征检测与可视化
void CdamoDlg::videoFeature(){
}
