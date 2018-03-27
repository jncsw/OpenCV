//通用
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h> 
#include <vector> 
//C++
#include<iostream>
//分辨率
#include <Windows.h>
#include <stdio.h>
using namespace cv;
using namespace std;

//滚动条相关函数
double mx = 0, my = 0;
int dx = 0, dy = 0, horizBar_x = 0, vertiBar_y = 0;
bool clickVertiBar = 1, clickHorizBar = 1, needScroll = false;
CvRect rect_bar_horiz, rect_bar_verti;
void mouse_callback(int event, int x, int y, int flags, void* param)
{
	if (needScroll)
	{
		switch (event)
		{
		case CV_EVENT_LBUTTONDOWN:
			mx = x, my = y;
			dx = 0, dy = 0;
			// 按下左键时光标定位在水平滚动条区域内  
			if (x >= rect_bar_horiz.x && x <= rect_bar_horiz.x + rect_bar_horiz.width
				&& y >= rect_bar_horiz.y && y <= rect_bar_horiz.y + rect_bar_horiz.height)
			{
				clickHorizBar = true;
			}
			// 按下左键时光标定位在垂直滚动条区域内  
			if (x >= rect_bar_verti.x && x <= rect_bar_verti.x + rect_bar_verti.width
				&& y >= rect_bar_verti.y && y <= rect_bar_verti.y + rect_bar_verti.height)
			{
				clickVertiBar = true;
			}
			break;
		case CV_EVENT_MOUSEMOVE:
			if (clickHorizBar)
			{
				dx = fabs(x - mx) > 1 ? (int)(x - mx) : 0;
				dy = 0;
			}
			if (clickVertiBar)
			{
				dx = 0;
				dy = fabs(y - my) > 1 ? (int)(y - my) : 0;
			}
			mx = x, my = y;
			break;
		case CV_EVENT_LBUTTONUP:
			mx = x, my = y;
			dx = 0, dy = 0;
			clickHorizBar = false;
			clickVertiBar = false;
			break;
		default:
			dx = 0, dy = 0;
			break;
		}
	}
}

void myShowImageScroll(char* title, IplImage* src_img,
	int winWidth = 1400, int winHeight = 700) // 显示窗口大小默认为 1400×700  
{
	IplImage* dst_img;
	CvRect  rect_dst,   // 窗口中有效的图像显示区域  
		rect_src;   // 窗口图像对应于源图像中的区域  
	int imgWidth = src_img->width,
		imgHeight = src_img->height,
		barWidth = 25;  // 滚动条的宽度（像素）  
	double  scale_w = (double)imgWidth / (double)winWidth,    // 源图像与窗口的宽度比值  用以判断是否超出显示范围
		scale_h = (double)imgHeight / (double)winHeight;            // 源图像与窗口的高度比值  用以判断是否超出显示范围

	if (scale_w<1)                                                                     //如果小于1 说明原图比窗口小，窗口的宽度将重新赋值
		winWidth = imgWidth + barWidth;
	if (scale_h<1)                                                                     //如果小于1 说明原图比窗口小，窗口的高度将重新赋值
		winHeight = imgHeight + barWidth;

	int showWidth = winWidth, showHeight = winHeight; // 窗口中有效的图像显示区域的宽和高  
	int src_x = 0, src_y = 0;                                                      // 源图像中 rect_src 的左上角位置  
	int horizBar_width = 0, horizBar_height = 0,                   //定义并初始化垂直于水平滑块的宽高
		vertiBar_width = 0, vertiBar_height = 0;

	needScroll = scale_w>1.0 || scale_h>1.0 ? true : false;
	// 若图像大于设定的窗口大小，则显示滚动条  
	if (needScroll)
	{
		dst_img = cvCreateImage(cvSize(winWidth, winHeight), src_img->depth, src_img->nChannels);
		cvZero(dst_img);
		// 源图像宽度大于窗口宽度，则显示水平滚动条  
		if (scale_w > 1.0)  //宽度超出了
		{
			showHeight = winHeight - barWidth;
			horizBar_width = (int)((double)winWidth / scale_w);
			horizBar_height = winHeight - showHeight;
			horizBar_x = min(
				max(0, horizBar_x + dx),
				winWidth - horizBar_width);
			rect_bar_horiz = cvRect(
				horizBar_x,
				showHeight + 1,
				horizBar_width,
				horizBar_height);
			// 显示水平滚动条  
			cvRectangleR(dst_img, rect_bar_horiz, cvScalarAll(255), -1);
		}
		// 源图像高度大于窗口高度，则显示垂直滚动条  
		if (scale_h > 1.0)  //高度超出了
		{
			showWidth = winWidth - barWidth;
			vertiBar_width = winWidth - showWidth;
			vertiBar_height = (int)((double)winHeight / scale_h);
			vertiBar_y = min(
				max(0, vertiBar_y + dy),
				winHeight - vertiBar_height);
			rect_bar_verti = cvRect(
				showWidth + 1,
				vertiBar_y,
				vertiBar_width,
				vertiBar_height);   //确定垂直滚动条的白色部分的大小
									// 显示垂直滚动条  
			cvRectangleR(dst_img, rect_bar_verti, cvScalarAll(255), -1);
		}

		showWidth = min(showWidth, imgWidth);
		showHeight = min(showHeight, imgHeight);
		// 设置窗口显示区的 ROI  
		rect_dst = cvRect(0, 0, showWidth, showHeight);
		cvSetImageROI(dst_img, rect_dst);
		// 设置源图像的 ROI  
		src_x = (int)((double)horizBar_x*scale_w);
		src_y = (int)((double)vertiBar_y*scale_h);
		src_x = min(src_x, imgWidth - showWidth);
		src_y = min(src_y, imgHeight - showHeight);
		rect_src = cvRect(src_x, src_y, showWidth, showHeight);
		cvSetImageROI(src_img, rect_src);
		// 将源图像内容复制到窗口显示区  
		cvCopy(src_img, dst_img);

		cvResetImageROI(dst_img);
		cvResetImageROI(src_img);
		// 显示图像和滚动条  
		cvShowImage(title, dst_img);

		cvReleaseImage(&dst_img);
	}
	// 源图像小于设定窗口，则直接显示图像，无滚动条  
	else
	{
		cvShowImage(title, src_img);
	}
}



