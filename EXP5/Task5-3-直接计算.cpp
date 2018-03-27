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

Mat img, output;

vector<Mat> channels;
int ch;

int getrand(int a, int b) {
	return (rand() % (b - a + 1)) + a;

}

int main()
{
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
	//int windowsRow = 10;
	//int windowsCol = 30;

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
		Mat win;
		img.copyTo(win);
		win = win.colRange(startcol, startcol + windowsCol);
		win = win.rowRange(startrow, startrow + windowsRow);
		cout << "StartY = " << startrow << "  StartX = " << startcol << endl;

		imshow("win", win);
		//直接计算
		Mat src, dst;
		img.copyTo(src);
		win.copyTo(dst);
		cvtColor(src, src, CV_BGR2GRAY);
		cvtColor(dst, dst, CV_BGR2GRAY);
		int H = src.rows;
		int mH = dst.rows;
		H -= mH;
		int W = src.cols;
		int mW = dst.cols;
		//cout << H << " " << mH << " " << W << " " << mW << endl;
		W -= mW;
		double SSD = 1e12, diff = 0.0;
		int fx = 0, fy = 0;
		clock_t start = clock();
		for (int y = 0; y < H; y++) {
			for (int x = 0; x < W; x++) {
				//cout << y << " " << x << endl;
				diff = 0.0;
				for (int ty = 0; ty < mH; ty++) {
					for (int tx = 0; tx < mW; tx++) {
						const uchar *source = src.ptr(y + ty, x + tx);
						const uchar *dist = dst.ptr(ty, tx);
						diff += (source[0] - dist[0])*(source[0] - dist[0]);
					}
				}
				//cout << diff << endl;
				if (diff < SSD) {
					SSD = diff;
					//cout << "SSD = "<< SSD << endl;
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

		imwrite("匹配图像.jpg", output, compression_params);

		waitKey();


	}


	return 0;
}