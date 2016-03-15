
// lprDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "lpr.h"
#include "lprDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <vector>

#define PI 3.1415926

//在连通域标记中使用
#define     NO_OBJECT       0  
//MIN在opencv中有定义，不需要重新定义
//#define     MIN(x, y)       (((x) < (y)) ? (x) : (y))  
#define     ELEM(img, r, c) (CV_IMAGE_ELEM(img, unsigned char, r, c))  
#define     ONETWO(L, r, c, col) (L[(r) * (col) + c]) 

//使用cout
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


char separatepathName[7];

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


// ClprDlg 对话框




ClprDlg::ClprDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ClprDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void ClprDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ClprDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &ClprDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &ClprDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// ClprDlg 消息处理程序

BOOL ClprDlg::OnInitDialog()
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

	// TODO: 在此添加额外的初始化代码
	//这个代码是在MFC中使用控制台窗口
	/*AllocConsole();
	freopen("CON", "r", stdin );
	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);*/

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void ClprDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void ClprDlg::OnPaint()
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
HCURSOR ClprDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//用来选择图片，并在图像编辑框中显示

void ClprDlg::OnBnClickedButton1()
{
	 //TODO: 在此添加控件通知处理程序代码
	CString m_strPicPath;  
	//弹出选择图片对话框  
	CFileDialog dlg(true,_T("*.bmp"),NULL,OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,_T("image file(*.bmp;*.jpg)|*.bmp;*.jpg|All Files(*.*)|*.*|"),NULL);  
	dlg.m_ofn.lpstrTitle = _T("Open Image");// 打开文件对话框的标题名  
	if( dlg.DoModal() != IDOK )        // 判断是否获得视频  
		return;  
	m_strPicPath = dlg.GetPathName();           //获取图片路径  
	m_strPicPath.Replace(_T("//"),_T("////"));  
  
	Src=cvLoadImage((CT2CA)m_strPicPath,1); //读取彩色图  
	//Src = cvLoadImage((CT2CA)m_strPicPath,0);        //读取灰度图片  
  
  
	if (TheImage != NULL)  
	{  
		ShowImage(Src, IDC_STATIC);  
		//ShowImage(Src, IDC_STATIC);
	} 


	//源程序
	//CFileDialog dlg(TRUE, NULL, NULL, NULL,NULL,this);
	//CString strFileName;//记录选择文件路径
	//if (!dlg.DoModal() == IDOK) return;
	//strFileName = dlg.GetPathName();

	////IplImage *image=NULL; //原始图像
	//if(Src) cvReleaseImage(&Src);
	//Src = cvLoadImage(strFileName); //显示图片

}

void ClprDlg::ShowImage(IplImage* img, UINT ID) //在picture控件中显示图片的代码  
{  
  CDC* pDC=GetDlgItem(ID)->GetDC();  
  HDC hDC=pDC->GetSafeHdc();  
  CRect rect;  
  GetDlgItem(ID)->GetClientRect(&rect);  
  SetRect(rect,rect.left,rect.top,rect.right,rect.bottom);  
  CvvImage cimg;  
  cimg.CopyOf( img ); // 复制图片  
  cimg.DrawToHDC( hDC, &rect );  // 将图片绘制到显示控件的指定区域内  
  ReleaseDC( pDC );  
} 


//开始识别传过来的单色图
void ClprDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	LprMain(this->Src);
}


