//ͨ��
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h> 
#include <vector> 
//C++
#include<iostream>
#include<map>
#include <stdio.h>
using namespace cv;
using namespace std;

Mat img, output;
//���鼯ӳ�����
int *a;
//�洢ͼ��ͬ����ı��
int **f;
//��¼һ����ɫ
struct node {
	int x, y, z;

};


//���鼯��find�����������ʺű��ʽʵ�֣���ʵ����·��ѹ�����͸��Ӷ�
int find(int x) {
	//cout << "find:" << x << " "<<a[x]<<endl;;
	return (a[x] == x)?x:(a[x] = find(a[x]));
}
//���鼯�ĺϲ�
void merg(int u, int v) {
	//cout << "merg" << endl;
	a[find(u)] = find(v);
}
//�������ɫ
RNG rng(0xFFFFFFFF);

//�����Ŷ�����ɫ��ӳ���ϵ
map<int, node>mp;
map<int, node>::iterator mit;
//��ñ����ɫ�ķ���
node getColor(int u, int v) {
	int t = find(f[u][v]);
	node nod;
	nod.x = 1;
	//������ɫ�Ƿ��Ѿ�������
	mit = mp.find(t);
	if (mit == mp.end()) {
		//��������µ���ɫ
		nod.x = rng.uniform(0, 255);
		nod.y = rng.uniform(0, 255);
		nod.z = rng.uniform(0, 255);
		mp[t] = nod;
	}
	else {
		//����map�б������ɫ
		nod = mp[t];
	}
	return nod;
}
//���������ͨ��
void calc() {
	mp.clear();
	int width = img.cols;
	int height = img.rows;
	//��̬��������
	f = new int*[height];
	a = new int[width*height];
	for (int i = 0; i < width*height; i++) {
		a[i] = i;//���鼯��ʼ��
	}
	for (int i = 0; i<height; i++)
	{
		f[i] = new int[width];
	}
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			f[i][j] = -1;//��ų�ʼ��
		}
	}
	//��¼��ͨ����	
	int cnt = 0;
	//����ÿ������
	for (int y = 0; y < img.rows; ++y) {
		for (int x = 0; x < img.cols; ++x) {
			bool up = 0;//�Ƿ���Ϸ�������ͬ
			bool left = 0;//�Ƿ�����������ͬ
			if (y - 1 >= 0) {
				if (img.at<cv::Vec3b>(y - 1,x) == img.at<cv::Vec3b>(y,x))up = 1;

			}
			if (x - 1 > 0) {
				if (img.at<cv::Vec3b>(y,x-1) == img.at<cv::Vec3b>(y,x))left = 1;
			}
			//cout <<up << " "<<left<<endl;
			if (!up && !left) {//���Ϸ����󷽾�����ͬ����Ϊһ���µ���ͨ��
				f[y][x] = ++cnt;
			}
			else if ((!up && left) || (!left && up)) {//����߻��ϱ�֮һ��ͬ������ֵͬ
				if (up)f[y][x] = f[y - 1][x];
				else f[y][x] = f[y][x-1];
			}
			else if (f[y - 1][x] != f[y][x - 1]) {//���Ϸ����󷽾���ͬ�����ж��Ϸ������Ƿ���ͬ���������ͬ����ϲ�����
				f[y][x] = f[y][x-1];//�ô濼��
				merg(f[y - 1][x], f[y][x - 1]);
			}else f[y][x] = f[y][x - 1];//�����ͬ��ֱ�Ӹ�ֵ

		}
	}
	
	output.create(img.size(), CV_8UC3);//�����ɫͼ��
	for (int y = 0; y < output.rows; ++y) {
		for (int x = 0; x < output.cols; ++x) {
			node c = getColor(y,x);
			output.at<cv::Vec3b>(y, x)[0] = c.x;
			output.at<cv::Vec3b>(y, x)[1] = c.y;
			output.at<cv::Vec3b>(y, x)[2] = c.z;
		}
	}

}
int main()
{

	img = imread("cc_input.png", CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);//������֤����
	cout << "**************ʵ��3.2**************" << endl;
	imshow("ʵ��3.2", img);
	rng = cvRNG(cvGetTickCount());//�������ʼ��
	//����ļ�
	vector<int>compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_params.push_back(100);
	calc();
	imshow("������ͨ��ͼ��", output);
	imwrite("������ͨ��ͼ��.jpg", output, compression_params);
	waitKey();
	return 0;
}