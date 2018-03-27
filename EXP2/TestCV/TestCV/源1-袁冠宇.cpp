//#include<iostream>  
//#include <opencv2/core/core.hpp>  
//#include <opencv2/highgui/highgui.hpp>  
//#include <highgui.h>
//#include <cv.h>
//
//
//using namespace cv;
//using namespace std;
//
//void linearInterrupt(const Mat &src, Mat &dst, float width, float height) {
//
//	float scale_x = (float)(width);		//src/dst
//	float scale_y = (float)(height);
//	float x0, y0, x1, y1, x2, y2;
//	float fx1, fy1, fx2, fy2;
//	dst.create((int)(src.rows / scale_y), (int)(src.cols / scale_x), CV_8UC3);	//
//
//	for (int j = 0; j < dst.rows - 1; j++) {
//		y0 = j*scale_y;
//		y1 = (int)y0;
//		if (y1 == dst.rows - 1) {
//			y2 = y1;
//		}
//		else y2 = y1 + 1.0;
//
//
//		fy1 = y0 - y1;
//		fy2 = 1.0f - fy1;
//
//
//		///*	cout << "i: " << i << "\n";*/
//		//	cout << "j: " << j << "\n";
//		//	cout << "x0: " << x0 << "\n"; ////
//		//	cout << "x1: " << x1 << "\n";
//		//	cout << "fx1: " << fx1 << "\n";
//		//	cout << "fx2: " << fx2 << "\n";
//		//	cout << "y0: " << y0 << "\n"; ////
//		//	cout << "y1: " << y1 << "\n";
//		//	cout << "width: " << (int)(src.cols / scale_x) << "\n";
//		//	cout << "width0: " << src.cols << "\n";
//		//	cout << "height: " << (int)(src.rows / scale_y) << "\n";
//		//	cout << "height0: " << src.rows << "\n";
//		//	cout << "\n" << "\n";
//
//		for (int i = 0; i < dst.cols - 1; i++) {
//			x0 = i*scale_x;
//			x1 = (int)x0;
//			if (x1 == dst.cols - 1) { x2 = x1; }
//			else x2 = x1 + 1.0;
//
//			fx1 = x0 - x1;
//			fx2 = 1.0f - fx1;
//
//
//
//			float s1 = fx1*fy1;
//			float s2 = fx2*fy1;
//			float s3 = fx2*fy2;
//			float s4 = fx1*fy2;
//
//
//
//			dst.at
//
//				<Vec3b>(j, i)[0] = src.at
//
//				<Vec3b>(y1, x1)[0] * s3 + src.at
//
//				<Vec3b>(y1, x2)[0] * s4 + src.at
//
//				<Vec3b>(y2, x1)[0] * s2 + src.at
//
//				<Vec3b>(y2, x2)[0] * s1;
//			dst.at
//
//				<Vec3b>(j, i)[1] = src.at
//
//				<Vec3b>(y1, x1)[1] * s3 + src.at
//
//				<Vec3b>(y1, x2)[1] * s4 + src.at
//
//				<Vec3b>(y2, x1)[1] * s2 + src.at
//
//				<Vec3b>(y2, x2)[1] * s1;
//			dst.at
//
//				<Vec3b>(j, i)[2] = src.at
//
//				<Vec3b>(y1, x1)[2] * s3 + src.at
//
//				<Vec3b>(y1, x2)[2] * s4 + src.at
//
//				<Vec3b>(y2, x1)[2] * s2 + src.at
//
//				<Vec3b>(y2, x2)[2] * s1;
//		}
//	}
//	namedWindow("2.1");
//	imshow("2.1", dst);
//
//}
//
//
//int main()
//{
//	Mat src;
//	src = imread("exp2.jpg");
//	Mat img;
//	linearInterrupt(src, img, 0.7, 0.7);
//
//	int a;
//	cin >> a;
//	return 0;
//}
//