//车牌识别的主程序
//void ClprDlg::LprMain()
void ClprDlg::LprMain(IplImage *ImageSrc)
{
	IplImage *GrayImage =NULL;
	GrayImage = cvCreateImage(cvSize(ImageSrc->width,ImageSrc->height),IPL_DEPTH_8U,1);
	//把彩色图像转换为灰度图像
	cvCvtColor(ImageSrc,GrayImage,CV_RGB2GRAY);


	IplImage* PrePrecessingImg = ImPreProcessing(GrayImage);
	
	LpLocation(PrePrecessingImg);

	IplImage* result = NULL;
	result = LpLocation1(ImageSrc);
	cvSaveImage("颜色特征.bmp",result);

	LpBoard(result);


	//定义ROI
	CvSize size = cvSize(colEnd-colStart,rowEnd-rowStart);
	cvSetImageROI(GrayImage,cvRect(colStart,rowStart,size.width,size.height));//设置源图像ROI
	CvRect roi = cvGetImageROI(GrayImage);
	IplImage* pDest = cvCreateImage(size,GrayImage->depth,GrayImage->nChannels);//创建目标图像

	cvCopy(GrayImage,pDest); //复制图像
	cvResetImageROI(GrayImage);//源图像用完后，清空ROI

	cvShowImage("RIO.bmp",pDest);

	//现在的图像为pDest，为一个粗定位的图片,是一个黑白图像
	IplImage* OtsuOrienImage = cvCreateImage(cvGetSize(pDest),pDest->depth,pDest->nChannels);
	cvThreshold(pDest,OtsuOrienImage,0,255,CV_THRESH_OTSU);
	cvShowImage("OtsuOrientImage",OtsuOrienImage);
	cvSaveImage("二值化之后的图像.bmp",OtsuOrienImage);



	//对车牌进行旋转，其中angle
	double angle = 0;
	IplImage* RotateImage_tmp = cvCreateImage(cvGetSize(pDest),pDest->depth,pDest->nChannels);
	angle = LineFitting(OtsuOrienImage);


	//对得到的角度分别对原来的黑白图和得到的二值化图像进行旋转校正
	//具体校正函数未知
	//个人觉得应该是对原来的彩图进行校正之后，然后再生成二值化图像，然后在对图像确定上下边界
	//源程序的做法是：将之前的二值化图像进行旋转，然后对旋转的图像又进行二值化，通过二值化之后再确定上下边界
	//rotated_localBW为目标车牌的黑白图像，rotated_Otsu为目标车牌图像的二值化结果
	IplImage* rotated_localBW = cvCreateImage(cvGetSize(pDest),pDest->depth, pDest->nChannels);
	rotated_localBW = ImageRotate(pDest,angle);
	IplImage* rotated_Otsu = cvCreateImage(cvGetSize(rotated_localBW),rotated_localBW->depth,rotated_localBW->nChannels);
	cvThreshold(rotated_localBW,rotated_Otsu,0,255,CV_THRESH_OTSU);

	cvSaveImage("rotate_local.bmp",rotated_localBW);
	cvSaveImage("rotate_otsu.bmp",rotated_Otsu);


	//确定上下边界
	UpDownBoard(rotated_Otsu,8);

	//确定上下边界之后的原图和二值化图像
	//首先通过RIO在原图上扣除确定上下边界的那一部分图上，然后在根据二值化得到结果
	//得到车牌的上下边界原图rotated_updown_locBW和二值化之后的图rotated_updown_Otsu
	CvSize size1 = cvSize(rotated_localBW->width,downboard - upboard);
	cvSetImageROI(rotated_localBW,cvRect(0,upboard,size1.width,size1.height));
	IplImage* rotated_updown_locBW = cvCreateImage(size1,rotated_localBW->depth,rotated_localBW->nChannels);
	cvCopy(rotated_localBW,rotated_updown_locBW);
	cvResetImageROI(rotated_localBW);
	//cvShowImage("上下边界车牌",rotated_updown_locBW);

	IplImage* rotated_updown_Otsu = cvCreateImage(cvGetSize(rotated_updown_locBW),rotated_updown_locBW->depth,rotated_updown_locBW->nChannels);
	cvThreshold(rotated_updown_locBW,rotated_updown_Otsu,0,255,CV_THRESH_OTSU);
	cvSaveImage("上下边界二值化.bmp",rotated_updown_Otsu);


	//连通域分割
	int numbers = 0;
	int resultpos[20] = {0};
	ConnectRegion(rotated_updown_Otsu,&numbers,resultpos);

	for(int i=0;i<numbers;i++)
	{
		int leftborder=resultpos[2*i];
		int rightborder=resultpos[2*i+1];
		if(resultpos[2*i+1]-resultpos[2*i]<rotated_updown_Otsu->height/2)
		{
			leftborder=(resultpos[2*i]+resultpos[2*i+1])/2-rotated_updown_Otsu->height/4;
			rightborder=(resultpos[2*i]+resultpos[2*i+1])/2+rotated_updown_Otsu->height/4;
		}
		if(leftborder>resultpos[2*i])leftborder=resultpos[2*i];
		if(rightborder<resultpos[2*i+1])rightborder=resultpos[2*i+1];
		if(leftborder<0)leftborder=0;
		if(rightborder>rotated_updown_Otsu->width-1)rightborder=rotated_updown_Otsu->width-1;
		size= cvSize(rightborder-leftborder,rotated_updown_Otsu->height);//区域大小
		cvSetImageROI(rotated_updown_Otsu,cvRect(leftborder,0,size.width,size.height));//设置源图像ROI
		roi = cvGetImageROI(rotated_updown_Otsu);
		pDest = cvCreateImage(size,rotated_updown_Otsu->depth,rotated_updown_Otsu->nChannels);//创建目标图像
		cvCopy(rotated_updown_Otsu,pDest); //复制图像
		IplImage *pDest1=cvCreateImage(cvSize(24,48),IPL_DEPTH_8U ,1);
		cvResize(pDest,pDest1,CV_INTER_LINEAR);
		CString str="fenge",str1;
		str1.Format("%d",i);
		str+=str1;
		cvShowImage(str, pDest1);

		sprintf(separatepathName,"cvpic\\sp_%d.jpg",i);
		cvSaveImage(separatepathName,pDest1);		
		if(i==7)
			break;
		//IdenResult = RecogniseChar(pDest1,i);
		//cvResetImageROI(GrayImage);//源图像用完后，清空ROI

	}





	//cvReleaseImage(&PrePrecessingImg);
	cvReleaseImage(&pDest);
	cvReleaseImage(&OtsuOrienImage);

}

//图像预处理进程，输入为单色图，输出为处理后的指针
IplImage* ClprDlg::ImPreProcessing(IplImage* GrayImage)
{
	//对原图进行9*9的开运算，运算之后的结果和原图进行相见
	IplImage* temp = cvCreateImage(cvGetSize(GrayImage),IPL_DEPTH_8U,1);//用来存放相减的结果，也用来当做开运算中缓存的量
	IplImage* Egray = cvCreateImage(cvGetSize(GrayImage),IPL_DEPTH_8U,1);//用来存放开运算的结果
	IplConvKernel* strel = cvCreateStructuringElementEx(9,9,4,4,CV_SHAPE_RECT);//用来开运算的模板，其中大小为9*9，锚的位置为(4,4)位于正方形的中间

	//开运算
	//?开运算中几个参数的意义，注意里面需要temp来进行缓存
	cvMorphologyEx(GrayImage,Egray,temp,strel,CV_MOP_OPEN,1);
	//cvShowImage("b1.bmp",Egray);

	//灰度相减
	//cvSub中如果相减出现负数怎么处理，应该全部为0
	//如果使用开运算的结果减去原来的单色图，得到的图像全部为0
	//cvSub(Egray,GrayImage,temp);
	cvSub(GrayImage,Egray,temp,NULL);
	//cvShowImage("b2.bmp",temp);
	
	//灰度拉伸
	//由于整个图像太暗，需要对图像进行拉伸
	//灰度拉伸的函数是由自己写的
	IplImage* AdjustImage = cvCreateImage(cvGetSize(GrayImage),IPL_DEPTH_8U,1);
	ImageAdjust(temp,AdjustImage,0.25,0.8,0,1);
	//cvShowImage("b3.bmp",AdjustImage);


	////otsu二值化
	//对拉伸完的图像进行二值化处理，方便后面进行边缘判断
	IplImage* otsuImage = cvCreateImage(cvGetSize(GrayImage),IPL_DEPTH_8U, 1);
	cvThreshold(AdjustImage,otsuImage,0,255,CV_THRESH_OTSU);
	//cvShowImage("二值化.bmp",otsuImage);

	//？进行瞎鸡巴多的开闭运算，原因未知
	//进行模板为23*7 闭运算
	//闭运算是先进行膨胀后腐蚀，是对白色区域进行；其实使用一个矩形核消除车牌中的黑色区域，显示出车牌的位置 
	IplImage* bg1 = cvCreateImage(cvGetSize(GrayImage),IPL_DEPTH_8U,1);
	strel = cvCreateStructuringElementEx(23,7,11,3,CV_SHAPE_RECT);
	cvMorphologyEx(otsuImage,bg1,temp,strel,CV_MOP_CLOSE);
	//cvShowImage("二值化之后闭运算.bmp",bg1);

	//进行模板为25*7的开运算
	//开运算是是先进行腐蚀后膨胀，消除图像中那些小面积的白色部分
	IplImage* bg2 = cvCreateImage(cvGetSize(GrayImage),IPL_DEPTH_8U,1);
	strel = cvCreateStructuringElementEx(25,7,11,3,CV_SHAPE_RECT);
	cvMorphologyEx(bg1,bg2,temp,strel,CV_MOP_OPEN);
	//cvShowImage("开运算图像.bmp",bg2);

	//进行模板为9*1的开运算
	//消除图像中那些横着的细小白色条纹
	IplImage* bg3 = cvCreateImage(cvGetSize(GrayImage),IPL_DEPTH_8U,1);
	strel = cvCreateStructuringElementEx(1,9,0,4,CV_SHAPE_RECT);
	cvMorphologyEx(bg2,bg3,temp,strel,CV_MOP_OPEN);
	//cvShowImage("二闭运算.bmp",bg3);
	cvSaveImage("二闭运算.bmp",bg3);

	cvReleaseImage(&temp);
	cvReleaseImage(&Egray);
	cvReleaseImage(&AdjustImage);
	cvReleaseImage(&bg1);
	cvReleaseImage(&bg2);

	//其中关于cvCreateImage的内存分配问题？
	return bg3;
}

