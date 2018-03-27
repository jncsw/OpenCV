//通用
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
//并查集映射矩阵
int *a;
//存储图像不同区域的标号
int **f;
//记录一种颜色
struct node {
	int x, y, z;

};


//并查集的find方法，采用问号表达式实现，并实现了路径压缩降低复杂度
int find(int x) {
	//cout << "find:" << x << " "<<a[x]<<endl;;
	return (a[x] == x)?x:(a[x] = find(a[x]));
}
//并查集的合并
void merg(int u, int v) {
	//cout << "merg" << endl;
	a[find(u)] = find(v);
}
//随机化颜色
RNG rng(0xFFFFFFFF);

//保存标号对于颜色的映射关系
map<int, node>mp;
map<int, node>::iterator mit;
//获得标号颜色的方法
node getColor(int u, int v) {
	int t = find(f[u][v]);
	node nod;
	nod.x = 1;
	//查找颜色是否已经被分配
	mit = mp.find(t);
	if (mit == mp.end()) {
		//随机生成新的颜色
		nod.x = rng.uniform(0, 255);
		nod.y = rng.uniform(0, 255);
		nod.z = rng.uniform(0, 255);
		mp[t] = nod;
	}
	else {
		//返回map中保存的颜色
		nod = mp[t];
	}
	return nod;
}
//计算快速连通域
void calc() {
	mp.clear();
	int width = img.cols;
	int height = img.rows;
	//动态分配数组
	f = new int*[height];
	a = new int[width*height];
	for (int i = 0; i < width*height; i++) {
		a[i] = i;//并查集初始化
	}
	for (int i = 0; i<height; i++)
	{
		f[i] = new int[width];
	}
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			f[i][j] = -1;//标号初始化
		}
	}
	//记录连通分量	
	int cnt = 0;
	//遍历每个像素
	for (int y = 0; y < img.rows; ++y) {
		for (int x = 0; x < img.cols; ++x) {
			bool up = 0;//是否和上方像素相同
			bool left = 0;//是否和左边像素相同
			if (y - 1 >= 0) {
				if (img.at<cv::Vec3b>(y - 1,x) == img.at<cv::Vec3b>(y,x))up = 1;

			}
			if (x - 1 > 0) {
				if (img.at<cv::Vec3b>(y,x-1) == img.at<cv::Vec3b>(y,x))left = 1;
			}
			//cout <<up << " "<<left<<endl;
			if (!up && !left) {//与上方和左方均不相同，则为一个新的连通块
				f[y][x] = ++cnt;
			}
			else if ((!up && left) || (!left && up)) {//与左边或上边之一相同，则赋相同值
				if (up)f[y][x] = f[y - 1][x];
				else f[y][x] = f[y][x-1];
			}
			else if (f[y - 1][x] != f[y][x - 1]) {//与上方和左方均相同，则判断上方和左方是否相同，如果不相同，则合并集合
				f[y][x] = f[y][x-1];//访存考虑
				merg(f[y - 1][x], f[y][x - 1]);
			}else f[y][x] = f[y][x - 1];//如果相同，直接赋值

		}
	}
	
	output.create(img.size(), CV_8UC3);//输出彩色图像
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

	img = imread("cc_input.png", CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);//尽量保证无损
	cout << "**************实验3.2**************" << endl;
	imshow("实验3.2", img);
	rng = cvRNG(cvGetTickCount());//随机数初始化
	//输出文件
	vector<int>compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_params.push_back(100);
	calc();
	imshow("快速连通域图像", output);
	imwrite("快速连通域图像.jpg", output, compression_params);
	waitKey();
	return 0;
}