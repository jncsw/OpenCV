//通用
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui.hpp>
//gif
#include "FreeImage.h"  
#pragma comment(lib,"FreeImage.lib")
//缩放
#include <opencv2\imgproc\imgproc.hpp> 
//输出
#include <vector> 
#include <opencv2/imgproc/imgproc.hpp>   
#include<iostream>
using namespace cv;
using namespace std;
IplImage*  gif2ipl(const char* filename)
{
	FreeImage_Initialise();         //load the FreeImage function lib  
	FREE_IMAGE_FORMAT fif = FIF_GIF;
	FIBITMAP* fiBmp = FreeImage_Load(fif, filename, GIF_DEFAULT);
	FIMULTIBITMAP * pGIF = FreeImage_OpenMultiBitmap(fif, filename, 0, 1, 0, GIF_PLAYBACK);
	//  FIBITMAPINFO fiBmpInfo = getfiBmpInfo(fiBmp);  
	int gifImgCnt = FreeImage_GetPageCount(pGIF);
	FIBITMAP * pFrame;
	int width, height;
	width = FreeImage_GetWidth(fiBmp);
	height = FreeImage_GetHeight(fiBmp);
	IplImage * iplImg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	iplImg->origin = 1;//should set to 1-top-left structure(Windows bitmap style)  
	RGBQUAD* ptrPalette = new RGBQUAD; // = FreeImage_GetPalette(fiBmp);  
	BYTE intens;
	BYTE* pIntensity = &intens;
	cvNamedWindow("gif", 0);
	//printf("gifImgCnt %d \n", gifImgCnt);
	for (int curFrame = 0; curFrame<gifImgCnt; curFrame++)
	{
		pFrame = FreeImage_LockPage(pGIF, curFrame);
		//ptrPalette = FreeImage_GetPalette(pFrame);  
		char * ptrImgDataPerLine;
		for (int i = 0; i<height; i++)
		{
			ptrImgDataPerLine = iplImg->imageData + i*iplImg->widthStep;
			for (int j = 0; j<width; j++)
			{
				//get the pixel index   
				//FreeImage_GetPixelIndex(pFrame,j,i,pIntensity);    
				FreeImage_GetPixelColor(pFrame, j, i, ptrPalette);
				ptrImgDataPerLine[3 * j] = ptrPalette->rgbBlue;
				ptrImgDataPerLine[3 * j + 1] = ptrPalette->rgbGreen;
				ptrImgDataPerLine[3 * j + 2] = ptrPalette->rgbRed;
				//ptrImgDataPerLine[3*j] = ptrPalette[intens].rgbBlue;  
				//ptrImgDataPerLine[3*j+1] = ptrPalette[intens].rgbGreen;  
				//ptrImgDataPerLine[3*j+2] = ptrPalette[intens].rgbRed;  
			}
		}

		printf("convert curFrame end %d \n", curFrame);
		cvShowImage("gif", iplImg);
		cvWaitKey(30);
		FreeImage_UnlockPage(pGIF, pFrame, 1);
	}
	FreeImage_Unload(fiBmp);
	FreeImage_DeInitialise();
	return iplImg;
}


void getChannel(const Mat &img, Mat &cimg, int c) {
	cimg.create(img.size(), CV_8UC1);
	for (int y = 0; y < img.rows; ++y) {
		for (int x = 0; x < img.cols; ++x) {
			const uchar *p = img.ptr(y, x);
			uchar *d = cimg.ptr(y, x);
			*d = p[c];

		}
	}
}

void MergeImg(const Mat &img, const Mat &alpha,Mat &bg, Mat &output) {
	output.create(img.size(), CV_8UC3);
	Mat tmp = bg;
	Mat resz;
	imshow("缩放前", bg);
	resize(tmp, resz, Size(img.cols,img.rows), 0, 0, INTER_LINEAR);
	imshow("缩放后",resz);

	//方法一
	for (int y = 0; y < img.rows; ++y)
		for (int x = 0; x < img.cols; ++x)
		{
			const uchar *ptimg = img.ptr(y, x);
			const uchar *pta = alpha.ptr(y, x);
			const uchar *ptbg = resz.ptr(y, x);
			uchar *out = output.ptr(y, x);
			//std::cout << "*pta" << std::endl;
			double a = (double)(*pta) / 255.0;
			for (int i = 0; i < 3; ++i)
			{
				*out = (*ptimg) * a + (*ptbg) * (1 - a);
				++out;
				++ptimg;
				++ptbg;
			}
		}

	//方法二
	std::vector<cv::Mat>scr_channels;
	std::vector<cv::Mat>dstt_channels;
	split(img, scr_channels);
	split(resz, dstt_channels);
	CV_Assert(scr_channels.size() == 4 && dstt_channels.size() == 3);

	for (int i = 0; i < 3; i++)
	{
		dstt_channels[i] = dstt_channels[i].mul(255.0 - scr_channels[3], 1.0 / 255.0);
		dstt_channels[i] += scr_channels[i].mul(scr_channels[3], 1.0 / 255.0);
	}
	merge(dstt_channels, output);

}

int main()
{
	
	Mat img = imread("png.png");
	namedWindow("png图片", WINDOW_NORMAL);
	
	imshow("png图片",img);

	img = imread("jpg.jpg");
	imshow("jpg图片", img);
	
	img = imread("bmp.bmp");
	imshow("bmp图片", img);

	//失败的方法
	//img = imread("gif.gif");
	//if (img.empty()) {std::printf("read null!");}
	////imshow("gif", img);

	//读取gif
	char* filename = "gif.gif";
	IplImage* iplImg = gif2ipl(filename);
	cvReleaseImage(&iplImg);


	
	Mat aimg = imread("a.png", IMREAD_UNCHANGED);
	imshow("a.png", aimg);
	

	Mat channels[4];
	split(aimg, channels);
	Mat alp = channels[3];
	imshow("Alpha通道方法一",alp);


	Mat cimg;
	getChannel(aimg, cimg, 3);
	imshow("Alpha通道方法二", cimg);

	Mat bg = imread("bg.jpg",CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);//尽量保证无损
	namedWindow("背景图");
	imshow("背景图", bg);
	Mat output;
	MergeImg(aimg, cimg, bg, output);
	imshow("合成图", output);
	

	//输出文件
	vector<int>compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_params.push_back(100);

	imwrite("Output.jpg", output, compression_params);
	
	waitKey();
	return 0;
}