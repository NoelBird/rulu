#define _CRT_SECURE_NO_WARNINGS
#include"image.hpp"
#include<Windows.h>
#include <direct.h>

void Image::load(const char* filename)
{
	cv::Mat matOrigin;
	
	matOrigin = cv::imread(filename);
	char buff[FILENAME_MAX];
	_getcwd(buff, FILENAME_MAX);
	std::cout << buff << std::endl;
	
	if (matOrigin.empty())
	{
		std::cout << "Image file cannot be loaded!" << std::endl;
		exit(1);
	}
	mChannels = matOrigin.channels();
	mType = matOrigin.type();
	mRows = matOrigin.rows;
	mCols = matOrigin.cols;
	mStep = (int)matOrigin.step[0];

	int dataLen = mChannels * mRows * mCols;


	// WARNING: forth parameter has to be 0.0 not 0
	// overflow 가능성 있음
	mData.assign(mRows * mCols * mChannels, 0.0);

	int count = 0;

	for (int k = 0; k < mChannels; ++k) {
		for (int i = 0; i < mRows; ++i) {
			for (int j = 0; j < mCols; ++j) {
				mData[count++] = matOrigin.data[i * mStep + j * mChannels + k];
			}
		}
	}

	matOrigin.release();
}

void Image::show(const char* title)
{
	int dataLen = mChannels*mRows*mCols;

	cv::Mat out = cv::Mat(mRows, mCols, mType, 0.0);

	int count = 0;
	for (int k = 0; k < mChannels; ++k) {
		for (int i = 0; i < mRows; ++i) {
			for (int j = 0; j < mCols; ++j) {
				out.data[i * mStep + j * mChannels + k] = (uchar)mData[count++];
			}
		}
	}

	cv::imshow(title, out);
	out.release();
}

int Image::getChannels()
{
	return mChannels;
}
int Image::getRows()
{
	return mRows;
}
int Image::getCols()
{
	return mCols;
}

//int Image::getPixel(int x, int y, int c)
//{
//	assert(x < mMat.cols && y < mMat.rows && c < mMat.channels());
//	return mMat.data[c * mMat.rows * mMat.cols + y * mMat.cols + x];
//}
//
//void Image::setPixel(int x, int y, int c, uchar val)
//{
//	int rows = mMat.rows;
//	int cols = mMat.cols;
//	int channels = mMat.channels();
//	mMat.data[x + y*cols + c*rows*cols] = val;
//}
//
//Image Image::copy()
//{
//		Image copy(*this);
//		return copy;
//}
//
//void Image::normalize()
//{
//	int channels = mMat.channels();
//	int rows = mMat.rows;
//	int cols = mMat.cols;
//
//	double* min = new double[channels];
//	double* max = new double[channels];
//	int i, j;
//	for (i = 0; i < channels; ++i) min[i] = max[i] = mMat.data[i * rows * cols];
//
//	for (j = 0; j < channels; ++j) {
//		for (i = 0; i < rows * cols; ++i) {
//			double v = mMat.data[i + j * rows * cols];
//			if (v < min[j]) min[j] = v;
//			if (v > max[j]) max[j] = v;
//		}
//	}
//	for (i = 0; i < channels; ++i) {
//		if (max[i] - min[i] < .00001) {
//			min[i] = 0;
//			max[i] = 1;
//		}
//	}
//	for (j = 0; j < channels; ++j) {
//		for (i = 0; i < cols * rows; ++i) {
//			mMat.data[i + j * rows * cols] = (uchar)(mMat.data[i + j * rows * cols] - min[j]) / (max[j] - min[j])*255;
//		}
//	}
//
//	delete[] min;
//	delete[] max;
//}
//
//void Image::threshold(int threshold)
//{
//	int channels = mMat.channels();
//	int rows = mMat.rows;
//	int cols = mMat.cols;
//	for (int i = 0; i < cols * rows * channels; ++i) {
//		if (mMat.data[i] < threshold) mMat.data[i] = 0;
//	}
//}
//
//void Image::subtract(Image img)
//{
//	int rows = mMat.rows;
//	int cols = mMat.cols;
//	int channels = mMat.channels();
//
//	cv::Mat _mat;
//	if (mMat.size() != img.mMat.size())
//	{
//		cv::resize(img.mMat, _mat, cv::Size(cols, rows), 0, 0, cv::InterpolationFlags::INTER_NEAREST);
//	}
//	else {
//		_mat = img.mMat ;
//	}
//	for (int i = 0; i < rows * cols * channels; ++i) mMat.data[i] -= _mat.data[i];
//}
//
void Image::rotate180()
{
	int size = mRows * mCols;
	int half = size / 2;
	for (int j = 0; j < mChannels; ++j) {
		for (int i = 0; i < half; ++i) {
			double swap = mData[j * size + i];
			mData[j * size + i] = mData[j * size + (size - 1 - i)];
			mData[j * size + (size - 1 - i)] = swap;
		}
	}
}

