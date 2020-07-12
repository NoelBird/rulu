#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<vector>
#include<algorithm>
#include<random>
#include<time.h>
#include<math.h>

double loss(double y, double yHat)
{
	return 0.0;
}

double connected(std::vector<double> input, std::vector<double> weight)
{
	return 0.0;
}


int simple_logic_learner()
{
	// set weight
	std::vector<double> weight;
	srand((unsigned int)time(NULL));
	double randn = rand() / (double)RAND_MAX;
	weight.push_back(randn);
	randn = rand() / (double)RAND_MAX;
	weight.push_back(randn);
	randn = rand() / (double)RAND_MAX;
	weight.push_back(randn);

	for (int j = 0; j < 100; ++j)
	{
		double n1, n2, n3;
		if (j % 4 == 0)
			n1 = 0.0, n2 = 0.0, n3 = 0.0;
		else if (j % 4 == 1)
			n1 = 0.0, n2 = 1.0, n3 = 1.0;
		else if (j % 4 == 2)
			n1 = 1.0, n2 = 0.0, n3 = 1.0;
		else if (j % 4 == 3)
			n1 = 1.0, n2 = 1.0, n3 = 1.0;

		// set input
		std::vector<double> input;
		input.push_back(n1);
		input.push_back(n2);

		// set output
		double y;
		y = n3;

		// add one dimension to input because of bias
		input.push_back(1);

		// inner dot
		double dot=0;
		for (int i = 0; i < weight.size(); ++i)
		{
			dot += input[i] * weight[i];
		}

		// activation
		double yHat;

		double s1 = -dot;
		double s1Grad = -1.0;

		double s2 = exp(s1);
		double s2Grad = s2;

		double s3 = 1.0 + s2;
		double s3Grad = 1.0;

		double s4 = 1.0 / s3;
		double s4Grad = -(1.0 / pow(s3, 2));

		yHat = s4;

		std::cout << "[*] predict: " << yHat << std::endl;

		// calculate loss
		double loss;
		loss = 1.0 / 2.0 * pow(yHat - y, 2);
		double lossGrad = yHat - y;

		std::cout << "[*] loss: " << loss << std::endl;

		// update
		std::vector<double> yHatGrad;
		double lr = 10;
		for (int i = 0; i < input.size(); ++i)
		{
			yHatGrad.push_back(lossGrad * s4Grad * s3Grad * s2Grad * s1Grad * input[i]);
		}
		for (int i = 0; i < weight.size(); ++i)
		{
			std::cout << weight[i] << " ";
			weight[i] -= yHatGrad[i] * lr;
			std::cout << weight[i] << std::endl;
		}
	}

	

	return 0;
}