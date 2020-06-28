#define _CRT_SECURE_NO_WARNINGS
#include"image.hpp"

void Image::load(const char* filename)
{
	cv::Mat matOrigin;
	
	matOrigin = cv::imread(filename);
	if (matOrigin.empty())
	{
		std::cout << "Image file cannot be loaded!" << std::endl;
		exit(1);
	}
	int channels = matOrigin.channels();
	int type = matOrigin.type();
	int rows = matOrigin.rows;
	int cols = matOrigin.cols;
	int step = (int)matOrigin.step[0];

	int dataLen = channels * rows * cols;

	

	// WARNING: forth parameter has to be 0.0 not 0
	mMat = cv::Mat(rows, cols, type, 0.0);

	int count = 0;

	for (int k = 0; k < channels; ++k) {
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				mMat.data[count++] = matOrigin.data[i * step + j * channels + k];
			}
		}
	}

	matOrigin.release();
}

void Image::show(const char* title)
{
	int channels = mMat.channels();
	int type = mMat.type();
	int rows = mMat.rows;
	int cols = mMat.cols;
	int step = (int)mMat.step[0];

	int dataLen = channels * rows * cols;

	cv::Mat out = cv::Mat(rows, cols, type, 0.0);

	int count = 0;
	for (int k = 0; k < channels; ++k) {
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				out.data[i * step + j * channels + k] = mMat.data[count++];
			}
		}
	}

	cv::imshow(title, out);
	out.release();
}

int Image::getChannels()
{
	return mMat.channels();
}
int Image::getRows()
{
	return mMat.rows;
}
int Image::getCols()
{
	return mMat.cols;
}

cv::Mat Image::getMat()
{
	return mMat;
}

int Image::getPixel(int x, int y, int c)
{
	assert(x < mMat.cols && y < mMat.rows && c < mMat.channels());
	return mMat.data[c * mMat.rows * mMat.cols + y * mMat.cols + x];
}

void Image::setPixel(int x, int y, int c, uchar val)
{
	int rows = mMat.rows;
	int cols = mMat.cols;
	int channels = mMat.channels();
	mMat.data[x + y*cols + c*rows*cols] = val;
}

Image Image::copy()
{
		Image copy(*this);
		return copy;
}

void Image::normalize()
{
	int channels = mMat.channels();
	int rows = mMat.rows;
	int cols = mMat.cols;

	double* min = new double[channels];
	double* max = new double[channels];
	int i, j;
	for (i = 0; i < channels; ++i) min[i] = max[i] = mMat.data[i * rows * cols];

	for (j = 0; j < channels; ++j) {
		for (i = 0; i < rows * cols; ++i) {
			double v = mMat.data[i + j * rows * cols];
			if (v < min[j]) min[j] = v;
			if (v > max[j]) max[j] = v;
		}
	}
	for (i = 0; i < channels; ++i) {
		if (max[i] - min[i] < .00001) {
			min[i] = 0;
			max[i] = 1;
		}
	}
	for (j = 0; j < channels; ++j) {
		for (i = 0; i < cols * rows; ++i) {
			mMat.data[i + j * rows * cols] = (uchar)(mMat.data[i + j * rows * cols] - min[j]) / (max[j] - min[j])*255;
		}
	}

	delete[] min;
	delete[] max;
}

void Image::threshold(int threshold)
{
	int channels = mMat.channels();
	int rows = mMat.rows;
	int cols = mMat.cols;
	for (int i = 0; i < cols * rows * channels; ++i) {
		if (mMat.data[i] < threshold) mMat.data[i] = 0;
	}
}

void Image::subtract(Image img)
{
	int rows = mMat.rows;
	int cols = mMat.cols;
	int channels = mMat.channels();

	cv::Mat _mat;
	if (mMat.size() != img.mMat.size())
	{
		cv::resize(img.mMat, _mat, cv::Size(cols, rows), 0, 0, cv::InterpolationFlags::INTER_NEAREST);
	}
	else {
		_mat = img.mMat ;
	}
	for (int i = 0; i < rows * cols * channels; ++i) mMat.data[i] -= _mat.data[i];
}

void Image::rotate180()
{
	int channels = mMat.channels();
	int rows = mMat.rows;
	int cols = mMat.cols;
	for (int j = 0; j < channels; ++j) {
		for (int i = 0; i < rows * cols / 2; ++i) {
			double swap = mMat.data[j * rows * cols + i];
			mMat.data[j * rows * cols + i] = mMat.data[j * rows * cols + (rows * cols - 1 - i)];
			mMat.data[j * rows * cols + (rows * cols - 1 - i)] = swap;
		}
	}
}

void Image::upsample(int stride)
{
	Image tmp;
	tmp.mMat = cv::Mat(mMat.rows * stride, mMat.cols * stride, mMat.type(), 0.0);
	int channels = tmp.mMat.channels();
	int rows = tmp.mMat.rows;
	int cols = tmp.mMat.cols;
	for (int k = 0; k < channels; ++k) {
		for (int i = 0; i < cols; ++i) {
			for (int j = 0; j < rows; ++j) {
				double val = getPixel(i/stride, j/stride, k);
				tmp.setPixel(i, j, k, val);
			}
		}
	}
	mMat = tmp.mMat.clone();
	tmp.mMat.release();
}

void Image::random(int rows, int cols, int channels)
{
	mMat = cv::Mat(rows, cols, CV_8UC3, channels);
	for (int i = 0; i < rows * cols * channels; ++i) {
		mMat.data[i] = ((double)rand() / RAND_MAX)*255;
	}
}

void Image::showLayer(const char* name)
{
	int channels = mMat.channels();
	char buff[256];
	for (int i = 0; i < channels; ++i) {
		sprintf(buff, "%s - Layer %d", name, i);
		Image layer = getLayer(i);

		int channels = layer.mMat.channels();
		int type = layer.mMat.type();
		int rows = layer.mMat.rows;
		int cols = layer.mMat.cols;
		int step = (int)layer.mMat.step[0];

		int dataLen = channels * rows * cols;

		cv::Mat out = cv::Mat(rows, cols, type, 0.0);

		int count = 0;
		for (int k = 0; k < channels; ++k) {
			for (int i = 0; i < rows; ++i) {
				for (int j = 0; j < cols; ++j) {
					out.data[i * step + j * channels + k] = layer.mMat.data[count++];
				}
			}
		}

		cv::imshow(buff, out);
		out.release();
	}
}

Image Image::getLayer(int layer)
{
	int rows = mMat.rows;
	int cols = mMat.cols;
	int channels = 1;
	Image out;
	out.mMat = cv::Mat(rows, cols, CV_8UC1);
	int i;
	for (i = 0; i < rows * cols; ++i) {
		out.mMat.data[i] = mMat.data[i + layer * rows * cols];
	}
	return out;
}

void Kernel::random(int size, int channels)
{
	int pad;
	if ((pad = (size % 2 == 0))) ++size;
	mImg.random(size, size, channels);
	if (pad) {
		for (int k = 0; k < channels; ++k) {
			for (int i = 0; i < size; ++i) {				
				mImg.setPixel(i, 0, k, 0);
				mImg.setPixel(0, i, k, 0);
			}
		}
	}
}

void Kernel::show(const char* title)
{
	mImg.show(title);
}