//源程序
//IplImage *ClprDlg::ImPreProcessing(IplImage *GrayImage)
//{
//	IplImage * temp = cvCreateImage(cvGetSize(GrayImage), 8,1);	
//	IplImage *Egray = cvCreateImage(cvSize(GrayImage->width,GrayImage->height),IPL_DEPTH_8U,1);	//开运算后的结果  的存放数组
//	IplConvKernel *strel=cvCreateStructuringElementEx(9,9,4,4,CV_SHAPE_RECT);	//开运算模板 9*9
//	//开运算
//	cvMorphologyEx(GrayImage,Egray,temp,strel,CV_MOP_OPEN,1);
//	cvShowImage("b1.bmp",Egray);
//	//灰度相减
//	cvSub(GrayImage,Egray,temp,NULL);
//	cvShowImage("b2.bmp",temp);
//
//	//灰度拉伸
//	IplImage *AdjustImage =cvCreateImage(cvGetSize(GrayImage), 8,1);
//	ImageAdjust(temp,AdjustImage,0.25,0.8,0,1);
//	cvShowImage("b3.bmp",AdjustImage);
//	//otsu二值化
//	IplImage *OtsuImage =cvCreateImage(cvGetSize(GrayImage), 8,1);
//	cvThreshold(AdjustImage, OtsuImage, 0, 255 , CV_THRESH_OTSU); //
//	cvShowImage("OtsuImage", OtsuImage); 
//	//闭运算
//	IplImage *bg1 = cvCreateImage(cvSize(GrayImage->width,GrayImage->height),IPL_DEPTH_8U,1);	//开运算后的结果
//	strel=cvCreateStructuringElementEx(23,7,11,3,CV_SHAPE_RECT);	//闭运算模板 9*25
//	cvMorphologyEx(OtsuImage,bg1,temp,strel,CV_MOP_CLOSE,1);
//	cvShowImage("bg1", bg1);
//	//开运算
//	IplImage *bg2 = cvCreateImage(cvSize(GrayImage->width,GrayImage->height),IPL_DEPTH_8U,1);	//开运算后的结果
//	strel=cvCreateStructuringElementEx(25,7,11,3,CV_SHAPE_RECT);	//开运算模板 9*27
//	cvMorphologyEx(bg1,bg2,temp,strel,CV_MOP_OPEN,1);
//	cvShowImage("bg2", bg2);
//	//开运算
//	IplImage *bg3 = cvCreateImage(cvSize(GrayImage->width,GrayImage->height),IPL_DEPTH_8U,1);	//开运算后的结果
//	strel=cvCreateStructuringElementEx(1,9,0,4,CV_SHAPE_RECT);	//开运算模板 9*1
//	cvMorphologyEx(bg2,bg3,temp,strel,CV_MOP_OPEN,1);
//
//	cvShowImage("bg3", bg3);
//
//	cvReleaseImage(&temp);
//	cvReleaseImage(&Egray);
//	cvReleaseImage(&AdjustImage);
//	cvReleaseImage(&OtsuImage);
//	cvReleaseImage(&bg1);
//	cvReleaseImage(&bg2);
//
//	return bg3;
//}


/****************************
***函数功能 ：			图像灰度拉伸
***输入		：img_in	待校正图像指针
：low_in	输入图像归一化灰度最小值
：high_in	输入图像归一化灰度最大值
：low_out	输出图像归一化灰度最小值
：high_out	输出图像归一化灰度最大值
***输出		：img_out	灰度拉伸后图像指针
***返回值	：			需校正的角度
*****************************/
void ClprDlg::ImageAdjust(IplImage *img_in,IplImage *img_out,double low_in, double high_in,double low_out, double high_out)
{
	double low = low_in * 255;
	double high = high_in * 255;
	double button = low_out * 255;
	double top = high_out * 255;
	double rate = (top - button)/(high - low);

	int x = 0;
	int y = 0;
	double value0 = 0;

	//一定需要注意的是怎么遍历整个图像数组
	for (y = 0; y < img_in->height; y++)
	{
		for (x = 0; x < img_in->width; x++)
		{
			value0 = ((uchar*)(img_in->imageData + img_in->widthStep*y))[x*img_in->nChannels];
			value0 = rate * (value0 - low) + button;
			if (value0 < 0) value0 = 0;
			if (value0 > 255) value0 = 255;
			((uchar*)(img_out->imageData + img_out->widthStep*y))[x*img_out->nChannels] = value0;
		}
	}
}
//源程序
//void ClprDlg::ImageAdjust(IplImage *img_in,IplImage *img_out,double low_in, double high_in,double low_out, double high_out)
//{
//	double low = low_in*255;  
//	double high = high_in*255;  
//	double bottom = low_out*255;  
//	double top = high_out*255;  
//	double scope_in = high - low;  
//	double scope_out = top - bottom;  
//
//	int x,y;  
//	double val0/*,val1,val2*/;  
//
//	// intensity transform  
//	for( y = 0; y < img_in->height; y++) {  
//		for (x = 0; x < img_in->width; x++){  
//			val0 = ((uchar*)(img_in->imageData+img_in->widthStep*y))[x*img_in->nChannels];   
//			val0 = pow((val0 - low)/scope_in,1)*scope_out+bottom;  
//			if(val0>255) val0=255;   
//			if(val0<0) val0=0;   
//			((uchar*)(img_out->imageData+img_out->widthStep*y))[x*img_in->nChannels]=(uchar)val0;  
//		}  
//	}  
//}

