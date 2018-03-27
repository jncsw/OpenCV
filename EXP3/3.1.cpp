//通用
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h> 
#include <vector> 
//C++
#include<iostream>
#include <stdio.h>
using namespace cv;
using namespace std;

Mat img,output;
int ch;
vector<Mat> channels;

//计算直方图
void calc_hist(uchar *data, int width, int height, int step, int H[256])
{
	memset(H, 0, sizeof(H[0]) * 256);
	uchar *row = data;
	for (int yi = 0; yi<height; ++yi, row += step)
	{
		for (int xi = 0; xi<width; ++xi)
		{
			H[row[xi]]++;
			
		}
	}
}
//计算流程
void calc_img() {

	int H[256];
	double totalP;//所有像素
	int T[256];
	vector<Mat> after;//保存均衡化之后的图像
	after.clear();
	for (int c = 0; c < ch; c++) {
	/*cout << c << endl;
	cout << channels.size() << endl;*/
		Mat tmp = channels[c];//取出一个通道
		Mat data;
		data.create(tmp.size(), tmp.type());
		calc_hist(tmp.data, tmp.cols, tmp.rows, tmp.step, H);
		cout << "For channel:" << c << endl;
		cout << "H:" << endl;
		for (int i = 0; i<256; i++)
			cout << H[i] << " ";
		cout << endl;
		totalP = tmp.rows*tmp.cols;
		int sum = 0;
		memset(T, 0, sizeof(T));

		for (int i = 0; i < 256; i++) {
			sum += H[i];
			T[i] = (sum*1.0) / totalP * 255.0;
		}
		cout << endl;
		cout << "T:" << endl;
		for (int i = 0; i<256; i++)
			cout << T[i] << " ";
		cout << endl;
		//像素映射
		uchar *pt = tmp.data;
		uchar *opt = data.data;
		for (int y = 0; y < tmp.rows; ++y) {
			for (int x = 0; x < tmp.cols; ++x) {
				const uchar *p = tmp.ptr(y, x);
				uchar *d = data.ptr(y, x);
				*d = T[p[0]];

			}
		}

		after.push_back(data);
	} 
	
	
	output.create(img.size(), img.type());
	merge(after, output);//合并各个通道的图像
}

int main()
{



	img = imread("timg.jpg", CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);//尽量保证无损
	cout << "**************实验三**************" << endl;
	//输出文件
	vector<int>compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_params.push_back(100);
	split(img, channels);

	ch = img.channels();
	//cout << ch << endl;
	if (channels.size()==0) {
		cout << "找不到文件" << endl;
		//char ch;
		//cin >> ch;
		
	}
	else {

		cout << "channels:" << ch << endl;

		calc_img();

		imshow("实验三原图", img);
		imshow("实验三均衡化", output);
	}
	imwrite("均衡化.jpg", output, compression_params);
	

	waitKey();
	return 0;
}