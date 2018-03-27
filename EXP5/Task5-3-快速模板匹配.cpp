//ͨ��
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

//���[a,b]�������
int getrand(int a, int b) {
	return (rand() % (b - a + 1)) + a;
}

void convolveDFT(Mat A, Mat B,Mat& C)
{
	//
	// ���䷵�ص�Mat
	C.create(abs(A.rows - B.rows) + 1, abs(A.cols - B.cols) + 1, A.type());
	Size dftSize;
	// ����DFTʹ�õĴ�С������FFT����
	dftSize.width = getOptimalDFTSize(A.cols + B.cols - 1);
	dftSize.height = getOptimalDFTSize(A.rows + B.rows - 1);
	// ������ʱ��������ʼ��Ϊ0
	Mat tA(dftSize, A.type(), Scalar::all(0));
	Mat tB(dftSize, B.type(), Scalar::all(0));
	// A��B�ֱ������ʱ����A����ʱ����B�����Ͻ�
	Mat roiA(tA, Rect(0, 0, A.cols, A.rows));
	A.copyTo(roiA);
	Mat roiB(tB, Rect(0, 0, B.cols, B.rows));
	B.copyTo(roiB);
	// ����ʱ����A��B���и���Ҷ�任
	// ʹ�á������С��������и���Ĵ���
	dft(tA, tA, 0, A.rows);
	dft(tB, tB, 0, B.rows);
	// ����Ƶ��
	//�ù��ܿ��Ժܺõش�������Ƶ�ױ�ʾ
	mulSpectrums(tA, tB, tA, DFT_COMPLEX_OUTPUT);
	// ���˻���Ƶ����ת������
	dft(tA, tA, DFT_INVERSE + DFT_SCALE);
	//��������棬ֻ��Ҫ�����ԭͼһ���Ĵ�С����
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
	//��ʼ�����������
	srand((unsigned)time(NULL));
	img = imread("img.jpg", CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);//������֤����
	cout << "**************ʵ����-����Ҷ�任**************" << endl;
	//����ļ�
	vector<int>compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_params.push_back(100);

	int imgcol = img.cols;
	int imgrow = img.rows;
	//���ڴ�С
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
		//����ģ��ƥ�����
		Mat src, dst;
		img.copyTo(src);
		win.copyTo(dst);
		cvtColor(src, src, CV_BGR2GRAY);
		cvtColor(dst, dst, CV_BGR2GRAY);
		//��ת�������������Ҫ
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


		//��T����
		double cons = 0.0;
		for (int y = 0; y < dst.rows; y++) {
			for (int x = 0; x < dst.cols; x++) {
				uchar *pt = dst.ptr(y, x);
				cons += (*pt)*(*pt);			
			}
		}

		src = Mat_<float>(src);
		dst = Mat_<float>(dst);

		//�����ͼ
	
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
				//ע�͵���������
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

		imwrite("����ƥ��ͼ��.jpg", output, compression_params);
		cv::waitKey();
	}
		
	return 0;
}