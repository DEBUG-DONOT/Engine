#include"tools.h"
#include<cmath>
std::vector<float> Tools::GenGussianBlurWeight(int radius,float sigma)
{
    double M_PI = 3.14159265358979323846;
    int size = 2 * radius + 1;
    std::vector<float> kernel(size*size);
    double sum = 0.0;

    // 1. 计算未归一化的权重值
    for (int y = -radius; y <= radius; ++y) 
    {
        for (int x = -radius; x <= radius; ++x) 
        {
            // 二维高斯函数公式[1,3,8](@ref)
            double weight = (1.0 / (2 * M_PI * sigma * sigma)) * 
                            exp(-(x*x + y*y) / (2 * sigma * sigma));
            //kernel[y + radius][x + radius] = weight;
            kernel[(y+radius)*size + (x+radius)] = weight;
            sum += weight;
        }
    }

    // 2. 归一化处理（确保权重总和为1）[3,5,8](@ref)
    for (int i = 0; i < size; ++i) 
    {
        for (int j = 0; j < size; ++j) 
        {
            //kernel[i][j] /= sum;
            kernel[i*size + j] /= sum;
        }
    }
    return kernel;

}