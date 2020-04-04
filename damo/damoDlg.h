
// damoDlg.h: 头文件
//

#pragma once

#include<opencv2\opencv.hpp>
#include<opencv2/highgui/highgui_c.h>
using namespace cv;

// CdamoDlg 对话框
class CdamoDlg : public CDialogEx
{
// 构造
public:
	CdamoDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DAMO_DIALOG };
#endif

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
	CRect rect1;
	CRect rect2;
	CRect rect3;
	Mat image1;
	Mat image2;
	Mat image3;
	Mat block;
	bool isImage1Exist;
	bool isImage2Exist;
	bool isImage3Exist;
	void showBlock(int number);
	void showPicture(int number, Mat pic);
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonOperate();
	void verifyFERNS();
	void pointMapping();
	void geometricCorrection();
	void imageMosaic();
	void targetDetection();
	void videoFeature();
};
