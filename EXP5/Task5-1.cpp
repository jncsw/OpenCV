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
#include <stdio.h>
using namespace cv;
using namespace std; 

Mat img,output;

vector<Mat> channels;
int ch;



int main()
{
	img = imread("img.jpg", IMREAD_GRAYSCALE);//读取灰度图像
	cout << "**************实验五-傅里叶变换**************" << endl;
	//输出文件
	vector<int>compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_params.push_back(100);

	//为加快DFT处理速度，将图像进行放大

	int x = getOptimalDFTSize(img.rows);
	int y = getOptimalDFTSize(img.cols);
	Mat border;
	copyMakeBorder(img, border, 0, x - img.rows, 0, y - img.cols, BORDER_CONSTANT, Scalar::all(0));
	
	Mat planes[] = { Mat_<float>(border), Mat::zeros(border.size(), CV_32F) };
	
	Mat complexImg;
	merge(planes, 2, complexImg);

	//进行DFT变换

	dft(complexImg, complexImg);

	//接下来进行log(1 + sqrt(Re(DFT(img))**2 + Im(DFT(img))**2)) 的计算

	split(complexImg, planes);
	//计算梯度，即sqrt(Re(DFT(img))**2 + Im(DFT(img))**2)部分
	magnitude(planes[0], planes[1], planes[0]);
	Mat mag = planes[0];
	// +1
	mag += Scalar::all(1);
	//取log
	log(mag, mag);
	

	//如果图像的行或列是奇数，其频谱是不对称的，要进行修剪
	//& -2 = & 1111 1110，即将最后一位置0，保证是偶数


	mag = mag(Rect(0, 0, mag.cols & -2, mag.rows & -2));
	Mat orioutput;
	mag.copyTo(orioutput);
	normalize(orioutput, orioutput, 0, 1, NORM_MINMAX);
	imshow("非移中图像", orioutput);
	orioutput.convertTo(output, CV_8UC3, 255);
	imwrite("非移中图像.jpg", output, compression_params);
	//移中处理
	int cx = mag.cols / 2;
	int cy = mag.rows / 2;
	Mat tmp;
	Mat q0(mag, Rect(0, 0, cx, cy));
	Mat q1(mag, Rect(cx, 0, cx, cy));
	Mat q2(mag, Rect(0, cy, cx, cy));
	Mat q3(mag, Rect(cx, cy, cx, cy));
	//交换左上右下
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	//交换右上左下
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	normalize(mag, mag, 0, 1, NORM_MINMAX);
	imshow("移中图像", mag);
	mag.convertTo(output, CV_8UC3,255);
	imwrite("移中图像.jpg", output, compression_params);
	
	


	waitKey();
	return 0;
}