//车牌定位程序
//输入：经过图像学处理后的图片
//输出：在类中的变量，开始和结束的行列
//0307其中读取图像的时候位置有问题(左上角才为0,0)
void ClprDlg::LpLocation(IplImage *img_in)
{
	int row = img_in->height;
	int col = img_in->width;

	int* horiLine = new int[col];
	int* verLine = new int[row];
	/*int horiLine[400];
	int verLine[400];*/
	
	memset(horiLine,0,sizeof(int)*col);
	memset(verLine,0,sizeof(int)*row);
	int value0;
	//水平投影和垂直投影
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			value0 = ((uchar*)(img_in->imageData + img_in->widthStep*i))[j*img_in->nChannels];
			if (value0 == 255)
			{
				verLine[i]++;
				horiLine[j]++;
			}
		}
	}

	//找到水平和垂直的开始和结束位置
	int rowStartTmp;
	int rowEndTmp;
	int colStartTmp;
	int colEndTmp;

	for (int i = row -1; i > 0; i--)
	{
		if (verLine[i] && verLine[i-1] && verLine[i-2] && verLine[i-3] && verLine[i-10] && verLine[i-15] )
		{
			rowEndTmp = i;
			break;
		}
	}

	for (int i = rowEndTmp; i > rowEndTmp - 60; i--)
	{
		if(!verLine[i])
		{
			rowStartTmp = i;
			break;
		}
	}

	/////////////////////////////
	for (int i = 0; i < col; i++)
	{
		if (horiLine[i]  && horiLine[i+10] && horiLine[i+20] && horiLine[i+30] && horiLine[i+40] )
		{
			colStartTmp = i;
			break;
		}
	}

	for (int i = colStartTmp + 1; i < colStartTmp + 200; i++)
	{
		if(!horiLine[i])
		{
			colEndTmp = i;
			break;
		}
	}

	//由于后面也需要抠图所以需要增大区域
	rowStart = rowStartTmp - 0.15*(rowEndTmp - rowStartTmp);
	rowEnd = rowEndTmp + 0.2*(rowEndTmp - rowStartTmp);
	colStart = colStartTmp - 0.1*(colEndTmp - colStartTmp);
	colEnd = colEndTmp+0.1*(colEndTmp - colStartTmp);
	/*rowStart = rowStartTmp;
	rowEnd = rowEndTmp;
	colStart = colStartTmp;
	colEnd = colEndTmp;*/

	if(rowStart < 0)
		rowStart = 0;
	if(colStart < 0)
		colStart = 0;

	delete []horiLine;
	delete []verLine;
}
//源程序
//void ClprDlg::LpLocation(IplImage *img)
//{
//	int rows = img->height;
//	int cols = img->width;
//	int *rowtmp = new int[rows];
//	int *coltmp = new int[cols];
//	int grayval;
//	int startlinetmp=0;
//	int endlinetmp=0;
//	int startcoltmp=0;
//	int endcoltmp=0;
//
//	//水平投影
//	for(int y = 0; y < rows; y++) 
//	{
//		rowtmp[y]=0;
//		for (int x = 0; x < cols; x++)
//		{  
//			grayval = ((uchar*)(img->imageData+img->widthStep*y))[x*img->nChannels];   
//			if(grayval==255) rowtmp[y]++;    
//		}  
//	}  
//
//	for(int i=rows-1;i>0;i--)
//	{
//		if(rowtmp[i]&&rowtmp[i-1]&&rowtmp[i-2]&&rowtmp[i-3]&&rowtmp[i-10]&&rowtmp[i-15])
//		{
//			startlinetmp = i;
//			break;
//		}	
//	}
//
//	for(int i=startlinetmp;i>startlinetmp-60;i--)
//	{
//		if(!rowtmp[i])
//		{
//			endlinetmp = i;
//			break;
//		}	
//	}
//
//	//垂直投影
//	for (int x = 0; x < cols; x++) 
//	{  
//		coltmp[x] = 0;
//		for(int y = endlinetmp; y < startlinetmp; y++)
//		{  
//			grayval = ((uchar*)(img->imageData+img->widthStep*y))[x*img->nChannels];   
//			if(grayval==255) coltmp[x]++;    
//		}  
//	}  
//
//	for(int i=0;i<cols-1;i++)
//	{
//		if(coltmp[i]&&coltmp[i+10]&&coltmp[i+20]&&coltmp[i+30]&&coltmp[i+40])
//		{
//			startcoltmp = i;
//			break;
//		}	
//	}
//
//	for(int i=startcoltmp+1;i<startcoltmp+200;i++)
//	{
//		if(!coltmp[i])
//		{
//			endcoltmp = i;
//			break;
//		}	
//	}
//
//	rowEnd=startlinetmp+0.2*(startlinetmp-endlinetmp);
//	rowStart=endlinetmp-0.15*(startlinetmp-endlinetmp);
//	colStart=startcoltmp-0.1*(endcoltmp-startcoltmp);
//	colEnd=endcoltmp+0.1*(endcoltmp-startcoltmp);
//
//	delete []rowtmp;
//	delete []coltmp;
//}


