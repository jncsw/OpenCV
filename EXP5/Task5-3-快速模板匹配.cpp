//通用
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h> 
#include <vector> 

#include "cvaux.h"
#include "cxcore.h"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "cv.h"       
//C++
#include<iostream>
#include<stdlib.h>
#include <stdio.h>
#include <time.h>
using namespace cv;
using namespace std;
typedef long long ll;
Mat img, output;

vector<Mat> channels;
int cH,cW,H,W,mH,mW,lH,lW;

//获得[a,b]的随机数
int getrand(int a, int b) {
	return (rand() % (b - a + 1)) + a;
}

void convolveDFT(Mat A, Mat B,Mat& C)
{
	//
	// 分配返回的Mat
	C.create(abs(A.rows - B.rows) + 1, abs(A.cols - B.cols) + 1, A.type());
	Size dftSize;
	// 计算DFT使用的大小，进行FFT加速
	dftSize.width = getOptimalDFTSize(A.cols + B.cols - 1);
	dftSize.height = getOptimalDFTSize(A.rows + B.rows - 1);
	// 分配临时变量并初始化为0
	Mat tA(dftSize, A.type(), Scalar::all(0));
	Mat tB(dftSize, B.type(), Scalar::all(0));
	// A、B分别放在临时变量A、临时变量B的左上角
	Mat roiA(tA, Rect(0, 0, A.cols, A.rows));
	A.copyTo(roiA);
	Mat roiB(tB, Rect(0, 0, B.cols, B.rows));
	B.copyTo(roiB);
	// 对临时变量A、B进行傅里叶变换
	// 使用“非零行”参数进行更快的处理
	dft(tA, tA, 0, A.rows);
	dft(tB, tB, 0, B.rows);
	// 乘以频谱
	//该功能可以很好地处理打包的频谱表示
	mulSpectrums(tA, tB, tA, DFT_COMPLEX_OUTPUT);
	// 将乘积从频率域转换回来
	dft(tA, tA, DFT_INVERSE + DFT_SCALE);
	//将结果保存，只需要保存和原图一样的大小即可
	//tA(Rect(0, 0, C.cols, C.rows)).copyTo(C);
	tA(Rect(B.cols - 1, B.rows - 1, C.cols, C.rows)).copyTo(C);
	//return C;
}

ll ** a;
double getA(int y, int x) {
	if (y < 0 || x < 0)return 0;
	if (y >= lH)y = lH - 1;
	if (x >= lW)x = lW - 1;
	return a[y][x];
}

int main()
{
	//初始化随机数种子
	srand((unsigned)time(NULL));
	img = imread("img.jpg", CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);//尽量保证无损
	cout << "**************实验五-傅里叶变换**************" << endl;
	//输出文件
	vector<int>compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_params.push_back(100);

	int imgcol = img.cols;
	int imgrow = img.rows;
	//窗口大小
	//int windowsRow = 50;
	//int windowsCol = 300;
	//int windowsRowArr[6] = { 50,40,10,20,15,17 };
	//int windowsColArr[6] = { 14,70,100,78,21,98 };

	int windowsRowArr[6] = { 10,30,50,70,90,110 };
	int windowsColArr[6] = { 20,40,60,80,100,120 };

	int windowsRow, windowsCol;
	for (int i = 0; i < 6; i++) {
		windowsRow = windowsRowArr[i];
		windowsCol = windowsColArr[i];
	

		int startrow = getrand(0, imgrow - windowsRow);
		int startcol = getrand(0, imgcol - windowsCol);

		/*int startrow = 20;
		int startcol = 100;*/


		Mat win;
		img.copyTo(win);
		win = win.colRange(startcol, startcol + windowsCol);
		win = win.rowRange(startrow, startrow + windowsRow);
		cout << "StartY = " << startrow << "  StartX = " << startcol << endl;

		imshow("win", win);
		//快速模板匹配计算
		Mat src, dst;
		img.copyTo(src);
		win.copyTo(dst);
		cvtColor(src, src, CV_BGR2GRAY);
		cvtColor(dst, dst, CV_BGR2GRAY);
		//翻转，做卷积运算需要
		flip(dst, dst, -1);


		H = src.rows;
		lH = H;
		mH = dst.rows;
		H -= mH;
		W = src.cols;
		lW = W;
		mW = dst.cols;
		//cout << H << " " << mH << " " << W << " " << mW << endl;
		W -= mW;
		double SSD = 1e12, diff = 0.0;
		int fx = 0, fy = 0;

		clock_t start = clock();


		//求T常数
		double cons = 0.0;
		for (int y = 0; y < dst.rows; y++) {
			for (int x = 0; x < dst.cols; x++) {
				uchar *pt = dst.ptr(y, x);
				cons += (*pt)*(*pt);			
			}
		}

		src = Mat_<float>(src);
		dst = Mat_<float>(dst);

		//求积分图
	
		a = new ll*[src.rows];
		for (int i = 0; i < src.rows; i++) {
			a[i] = new ll[src.cols];
		}

		for (int y = 0; y < src.rows; y++) {
			for (int x = 0; x < src.cols; x++) {
				int pt = src.at<float>(y, x);
				if (x == 0 && y == 0)a[y][x] = (ll)(pt)*(ll)(pt);
				else if (y == 0) a[y][x] = a[y][x - 1] + (ll)(pt)*(ll)(pt);
				else if (x == 0) a[y][x] = a[y - 1][x] + (ll)(pt)*(ll)(pt);
				else a[y][x] = a[y - 1][x] + a[y][x - 1] - a[y - 1][x - 1] + (ll)(pt)*(ll)(pt);
			}
		}


		Mat conv;
		convolveDFT(src, dst,conv);

		cH = conv.rows;
		//cH -= mH;
		cW = conv.cols;
		//cW -= mW;

	
		for (int y = 0; y < cH; y++) {
			for (int x = 0; x < cW; x++) {
				//diff = 0.0;
				diff = getA(y + mH - 1, x + mW - 1) - getA(y - 1, x + mW - 1) - getA(y - 1 + mH, x - 1) + getA(y - 1, x - 1);
				//cout << "diff: "<<diff << endl;
				//注释掉常数部分
				//diff += cons;
				diff -= 2.0 * conv.at<float>(y, x);
				if (diff < SSD) {
					SSD = diff;
					//cout << SSD << endl;
					fx = x;
					fy = y;
				}

			}
		}
	
		clock_t ends = clock();
		cout << "Running Time : " << (double)(ends - start) / CLOCKS_PER_SEC << " s" << endl;
		cout << "SSD = " << SSD << " y = " << fy << " x = " << fx << endl;
		Mat output;
		img.copyTo(output);
		rectangle(output, Rect(fx, fy, mW, mH), Scalar(0, 0, 255), 1, 1, 0);

		imshow("output", output);

		imwrite("快速匹配图像.jpg", output, compression_params);
		cv::waitKey();
	}
		
	return 0;
}