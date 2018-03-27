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

Mat img, output;

vector<Mat> channels;
int ch;



int main()
{
	img = imread("img.png", IMREAD_GRAYSCALE);//读取灰度图像
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
	//cout << (*planes).type() << endl;
	//cout << (*planes).channels() << endl;

	//计算梯度，即sqrt(Re(DFT(img))**2 + Im(DFT(img))**2)部分
	magnitude(planes[0], planes[1], planes[0]);
	//在频率域对亮点进行处理
	complexImg.at<Vec2f>(0, 10)[0] = 0;
	complexImg.at<Vec2f>(0, 10)[1] = 0;
	complexImg.at<Vec2f>(0, 310)[0] = 0;
	complexImg.at<Vec2f>(0, 310)[1] = 0;
	for (int i = 6; i <= 14; i++) {
		complexImg.at<Vec2f>(0, i)[0] = 0;
		complexImg.at<Vec2f>(0, i)[1] = 0;
	}
	for (int i =308; i <= 312; i++) {
		complexImg.at<Vec2f>(0, i)[0] = 0;
		complexImg.at<Vec2f>(0, i)[1] = 0;
	}
	for (int i = 315; i <= 317; i++) {
		complexImg.at<Vec2f>(319, i)[0] = 0;
		complexImg.at<Vec2f>(319, i)[1] = 0;
	}
	for (int i = 308; i <= 312; i++) {
		complexImg.at<Vec2f>(0, i)[0] = 0;
		complexImg.at<Vec2f>(0, i)[1] = 0;
	}
	for (int i = 314; i <= 319; i++) {
		complexImg.at<Vec2f>(i, 0)[0] = 0;
		complexImg.at<Vec2f>(i, 0)[1] = 0;
	}
	for (int i = 314; i <= 319; i++) {
		complexImg.at<Vec2f>(i, 319)[0] = 0;
		complexImg.at<Vec2f>(i, 319)[1] = 0;
	}
	Mat tmp;
	complexImg.copyTo(tmp);
	Mat idftMat[] = { Mat::zeros(planes[0].size(),CV_32F),Mat::zeros(planes[0].size(),CV_32F) };
	idft(tmp, tmp);//傅里叶逆变换
	split(tmp, idftMat);
	//计算梯度，即sqrt(Re(DFT(img))**2 + Im(DFT(img))**2)部分
	magnitude(idftMat[0], idftMat[1], idftMat[0]);



	Mat mag = idftMat[0];
	mag += Scalar::all(1);
	//取log
	//log(mag, mag);

	//如果图像的行或列是奇数，其频谱是不对称的，要进行修剪
	//& -2 = & 1111 1110，即将最后一位置0，保证是偶数


	mag = mag(Rect(0, 0, mag.cols & -2, mag.rows & -2));

	Mat orioutput;
	mag.copyTo(orioutput);
	normalize(orioutput, orioutput, 1, 0, NORM_MINMAX);
	orioutput.convertTo(output, CV_8UC3, 255);
	//还原原始图像的像素
	output = output.rowRange(0, img.rows);
	output = output.colRange(0, img.cols);
	imshow("IDFT图像", output);
	
	imwrite("IDFT图像.png", output, compression_params);

	waitKey();
	return 0;
}