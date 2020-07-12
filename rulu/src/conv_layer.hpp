#pragma once

#include<vector>
#include<algorithm>
#include "image.hpp"


class ConvLayer
{
private:
    int n;
    int stride;
    std::vector<Kernel> kernels;
    std::vector<Kernel> kernelUpdates;
    Image upsampled;
    Image output;
public:
    ConvLayer make(int w, int h, int c, int n, int size, int stride);
    void run(const Image input, const ConvLayer layer);
    void backpropagateLayer(Image input, ConvLayer layer);
    void backpropagateLayerConvolve(Image input, ConvLayer layer);
};


