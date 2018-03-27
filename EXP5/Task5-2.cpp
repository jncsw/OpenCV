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
#include <stdio.h>
using namespace cv;
using namespace std;

Mat img, output;

vector<Mat> channels;
int ch;



int main()
{
	img = imread("img.png", IMREAD_GRAYSCALE);//��ȡ�Ҷ�ͼ��
	cout << "**************ʵ����-����Ҷ�任**************" << endl;
	//����ļ�
	vector<int>compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_params.push_back(100);

	//Ϊ�ӿ�DFT�����ٶȣ���ͼ����зŴ�

	int x = getOptimalDFTSize(img.rows);
	int y = getOptimalDFTSize(img.cols);
	Mat border;
	copyMakeBorder(img, border, 0, x - img.rows, 0, y - img.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes[] = { Mat_<float>(border), Mat::zeros(border.size(), CV_32F) };

	Mat complexImg;
	merge(planes, 2, complexImg);

	//����DFT�任

	dft(complexImg, complexImg);
	
	
	//����������log(1 + sqrt(Re(DFT(img))**2 + Im(DFT(img))**2)) �ļ���

	split(complexImg, planes);
	//cout << (*planes).type() << endl;
	//cout << (*planes).channels() << endl;

	//�����ݶȣ���sqrt(Re(DFT(img))**2 + Im(DFT(img))**2)����
	magnitude(planes[0], planes[1], planes[0]);
	//��Ƶ�����������д���
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
	idft(tmp, tmp);//����Ҷ��任
	split(tmp, idftMat);
	//�����ݶȣ���sqrt(Re(DFT(img))**2 + Im(DFT(img))**2)����
	magnitude(idftMat[0], idftMat[1], idftMat[0]);



	Mat mag = idftMat[0];
	mag += Scalar::all(1);
	//ȡlog
	//log(mag, mag);

	//���ͼ����л�������������Ƶ���ǲ��ԳƵģ�Ҫ�����޼�
	//& -2 = & 1111 1110���������һλ��0����֤��ż��


	mag = mag(Rect(0, 0, mag.cols & -2, mag.rows & -2));

	Mat orioutput;
	mag.copyTo(orioutput);
	normalize(orioutput, orioutput, 1, 0, NORM_MINMAX);
	orioutput.convertTo(output, CV_8UC3, 255);
	//��ԭԭʼͼ�������
	output = output.rowRange(0, img.rows);
	output = output.colRange(0, img.cols);
	imshow("IDFTͼ��", output);
	
	imwrite("IDFTͼ��.png", output, compression_params);

	waitKey();
	return 0;
}