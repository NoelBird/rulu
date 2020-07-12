#include<iostream>
#include"image.hpp"

extern "C" {
void testColor()
{
	Image dog;
	dog.load("test_color.png");
	dog.show("Test Color");
	cv::waitKey(0);
}


	__declspec(dllexport) void testLoad()
	{
		Image dog;
		dog.load("dog.jpg");
		dog.show("Test Load");
		dog.showLayer("Test Load");
		cv::waitKey(0);
		cv::destroyAllWindows();
	}


	__declspec(dllexport) void testConv()
{
	Image dog;
	dog.load("dog.jpg");
	dog.simpleConv();
	dog.show("Test Load");
}

void testRotate()
{
	Image dog;
	dog.load("dog.jpg");
	clock_t start = clock(), end;
	for (int i = 0; i < 1001; ++i) {
		dog.rotate180();
	}
	end = clock();
	printf("Rotations: %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
	dog.show("Test Rotate");
	cv::waitKey(0);

	Image random;
	random.random(3, 3, 3);
	random.show("Test Rotate Random");
	random.rotate180();
	random.show("Test Rotate Random");
	random.rotate180();
	random.show("Test Rotate Random");
	cv::waitKey(0);
}
//
//void testUpsample()
//{
//	Image dog;
//	dog.load("dog.jpg");
//	
//	int n = 2;
//
//	dog.upsample(n);
//	dog.show("Test Upsample");
//	dog.showLayer("Test Upsample");
//}

int main()
{
	// testLoad();
	testConv();
	// testRotate();
	// testUpsample();
	
	// testColor();
	//test_backpropagate();
	//test_convolve();
	//test_network();
	//test_convolutional_layer();
	
	// Image img;
	// img.random(224, 244, 3);
	// img.load("test_hinton.jpg");
	// img.rotate180();
	// img.show("test hinton");
	// img.showLayer("layer");
	/*Kernel knl;
	knl.random(224, 3);
	knl.show("hello");*/

	
	//Image img2(img);
	// img2 = img.copy();
	//img2.load("test_dog.jpg");
	// img2.subtract(img);
	// img2.threshold(128);
	// img2.normalize();
	// img2.show("normalized");
	
	cv::waitKey(0);
	return 0;
}
}