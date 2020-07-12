#pragma once

#include<opencv2/opencv.hpp>
#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>

class Image
{
private:
	int mRows = 0;
	int mCols = 0;
	int mChannels = 0;
	int mType = 0;
	int mStep = 0;
	std::vector<double> mData;
public:
	Image() {};
	// Image(const Image& ref):mMat(ref.mMat) {};
	void load(const char* filename);
	void show(const char* title);

	int getChannels();
	int getRows();
	int getCols();
	cv::Mat getMat();

	int getPixel(int x, int y, int channel);
	void setPixel(int x, int y, int channel, uchar val);

	Image copy();
	void normalize();
	void threshold(int threshold);
	void subtract(Image img);
	void rotate180();
	void upsample(int stride);
	void random(int rows, int cols, int channels);
	void simpleConv();

	// 레이어 함수
	// TODO: 새로운 class로 분할하기
	void showLayer(const char* name);
	Image getLayer(int layer);
};

class Kernel:Image
{
private:
	Image mImg;
public:
	void random(int size, int channels);
	void show(const char* title);
	void update();

	void conv2d(int mc, Kernel kernel, int kc, int stride, Image out, int oc);
};