//对图像进行角度校正，输出需要旋转的角度
double ClprDlg::LineFitting(IplImage* img)
{
	double angle1 = 0, angle2 = 0, angle = 0;
	int rows = img->height;
	int cols = img->width;
	int count = 0;
	int grayval;

	//下边缘拟合
	//首先统计下边缘的范围中有多少个点需要拟合
	for (int x = 0; x < cols; x++)
	{
		for (int y = rows-1; y >2*rows/3; y--)
		{
			grayval = ((uchar*)(img->imageData + img->widthStep*y))[x*img->nChannels];
			if(grayval == 255)
			{
				count++;
				break;
			}
		}
	}

	int* x1 = new int[count];
	int* y1 = new int[count];
	int k = 0;

	for (int i = 0; i < cols; i++)
	{
		for(int j = rows - 1; j>2*rows/3; j--)
		{
			grayval = ((uchar*)(img->imageData + img->widthStep*j))[i*img->nChannels];
			if (grayval == 255)
			{
				x1[k] = i;
				y1[k] = j;
				k++;
				break;
			}
		}
	}

	//这时候得到了下边缘的x和y的坐标,返回的值是下边缘相对于x轴的角度
	angle1 = PolyFit(x1,y1,count);

	//上边缘拟合
	count = 0;
	for (int x = 0; x < cols; x++)
	{
		for (int y = 0; y < 1*rows/3; y++)
		{
			grayval = ((uchar*)(img->imageData + img->widthStep*y))[x*img->nChannels];
			if(grayval == 255)
			{
				count++;
				break;
			}
		}
	}

	int* x2 = new int[count];
	int* y2 = new int[count];
	k = 0;

	for (int i = 0; i < cols; i++)
	{
		for(int j = 0; j < 1*rows/3; j++)
		{
			grayval = ((uchar*)(img->imageData + img->widthStep*j))[i*img->nChannels];
			if (grayval == 255)
			{
				x2[k] = i;
				y2[k] = j;
				k++;
				break;
			}
		}
	}

	//这时候得到了下边缘的x和y的坐标,返回的值是下边缘相对于x轴的角度
	angle2 = PolyFit(x2,y2,count);

	angle = (angle1 + angle2)/2;

	delete[] x1;
	delete[] y1;
	delete[] x2;
	delete[] y2;

	return angle;
}

//输入count个xy坐标，拟合成一条直线，并输出直线的角度
double ClprDlg::PolyFit(int* x, int* y, int count)
{
	double a=0,b=0,c=0,d=0;
	double k=0; //直线斜率

	for(int i = 0;i < count;i++)
	{
		a += x[i]*x[i];
		b += x[i];
		c += x[i]*y[i];
		d += y[i];
	}
	k = (count*c - b*d)/(count*a - b*b);
	//斜率反正切值，得到一个弧度，通过PI的计算得到最后的角度
	return atan(k)*180/PI;
}


//图像旋转
/****************************
***函数功能 ：			图像旋转
***输入		：src		待旋转的图像
			：angle		待拟旋转的角度
***返回值	：			旋转后的图像的指针
*****************************/
void bound(int x, int y, float ca, float sa, int *xmin, int *xmax, int *ymin, int *ymax)
{   
	int rx,ry;
	// 顺时针旋转
	rx = (int)floor(ca*(float)x+sa*(float)y);
	ry = (int)floor(-sa*(float)x+ca*(float)y);
	if (rx<*xmin) *xmin=rx; if (rx>*xmax) *xmax=rx;
	if (ry<*ymin) *ymin=ry; if (ry>*ymax) *ymax=ry;
}
IplImage *ClprDlg::ImageRotate(IplImage *src, double angle)
{
	double anglerad  = (CV_PI* (angle/180)) ;
	int newheight =int (fabs(( sin(anglerad)*src->width )) + fabs(( cos(anglerad)*src->height )) );
	int newwidth  =int (fabs(( sin(anglerad)*src->height)) + fabs(( cos(anglerad)*src->width)) );
	float m[6];            
	CvMat M = cvMat( 2, 3, CV_32F, m );
	int  nx,ny; 
	float  ca,sa;
	int  xmin,xmax,ymin,ymax,sx,sy;
	ca = (float)cos((double)(angle)*CV_PI/180.0);
	sa = (float)sin((double)(angle)*CV_PI/180.0);
	nx = src->width;
	ny=src->height;
	xmin = xmax = ymin = ymax = 0;
	bound(nx-1,0,ca,sa,&xmin,&xmax,&ymin,&ymax);
	bound(0,ny-1,ca,sa,&xmin,&xmax,&ymin,&ymax);
	bound(nx-1,ny-1,ca,sa,&xmin,&xmax,&ymin,&ymax);
	sx = xmax-xmin+1;
	sy = ymax-ymin+1;
	IplImage* newImage;
	newImage=cvCreateImage(cvSize(sx,sy),src->depth,src->nChannels);
	m[0] = ca;
	m[1] = sa;
	m[2] =-(float)xmin; 
	m[3] =-m[1];
	m[4] = m[0];
	m[5] =-(float)ymin;
	cvWarpAffine( src, newImage, &M,CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS,cvScalarAll(0) );

	return newImage;
}


//对抠出来的车牌确定车牌上下边界
//函数的设计思想是统计height上水平投影上穿越的次数，如果大于times的话就说明是边界
//经检验，出来的上下边界和实际上是一致的
void ClprDlg::UpDownBoard(IplImage* RotatedImage, int times)
{
	upboard = 0;
	downboard = RotatedImage->height - 1;

	int rows = RotatedImage->height;
	int cols = RotatedImage->width;

	//定义一个数组来存放水平投影的穿越特征
	int* thru = new int[rows];
	memset(thru,0,sizeof(int)*rows);

	//用两个值分别来保存相邻两个点的值
	uchar grayValue0 = 0;
	uchar grayValue1 = 0;

	for(int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols-1; j++)
		{
			grayValue0 = ((uchar*)(RotatedImage->imageData + i*(RotatedImage->widthStep)))[j*RotatedImage->nChannels];
			grayValue1 = ((uchar*)(RotatedImage->imageData + i*(RotatedImage->widthStep)))[(j+1	)*RotatedImage->nChannels];
			if (!grayValue0 && grayValue1)
			{
				thru[i]++;
			}
		}
	}

	//找到上边界开始的点
	for (int i = 0; i < rows-6; i++)
	{
		if ((thru[i] >= times)&&(thru[i+1] >= times)&&(thru[i+2] >= times)&&(thru[i+3] >= times)&&(thru[i+4] >= times)&&(thru[i+5] >= times)&&(thru[i+6] >= times))
		{
			//注意如果是上边界的话，直接取第一满足条件的点就可以了
			upboard = i;
			break;
		}
	}

	//找下边界
	for (int i = rows-1 ; i > 6; i--)
	{
		if ((thru[i] >= times)&&(thru[i-1] >= times)&&(thru[i-2] >= times)&&(thru[i-3] >= times)&&(thru[i-4] >= times)&&(thru[i-5] >= times)&&(thru[i-6] >= times))
		{
			//注意如果是下边界应该是i+1，方便以后我们size的使用
			downboard = i+1;
			break;
		}
	}

	delete[] thru;
}




