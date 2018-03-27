//ͨ��
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h> 
#include <vector> 
//C++
#include<iostream>
#include <stdio.h>
using namespace cv;
using namespace std;

Mat img, output;

vector<Mat> channels;
int ch;
//���ڻ���ͼ�Ŀ��پ�ֵ�˲�
void MeanFilter(const Mat &input, Mat &output, int window_size) {
	int SizeOfWindows = window_size;
	int Addition = (window_size-1)/2;
	Mat newimg;
	cout << "SizeOfWindows:" << SizeOfWindows << endl;
	newimg.create(input.rows + SizeOfWindows, input.cols + SizeOfWindows, input.type());
	cout << "new img size:\nrows:" << input.rows + SizeOfWindows << "\ncols:" << input.cols + SizeOfWindows << endl;
	cout << "Addition:" << Addition << endl;
	//�߽紦��-���ӱ߽�
	for (int y = 0; y < input.rows; ++y)
	{
		for (int x = 0; x < input.cols; ++x)
		{
			const uchar *ptimg = input.ptr(y, x);
			uchar *ptnew = newimg.ptr(y + Addition, x + Addition);
			*ptnew = *ptimg;
		}
	}

	//�߽縳ֵ
	//����
	for (int x = 0; x < input.cols; ++x) {
		int tx = x + Addition;
		int y = Addition; int y2 = newimg.rows - Addition - 1;
		for (int dy = 1; dy <= Addition; dy++) {
			//cout << y + dy <<" "<< y - dy<< " "<< tx<<endl;
			uchar *ptnew = newimg.ptr(y + dy, tx);
			uchar *ptnew2 = newimg.ptr(y - dy, tx);
			*ptnew2 = *ptnew;
			//cout <<"2: " <<y2 + dy << " " << y2 - dy << " " << tx << endl;
			uchar *ptnew1 = newimg.ptr(y2 - dy, tx);
			uchar *ptnew21 = newimg.ptr(y2 + dy, tx);
			*ptnew21 = *ptnew1;
		}

	}


	//����
	for (int ty = 0; ty < input.rows; ty++) {
		int y = ty + Addition;
		int x = Addition;
		int x2 = newimg.cols - Addition - 1;
		for (int dx = 1; dx <= Addition; dx++) {
			uchar *ptnew = newimg.ptr(y, x + dx);
			uchar *ptnew2 = newimg.ptr(y, x - dx);
			*ptnew2 = *ptnew;
			uchar *ptnew1 = newimg.ptr(y, x2 - dx);
			uchar *ptnew21 = newimg.ptr(y, x2 + dx);
			*ptnew21 = *ptnew1;
		}
	}

	//�ĸ���
	//����
	for (int x = 0; x < Addition; x++) {
		for (int y = 0; y < Addition; y++) {

			const uchar *pt = input.ptr(y, x);
			uchar *ptnew2 = newimg.ptr(y, x);
			*ptnew2 = *pt;
		}
	}
	//����
	for (int x = 0; x < Addition; x++) {
		for (int y = 0; y < Addition; y++) {

			const uchar *pt = input.ptr(y, input.cols - 1 - x);
			uchar *ptnew2 = newimg.ptr(y, x + Addition + input.cols);
			*ptnew2 = *pt;
		}
	}
	//����
	for (int x = 0; x < Addition; x++) {
		for (int y = 0; y < Addition; y++) {
			const uchar *pt = input.ptr(input.rows - 1 - y, x);
			uchar *ptnew2 = newimg.ptr(y + Addition + input.rows, x);
			*ptnew2 = *pt;
		}
	}

	//����
	for (int x = 0; x < Addition; x++) {
		for (int y = 0; y < Addition; y++) {

			const uchar *pt = input.ptr(input.rows - 1 - y, input.cols - 1 - x);
			uchar *ptnew2 = newimg.ptr(y + Addition + input.rows, x + Addition + input.cols);
			*ptnew2 = *pt;
		}
	}

	imshow("newimg", newimg);

	int ** a;
	a = new int*[newimg.rows];
	for (int i = 0; i < newimg.rows; i++) {
		a[i] = new int[newimg.cols];
	}
	for (int y = 0; y < newimg.rows; y++) {
		for (int x = 0; x < newimg.cols; x++) {
			uchar *pt = newimg.ptr(y, x);
			if (x == 0 && y == 0)a[y][x] = (int)(*pt);
			else if (y == 0) a[y][x] = a[y][x - 1] + (int)(*pt);
			else if (x == 0) a[y][x] = a[y - 1][x] + (int)(*pt);
			else a[y][x] = a[y - 1][x] + a[y][x - 1] - a[y - 1][x - 1] + (int)(*pt);
		}
	}
	
	//�����ֵ
	Mat tmp;
	tmp.create(newimg.size(), newimg.type());
	for (int y = 0; y < newimg.rows; y++) {
		for (int x = 0; x < newimg.cols; x++) {
			uchar *p = tmp.ptr(y, x);
			if (y - Addition-1 >= 0 && y + Addition < tmp.rows && x - Addition-1 >= 0 && x + Addition < tmp.cols) {
				
				*p = ((a[y + Addition][x + Addition] + a[y - 1 - Addition][x - 1 - Addition] - a[y - 1 - Addition][x + Addition] - a[y + Addition][x - 1 - Addition]) / ((Addition * 2 + 1)*(Addition * 2 + 1)));
			}
			else *p = 0;
		}
	}



	//�������ͼ�����ҳ�������
	for (int y = 0; y < input.rows; ++y)
	{
		for (int x = 0; x < input.cols; ++x)
		{
			const uchar *ptnew = tmp.ptr(y + Addition, x + Addition);
			uchar *ptout = output.ptr(y, x);
			*ptout = *ptnew;
		}
	}





}

int main()
{
	img = imread("img.jpg", CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);//������֤����
	cout << "**************ʵ����-���پ�ֵ�˲�**************" << endl;
	//����ļ�
	vector<int>compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_params.push_back(100);

	split(img, channels);


	ch = img.channels();
	if (channels.size() == 0) {
		cout << "�Ҳ����ļ�" << endl;

	}
	else {

		cout << "channels:" << ch << endl;
		int windows = 8;
		cout << "�������˲����ڴ�С��" << endl;
		cin >> windows;
		while (windows > img.rows || windows > img.cols || windows < 0 || windows%2==0) {
			cout << "�˲����ڴ�С�������󣬱���Ϊ�Ǹ�ż����С�ڵ���ͼƬ��С�����������룺" << endl;
			cin >> windows;
		}


		vector<Mat> after;
		after.clear();

		for (int i = 0; i < ch; i++) {
			Mat tout;
			tout.create(img.size(), CV_8UC1);
			MeanFilter(channels[i], tout, windows);
			after.push_back(tout);

		}
		cout << after.size() << endl;
		merge(after, output);
		imshow("ʵ����ԭͼ", img);
		imshow("���پ�ֵ�˲�", output);
		imwrite("���پ�ֵ�˲�.jpg", output, compression_params);
	}


	waitKey();
	return 0;
}