//void Image::upsample(int stride)
//{
//	Image tmp;
//	tmp.mMat = cv::Mat(mMat.rows * stride, mMat.cols * stride, mMat.type(), 0.0);
//	int channels = tmp.mMat.channels();
//	int rows = tmp.mMat.rows;
//	int cols = tmp.mMat.cols;
//	for (int k = 0; k < channels; ++k) {
//		for (int i = 0; i < cols; ++i) {
//			for (int j = 0; j < rows; ++j) {
//				double val = getPixel(i/stride, j/stride, k);
//				tmp.setPixel(i, j, k, val);
//			}
//		}
//	}
//	mMat = tmp.mMat.clone();
//	tmp.mMat.release();
//}
//
void Image::random(int rows, int cols, int channels)
{
	std::vector<double> data(rows*cols*channels, 0.0);
	for (int i = 0; i < rows * cols * channels; ++i) {
		data[i] = (double)rand() / RAND_MAX;
	}
	mRows = rows;
	mCols = cols;
	mChannels = channels;
	mData.swap(data);
}

////void Image::conv2d(int mc, Kernel kernel, int kc, int stride, Image out, int oc)
////{
////	for (int x = 0; x < m.h; x += stride) {
////		for (int y = 0; y < m.w; y += stride) {
////			double sum = 0;
////			for (int i = 0; i < kernel.h; ++i) {
////				for (int j = 0; j < kernel.w; ++j) {
////					sum += getPixel(kernel, i, j, kc) * get_pixel_extend(m, x + i - kernel.h / 2, y + j - kernel.w / 2, mc);
////				}
////			}
////			add_pixel(out, x / stride, y / stride, oc, sum);
////		}
////	}
////	return;
////}
//
void Image::showLayer(const char* name)
{
	char buff[256];
	for (int i = 0; i < mChannels; ++i) {
		sprintf(buff, "%s - Layer %d", name, i);
		Image layer = getLayer(i);

		int dataLen = mChannels * mRows * mCols;

		cv::Mat out = cv::Mat(mRows, mCols, mType, 0.0);

		int count = 0;
		for (int k = 0; k < mChannels; ++k) {
			for (int i = 0; i < mRows; ++i) {
				for (int j = 0; j < mCols; ++j) {
					out.data[i * mStep + j * mChannels + k] = mData[count++];
				}
			}
		}

		cv::imshow(buff, out);
		out.release();
	}
}

Image Image::getLayer(int layer)
{
	Image out;
	out.mData.assign(mRows*mCols, 0.0);
	for (int i = 0; i < mRows * mCols; ++i) {
		out.mData[i] = mData[i + layer * mRows * mCols];
	}
	return out;
}

void Image::simpleConv()
{
	std::vector<double> kernel(9, 0.0);

	kernel[0] = 1./9;
	kernel[1] = 1./9;
	kernel[2] = 1./9;
	kernel[3] = 1./9;
	kernel[4] = 1./9;
	kernel[5] = 1./9;
	kernel[6] = 1. / 9;
	kernel[7] = 1. / 9;
	kernel[8] = 1. / 9;

	std::vector<double> update(mRows * mCols * mChannels, 0.0);

	for (int m = 0; m < mChannels; ++m)
	{
		for (int i = 0; i < mRows; ++i)
		{
			for (int j = 0; j < mCols; ++j)
			{
				double sum = 0;
				for (int k = -1; k < 2; ++k)
				{
					for (int l = -1; l < 2; ++l)
					{
						if (i + k < 0 || i + k >= mRows) continue;
						if (j + l < 0 || j + l >= mCols) continue;
						sum += mData[m*mRows*mCols+(i+k)*mCols+(j+l)] * kernel[(k + 1) * 3 + (l + 1)];
					}
				}
				update[m * mRows * mCols + i * mCols + j] = sum;
			}
		}
	}
	
	mData.swap(update);

}


//void Kernel::random(int size, int channels)
//{
//	int pad;
//	if ((pad = (size % 2 == 0))) ++size;
//	mImg.random(size, size, channels);
//	if (pad) {
//		for (int k = 0; k < channels; ++k) {
//			for (int i = 0; i < size; ++i) {				
//				mImg.setPixel(i, 0, k, 0);
//				mImg.setPixel(0, i, k, 0);
//			}
//		}
//	}
//}
//
//void Kernel::show(const char* title)
//{
//	mImg.show(title);
//}