//连通域标记
//代码来源http://blog.csdn.net/yysdsyl/article/details/5343075
int find( int set[], int x )  
{  
	int r = x;  
	while ( set[r] != r )  
		r = set[r];  
	return r;  
}
/* 
labeling scheme 
+-+-+-+ 
|D|C|E| 
+-+-+-+ 
|B|A| | 
+-+-+-+ 
| | | | 
+-+-+-+ 
A is the center pixel of a neighborhood.  In the 3 versions of 
connectedness: 
4:  A connects to B and C 
6:  A connects to B, C, and D 
8:  A connects to B, C, D, and E 
*/  
/////////////////////////////  
//by yysdsyl  
//  
//input:img     --  gray image  
//      n       --  n connectedness  
//      labels  --  label of each pixel, labels[row * col]  
//output:  number of connected regions  
//  
//
int bwlabel(const IplImage* img, int n, int* labels)  
{  
	if(n != 4 && n != 8)  
		n = 4;  
	int nr = img->height;  
	int nc = img->width;  
	int total = nr * nc;  
	// results  
	memset(labels, 0, total * sizeof(int));  
	int nobj = 0;                               // number of objects found in image  
	// other variables                               
	int* lset = new int[total];   // label table  
	memset(lset, 0, total * sizeof(int));  
	int ntable = 0;  
	for( int r = 0; r < nr; r++ )   
	{  
		for( int c = 0; c < nc; c++ )   
		{              
			if ( ELEM(img, r, c) )   // if A is an object  
			{                 
				// get the neighboring pixels B, C, D, and E  
				int B, C, D, E;  
				if ( c == 0 )   
					B = 0;   
				else   
					B = find( lset, ONETWO(labels, r, c - 1, nc) );  
				if ( r == 0 )   
					C = 0;   
				else   
					C = find( lset, ONETWO(labels, r - 1, c, nc) );  
				if ( r == 0 || c == 0 )   
					D = 0;   
				else   
					D = find( lset, ONETWO(labels, r - 1, c - 1, nc) );  
				if ( r == 0 || c == nc - 1 )   
					E = 0;  
				else   
					E = find( lset, ONETWO(labels, r - 1, c + 1, nc) );  
				if ( n == 4 )   
				{  
					// apply 4 connectedness  
					if ( B && C )   
					{        // B and C are labeled  
						if ( B == C )  
							ONETWO(labels, r, c, nc) = B;  
						else {  
							lset[C] = B;  
							ONETWO(labels, r, c, nc) = B;  
						}  
					}   
					else if ( B )             // B is object but C is not  
						ONETWO(labels, r, c, nc) = B;  
					else if ( C )               // C is object but B is not  
						ONETWO(labels, r, c, nc) = C;  
					else   
					{                      // B, C, D not object - new object  
						//   label and put into table  
						ntable++;  
						ONETWO(labels, r, c, nc) = lset[ ntable ] = ntable;  
					}  
				}   
				else if ( n == 6 )   
				{  
					// apply 6 connected ness  
					if ( D )                    // D object, copy label and move on  
						ONETWO(labels, r, c, nc) = D;  
					else if ( B && C )   
					{        // B and C are labeled  
						if ( B == C )  
							ONETWO(labels, r, c, nc) = B;  
						else   
						{  
							int tlabel = MIN(B,C);  
							lset[B] = tlabel;  
							lset[C] = tlabel;  
							ONETWO(labels, r, c, nc) = tlabel;  
						}  
					}   
					else if ( B )             // B is object but C is not  
						ONETWO(labels, r, c, nc) = B;  
					else if ( C )               // C is object but B is not  
						ONETWO(labels, r, c, nc) = C;  
					else   
					{                      // B, C, D not object - new object  
						//   label and put into table  
						ntable++;  
						ONETWO(labels, r, c, nc) = lset[ ntable ] = ntable;  
					}  
				}  
				else if ( n == 8 )   
				{  
					// apply 8 connectedness  
					if ( B || C || D || E )   
					{  
						int tlabel = B;  
						if ( B )   
							tlabel = B;  
						else if ( C )   
							tlabel = C;  
						else if ( D )   
							tlabel = D;  
						else if ( E )   
							tlabel = E;  
						ONETWO(labels, r, c, nc) = tlabel;  
						if ( B && B != tlabel )   
							lset[B] = tlabel;  
						if ( C && C != tlabel )   
							lset[C] = tlabel;  
						if ( D && D != tlabel )   
							lset[D] = tlabel;  
						if ( E && E != tlabel )   
							lset[E] = tlabel;  
					}   
					else   
					{  
						//   label and put into table  
						ntable++;  
						ONETWO(labels, r, c, nc) = lset[ ntable ] = ntable;  
					}  
				}  
			}   
			else   
			{  
				ONETWO(labels, r, c, nc) = NO_OBJECT;      // A is not an object so leave it  
			}  
		}  
	}  
	// consolidate component table  
	for( int i = 0; i <= ntable; i++ )  
		lset[i] = find( lset, i );                                                                                                   
	// run image through the look-up table  
	for( int r = 0; r < nr; r++ )  
		for( int c = 0; c < nc; c++ )  
			ONETWO(labels, r, c, nc) = lset[ ONETWO(labels, r, c, nc) ];  
	// count up the objects in the image  
	for( int i = 0; i <= ntable; i++ )  
		lset[i] = 0;  
	for( int r = 0; r < nr; r++ )  
		for( int c = 0; c < nc; c++ )  
			lset[ ONETWO(labels, r, c, nc) ]++;  
	// number the objects from 1 through n objects  
	nobj = 0;  
	lset[0] = 0;  
	for( int i = 1; i <= ntable; i++ )  
		if ( lset[i] > 0 )  
			lset[i] = ++nobj;  
	// run through the look-up table again  
	for( int r = 0; r < nr; r++ )  
		for( int c = 0; c < nc; c++ )  
			ONETWO(labels, r, c, nc) = lset[ ONETWO(labels, r, c, nc) ];  
	//  
	delete[] lset;  
	return nobj;  
} 


