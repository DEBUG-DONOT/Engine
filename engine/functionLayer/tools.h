#pragma once
#include<vector>
class m_Tools
{
    public:
    static std::vector<float> GenGussianBlurWeight(int radius,float sigma);
    static std::vector<float> GenGussianBlurWeightOneDimension(int radius,float sigma);
    
    
};