//双线性插值
double bilinear(double a, double b, double c, double d, double dx, double dy)
{
	double h1 = a + dx*(b - a); // = (1-dx)*a + dx*b
	double h2 = c + dx*(d - c);
	return h1 + dy*(h2 - h1);
}

//实验2.2：图像变形
void Rotate(const Mat &input, Mat &output) {
	double H = input.rows, W = input.cols;
	output.create(input.size(), CV_8UC3);
	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			double xx = ((double)x - 0.5*W) / (0.5*W);
			double yy = ((double)y - 0.5*H) / (0.5*H);

			double r = sqrt(xx*xx + yy*yy);
			//cout << "xx=" << xx << " yy=" << yy << endl;
			
			double tx, ty,rx,ry;
			if (r >= 1) {
				rx = xx;
				ry = yy;
			}
			else {
				double theta = (1 - r)*(1 - r);
				ry = sin(theta)*xx + cos(theta)*yy;
				rx = cos(theta)*xx - sin(theta)*yy;
			}
			tx = 0.5*W*rx + 0.5*W;
			ty = 0.5*H*ry + 0.5*H;

			if (tx > input.cols - 1)tx = tx - 1.0;
			if (ty > input.rows - 1)ty = ty - 1.0;
			
			uchar *om = output.ptr(y,x);
			//cout << tx << " " << ty << endl;
			const uchar *A = input.ptr(floor(ty), floor(tx));
			const uchar *B = input.ptr(floor(ty), ceil(tx));

			const uchar *C = input.ptr(ceil(ty), floor(tx));
			const uchar *D = input.ptr(ceil(ty), ceil(tx));
			for (int i = 0; i < 3; i++) {
				//om[i] = input.ptr(floor(ty), floor(tx))[i];
				om[i] = bilinear(A[i], B[i], C[i], D[i], tx - (int)tx, ty - (int)ty);
			}
			

		}
	}


}
//实验2.1：图像缩放
void Scale(const Mat &input, Mat &output, double sx, double sy) {
	int oy = input.rows*sy;
	int ox = input.cols*sx;
	if (ox < 1)ox = 1;
	if (oy < 1)oy = 1;
	cout << "缩放后高度：" << oy << " " << "宽度：" << ox << endl;
	output.create(oy, ox, CV_8UC3);
	for (int y = 0; y < oy; ++y) {
		for (int x = 0; x <ox; ++x) {
			double tx = x/sx;
			double ty = y/sy;
			//cout << "tx=" << tx << " ty=" << ty << endl;
			if (tx > input.cols - 1)tx=tx-1.0;
			if (ty > input.rows - 1)ty=ty-1.0;

			const uchar *A = input.ptr(floor(ty),floor(tx));
			const uchar *B = input.ptr(floor(ty),ceil(tx));

			const uchar *C = input.ptr(ceil(ty),floor(tx));
			const uchar *D = input.ptr(ceil(ty),ceil(tx));
			
			uchar *d = output.ptr(y, x);
			for (int i = 0; i < 3; i++) {
				
				d[i] = bilinear(A[i], B[i],C[i],D[i],tx-(int)tx,ty-(int)ty);
			}

		}
	}


}


int main()
{
	
	Mat img = imread("exp2.jpg", CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);//尽量保证无损
	cout << "**************实验二**************" << endl;
	double sx = 5;
	double sy = 5;
	//输出文件
	vector<int>compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_params.push_back(100);
	Mat output;
	while (1) {
		cout << "请输入宽度缩放倍数sx:" << endl;
		cin >> sx;
		cout << "请输入高度缩放倍数sy:" << endl;
		cin >> sy;

		imshow("实验二原图", img);
		
		Scale(img, output, sx, sy);

		/*namedWindow("缩放", WINDOW_AUTOSIZE);
		imshow("缩放", output);*/

		//namedWindow("缩放后", WINDOW_NORMAL);//允许图片拉伸
		//imshow("缩放后", output);

		IplImage iplimg = (IplImage)output;
		cvNamedWindow("缩放后", 1);
		cvSetMouseCallback("缩放后", mouse_callback);
		int nScreenWidth, nScreenHeight;
		nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
		while (1)
		{

			myShowImageScroll("缩放后", &iplimg, nScreenWidth, nScreenHeight*0.9);

			int KEY = cvWaitKey(10);
			if ((char)KEY == 27)//ESC键
				break;

		}
		cvDestroyWindow("缩放后");
		cout << "继续输入1，退出缩放功能输入其他任意字符" << endl;
		char c;
		getchar();
		c = getchar();
		if (c != '1')break;
	}
	imwrite("缩放后.jpg", output, compression_params);
	Mat rot;
	Rotate(img, rot);
	cout << "计算变形图像" << endl;
	imshow("变形后", rot);
	imwrite("变形后.jpg", rot, compression_params);

	waitKey();
	return 0;
}