//删除连通域数组中的某个label，并使用心得label来替代
void removeLabel(int* LabelArray, int size, int oldval, int newval)
{
	for(int i = 0; i< size; i++)
	{
		if (LabelArray[i] == oldval)
		{
			LabelArray[i] = newval;
		}
	}
}

//求出某个连通域的左右边界
//输入为连通域数组，数组的长度，需要寻找的连通域k，boardArray为左右边界的数组
void LCboard(const int* LabelArray, int cols, int size, int k, int* boardArray)
{
	int lboard = cols - 1;
	int rboard = 0;
	int m;//临时保存的边界
	for(int i = 0; i < size; i++)
	{
		if (LabelArray[i] == k)
		{
			m = i % cols;
			lboard = MIN(lboard,m);
			rboard = MAX(rboard,m);
		}
	}
	if (lboard < rboard)
	{
		boardArray[2*k - 2] = lboard;
		boardArray[2*k - 1] = rboard;
	}
}


//合并包含关系的连通域，并且将连通域排序
//先排序后进行合并
void mergeBoard(int* labelBoard, int* labelBoard_sort, int labelNumbers, int* labelNumbers_sort)
{
	//使用选择排序，按照左边界的大小进行排序
	for (int i = 0; i < labelNumbers - 1; i++)
	{
		int k = i;
		int temp;
		int temp1;
		for(int j = i + 1; j < labelNumbers; j++)
		{
			if(labelBoard[2*j] < labelBoard[2*k])
			{
				k = j;
			}
		}
		if (k != i)
		{
			temp = labelBoard[2*i];
			temp1 = labelBoard[2*i+1];
			labelBoard[2*i] = labelBoard[2*k];
			labelBoard[2*i+1] = labelBoard[2*k+1];
			labelBoard[2*k] = temp;
			labelBoard[2*k+1] = temp1;
		}
	}

	//根据排序来删除包含关系，并将最后得到的边界放入新的左右边界数组中labelBoard_sort

	//使用一个数组来记录删除之后有效的边界位置
	int* flag = new int[labelNumbers];
	memset(flag, 0, sizeof(int)*labelNumbers);
	for (int i = 1; i < labelNumbers; i++)
	{
		if (labelBoard[2*i+1] < labelBoard[2*i-1])
		{
			flag[i-1] = 1;
		}
	}

	//根据flag标志创建一个新的labelBoard_sort
	int count = 0;
	for(int i = 0; i < labelNumbers; i++)
	{
		if (flag[i] == 0)
		{
			labelBoard_sort[2*count] = labelBoard[2*i];
			labelBoard_sort[2*count+1] = labelBoard[2*i+1];	
			count++;
		}
	}
	*labelNumbers_sort = count;
}

//连通域分割，图像输入二值化后的图片，输出numbers和存放分割之后的坐标
//对原图使用const，保证原图的数据不会被修改，以免各种复制
void ClprDlg::ConnectRegion(const IplImage* RotateImage2, int* numbers , int* resultpos)
{
	//定义一个一维数组来存放标记之后的结果，大小根据RotateImage2的大小来确定
	int cols = RotateImage2->width;
	int rows = RotateImage2->height;
	int* labelArray = new int[cols * rows];
	memset(labelArray, 0 , sizeof(int)*cols*rows);

	//用一个变量来保存连通域标记之后的返回值，连通域的个数
	int labelNumbers = 0;
	labelNumbers = bwlabel(RotateImage2,8,labelArray);

	//在控制台中打印出labelArray，验证函数
	/*for (int i = 0; i < rows; i++)
	{
		for(int j = 0; j < cols; j++)
		{
			cout << labelArray[i*cols + j];
		}
		cout << endl;
	}
	cout << "连通域的个数："<<labelNumbers;*/

	//求各个连通域的面积，并存放在areaRegion中
	int* LabelArea = new int[labelNumbers];
	memset(LabelArea,0,sizeof(int)*labelNumbers);
	for (int i = 0; i < cols*rows; i++)
	{
		int k = labelArray[i];
		if (k > 0)
		{
			LabelArea[k-1]++;
		}
	}

	//设定连通域面积的范围，根据高度来设定
	int lowArea = rows * rows /20;
	int highArea = rows * rows * 5 /12;
	//使用这个newLabel来记录重新排序后的label值
	int labelNumbers_tmp = labelNumbers;
	int* newLabel = new int[labelNumbers_tmp];
	for (int i = 0; i < labelNumbers_tmp; i++)
	{
		newLabel[i] = 1;
	}
	//对面积进行比较，删除不符合条件的面积
	for(int i = 0; i < labelNumbers_tmp; i++)
	{
		if (LabelArea[i] < lowArea || LabelArea[i] > highArea)
		{
			removeLabel(labelArray,rows*cols,i+1,0);
			labelNumbers--;
			newLabel[i] = 0;
		}
	}

	//重新排序得到新的label值
	//首先计算newLabel的值，重新的映射值
	int count_label = 0;
	for (int i = 0; i < labelNumbers_tmp; i++)
	{
		if (newLabel[i] != 0)
		{
			//这个计算方法是有错误的，由于你计算的时候如果用累加的话，会导致后面的数字全部错误
			/*for(int j = 0; j < i; j++)
			{
			newLabel[i] += newLabel[j];
			}
			cout << newLabel[i] << "  ";*/
			count_label++;
			newLabel[i] = count_label;
		}
	}

	//重新排序，得到新的labelArray
	for (int i = 0; i < cols*rows; i++)
	{
		int k = labelArray[i];
		if (k != 0)
		{
			labelArray[i] = newLabel[k-1];
		}
	}

	/*for (int i = 0; i < rows; i++)
	{
		for(int j = 0; j < cols; j++)
		{
			cout << labelArray[i*cols + j];
		}
		cout << endl;
	}
	cout << "连通域的个数："<<labelNumbers;*/


	//记录每一个连通域的宽度,连通域k的宽度位于2k-2和2k-1的位置
	int* labelBoard = new int[2*labelNumbers];
	memset(labelBoard, 0, sizeof(int)*2*labelNumbers);
	//int labelBoard[20] = {0};
	for(int i = 0; i < labelNumbers; i++)
	{
		LCboard(labelArray,cols,cols*rows,i+1,labelBoard);
	}

	//通过连通域的宽度，舍去过宽的连通域
	labelNumbers_tmp = labelNumbers;
	for(int i = 0; i < labelNumbers_tmp; i++)
	{
		if (labelBoard[2*i+1] - labelBoard[2*i] > rows*4/3)
		{
			if (i != labelNumbers_tmp-1)
			{
				labelBoard[2*i+1] = labelBoard[2*i+3];
				labelBoard[2*i] = labelBoard[2*i+2];
			}
			else
			{
				labelBoard[2*i+1] = labelBoard[2*i-1];
				labelBoard[2*i] = labelBoard[2*i-2];
			}
		}
	}
	int* labelBoard_sort = new int[labelNumbers_tmp];
	memset(labelBoard_sort,0,sizeof(int)*labelNumbers_tmp);
	//这时候labelNumbers的数值已经更新，同时labelBoard_sort也是更新好了的数组，虽然中间可能含有冗余的空间
	mergeBoard(labelBoard,labelBoard_sort,labelNumbers_tmp,&labelNumbers);

	//根据连通域的数据，对车牌的连通域进行划分（车牌有七个连通域）
	if (labelNumbers == 7)
		for (int i = 0; i < 2*labelNumbers; i++)
			resultpos[i] = labelBoard_sort[i];
	else
		if (labelNumbers == 6)
			if (resultpos[0] > rows)
			{
				resultpos[0] = 0;
				resultpos[1] = labelBoard_sort[0]-1;
				for (int i = 2; i < 2*labelNumbers; i++)
					resultpos[i] = labelBoard_sort[i-2];
			}
		else
			if (labelNumbers > 7)
			{
				for (int i = 2; i < 2*labelNumbers; i++)
					resultpos[i] = labelBoard_sort[i-2];
			}
	*numbers = 7;
}







