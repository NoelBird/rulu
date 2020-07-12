#pragma once

#include<string>

enum LayerType
{
	InputLayer,
	ConvLayer,
	FullyConnectedLayer,
	OutputLayer
};

class Layer
{
private:
	LayerType layerType;
	std::string name;
public:

};

class ConvLayer :Layer
{
	
};