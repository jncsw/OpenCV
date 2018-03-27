//#include <opencv2/opencv.hpp>
//#include <iostream>
//
//using namespace std;
//using namespace cv;
//
//float bilinear(float a, float b, float c, float d, float dx, float dy)
//{
//	float h1 = a + (b - a)*dx;
//	float h2 = c + (d - c)*dx;
//	return h1 + (h2 - h1)*dy;
//}
//
//void zoom(const Mat &img, Mat &zimg, float zx, float zy)
//{
//	int rows = (int)img.rows*zy;
//	int cols = (int)img.cols*zx;
//	//int rows = 300;
//	//int cols = 300;
//	zimg.create(rows, cols, CV_8UC3);
//	cout << zimg.rows << "  " << zimg.cols << endl;
//	cout << img.rows << "  " << img.cols << endl;
//	for (int i = 0; i < rows; i++)
//	{
//		for (int j = 0; j < cols; j++)
//		{
//			float x = j / zx;
//			float y = i / zy;
//			int x1 = (int)x;
//			int y1 = (int)y;
//			if (x1 >= img.cols - 1) x1--;
//			if (y1 >= img.rows - 1) y1--;
//			//cout << i << " " << j << " " << x << " " << y << " " << img.rows << " " << img.cols << endl;
//			//cout <<"first"<< i << " " << j << " " << x1 << " " << y1 << endl;
//			const uchar *p1 = img.ptr(y1, x1);
//			const uchar *p2 = img.ptr(y1, x1 + 1);
//			const uchar *p3 = img.ptr(y1 + 1, x1);
//			const uchar *p4 = img.ptr(y1 + 1, x1 + 1);
//			uchar *z = zimg.ptr(i, j);
//			//cout << i << " " << j << " " << x1 << " " << y1<< endl;
//			for (int k = 0; k < 3; k++)
//			{
//				//cout<<"___________________" << p1[k] << endl;
//				z[k] = bilinear(p1[k], p2[k], p3[k], p4[k], x - (int)x1, y - int(y1));
//			}
//		}
//	}
//
//}
//
//void rotate(const Mat &img, Mat &rimg)
//{
//	int cols = img.cols;
//	int rows = img.rows;
//	rimg.create(rows, cols, CV_8UC3);
//	cout << rimg.rows << "  " << rimg.cols << endl;
//	cout << img.rows << "  " << img.cols << endl;
//	float x1 = 0, y1 = 0;
//	float x = 0, y = 0;
//	for (int i = 0; i < rows; i++)
//	{
//		for (int j = 0; j < cols; j++)
//		{
//			x1 = (j - cols / 2) / ((float)cols / 2);
//			y1 = (i - rows / 2) / ((float)rows / 2);
//			float r = sqrt(x1*x1 + y1*y1);
//			float heta = (1 - r)*(1 - r);
//			if (r >= 1)
//			{
//				x = x1;
//				y = y1;
//			}
//			else
//			{
//				x = cos(heta)*x1 - sin(heta)*y1;
//				y = sin(heta)*x1 + cos(heta)*y1;
//			}
//			//cout << "first" << r << " " << x << " " << y << endl;
//			x = x*cols / 2 + (float)cols / 2;
//			y = y*rows / 2 + (float)rows / 2;
//
//			//cout << i << " " << j << " " << y << " " << x<<endl;
//			x1 = (int)x;
//			y1 = (int)y;
//			if (x1 >= img.cols - 1) x1--;
//			if (y1 >= img.rows - 1) y1--;
//			const uchar *p1 = img.ptr(y1, x1);
//			const uchar *p2 = img.ptr(y1, x1 + 1);
//			const uchar *p3 = img.ptr(y1 + 1, x1);
//			const uchar *p4 = img.ptr(y1 + 1, x1 + 1);
//			uchar *p = rimg.ptr(i, j);
//			for (int k = 0; k < 3; k++)
//			{
//				p[k] = bilinear(p1[k], p2[k], p3[k], p4[k], x - x1, y - y1);
//			}
//		}
//	}
//}
//
//
//void E2zoom()
//{
//	Mat img = imread("ÃÉÄÈÀöÉ¯.jpg");
//	Mat zimg;
//	zoom(img, zimg, 5, 5);
//	cout << "finish" << endl;
//	imshow("ÃÉÄÈÀöÉ¯", zimg);
//}
//
//void E2rotate()
//{
//	Mat img = imread("ÃÉÄÈÀöÉ¯.jpg");
//	Mat rimg;
//	rotate(img, rimg);
//	imshow("Ðý×ª", rimg);
//
//}
//int main(int argc, char* argv[])
//{
//	E2zoom();
//	E2rotate();
//	waitKey(0);
//	return 0;
//}