IplImage* ClprDlg::LpLocation1(const IplImage *img_in)
{
	IplImage* HSV = NULL;  
	IplImage* HImg= NULL;
	IplImage* SImg= NULL;
	IplImage* VImg= NULL;
	IplImage* result = NULL;

	CvSize SrcSize = cvGetSize(img_in);
	HSV = cvCreateImage(SrcSize,8,3);
	HImg= cvCreateImage(SrcSize,8,1);  
	SImg= cvCreateImage(SrcSize,8,1);  
	VImg= cvCreateImage(SrcSize,8,1);  
	result= cvCreateImage(SrcSize,8,1);

	//将图像从RGB颜色空间转换到HSV空间
	cvCvtColor(img_in,HSV,CV_BGR2HSV);
	//将HSV分解为三张单通道图像，便于后面就行处理  
	cvSplit(HSV,HImg,SImg,VImg,NULL);

	int rows = SrcSize.height;
	int cols = SrcSize.width;
	int hvalue = 0;
	int svalue = 0;
	int vvalue = 0;


	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			hvalue = ((uchar*)(HImg->imageData + HImg->widthStep*i))[j*HImg->nChannels];
			svalue = ((uchar*)(SImg->imageData + SImg->widthStep*i))[j*SImg->nChannels];
			vvalue = ((uchar*)(VImg->imageData + VImg->widthStep*i))[j*VImg->nChannels];

		//	if ((hvalue>=95 && hvalue<=120 && svalue>=82 && svalue<=255 && vvalue>=89 && vvalue<=255) || (svalue>=0 && svalue<=25 && vvalue>=232 && vvalue<=255))
			if (hvalue>=95 && hvalue<=120 && svalue>=82 && svalue<=255 && vvalue>=89 && vvalue<=255)
				((uchar*)(result->imageData + result->widthStep*i))[j*result->nChannels] = 255;
			else
				((uchar*)(result->imageData + result->widthStep*i))[j*result->nChannels] = 0;	
		}
	}

	return result;
}

bool ClprDlg::LpBoard(const IplImage* img_in)
{
	int rows = img_in->height;
	int cols = img_in->width;

	int* hor_traver = new int[rows];
	memset(hor_traver,0,sizeof(int)*rows);
	int value0 = 0;
	int value1 = 0;

	//算出水平穿越特征的次数，存在hor_traver的数组中
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols-1; j++)
		{
			value0 = ((uchar*)(img_in->imageData + img_in->widthStep*i))[j*img_in->nChannels];
			value1 = ((uchar*)(img_in->imageData + img_in->widthStep*i))[(j+1)*img_in->nChannels];
			if (value0 != value1)
			{
				hor_traver[i]++;
			}
		}
	}

	//使用一个数组来存放判断这个点是不是满足条件的点，穿越特征大约15，并且接下来的4个之中有一半是满足条件的，我们可以将这个点定义为1，如果不满足任何一点条件，我们置位0
	//对于最后四个点我们只需要判断第一个条件

	//hor_judge中满足条件我们置位1，否则为0
	int* hor_judge = new int[rows];
	memset(hor_judge,0,sizeof(int)*rows);
	for (int i = 0; i < rows; i++)
	{
		if(hor_traver[i] >= 15)
			hor_judge[i] = 1;
	}
	
	//再对hor_judge处理，若这个点的值为1，并且后面的四个点的值和大于2，那么我们定义为1；整个数组最后的四个点不处理
	for (int i = 0; i < rows-4; i++)
	{
		if (hor_judge[i] && (hor_judge[i+1]+hor_judge[i+2]+hor_judge[i+3]+hor_judge[i+4] > 1))
			hor_judge[i] = 1;
		else
			hor_judge[i] = 0;
	}
	
	//寻找连续为1的边界，使用梯度来求值
	//我们使用vector来保存边界
	vector<int> board_may;
	int* hor_grade = new int[rows];
	memset(hor_grade,0,sizeof(int)*rows);
	for (int i = 1; i < rows; i++)
	{
		hor_grade[i] = hor_judge[i] - hor_judge[i-1];
	}

	//遍历梯度数组，将符合条件的梯度保存在向量中
	int k0 = 0;
	int k1 = 0;
	bool flag = false;
	int count = 0;
	for (int i = 0; i < rows-1; i++ )
	{
		if (hor_grade[i] == 1)
		{
			k0 = i;
			flag = true;
		}
		if (hor_grade[i] == -1 && flag && (i - k0 > 3))
		{
			board_may.push_back(k0-1);
			board_may.push_back(i+1);
			count++;
		}	
	}
	
	delete[] hor_traver;
	delete[] hor_judge;
	delete[] hor_grade;

	if (count)
		return true;
	else
		return false;
	
}