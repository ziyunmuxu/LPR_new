
// lprDlg.h : 头文件
//

#pragma once
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>
#include "CvvImage.h"

// ClprDlg 对话框
class ClprDlg : public CDialogEx
{
// 构造
public:
	ClprDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_LPR_DIALOG };

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
	afx_msg void OnBnClickedButton1();
	void ShowImage(IplImage* img, UINT ID);

//关于原图像的两个指针TheImage 代表原图；Src代表原图的单色图
private:
	IplImage* TheImage;
	IplImage* Src;

	int rowStart;
	int rowEnd;
	int colStart;
	int colEnd;

	int downboard;
	int upboard;
public:
	afx_msg void OnBnClickedButton2();


protected:
	void LprMain(IplImage *ImageSrc);
	IplImage* ImPreProcessing(IplImage* GrayImage);
	//灰度拉伸函数
	void ImageAdjust(IplImage *img_in,IplImage *img_out,double low_in, double high_in,double low_out, double high_out);

	//车牌定位
	void LpLocation(IplImage *img_in);

	//图像校正，返回图像需要旋转的角度
	double LineFitting(IplImage* img);
	double PolyFit(int* x, int* y, int count);

	//图像旋转，返回已经旋转的车牌
	IplImage *ImageRotate(IplImage *src, double angle);

	//对扣出来的车牌进行确定字符的上下边界
	void UpDownBoard(IplImage* RotatedImage, int times);


	//连通域分割
	void ConnectRegion(const IplImage*, int* , int*);

	//基于颜色和纹理特征的车牌定位
	IplImage* LpLocation1(const IplImage *img_in);

	bool LpBoard(const IplImage* img_in);
};
