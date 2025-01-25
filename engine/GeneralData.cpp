#include "GeneralData.h"

    // 在类外进行静态成员变量的初始化
 unsigned int GeneralData::width = 1920;
 unsigned int GeneralData::height = 1080;
 float        GeneralData::near = 0.1f;
 float        GeneralData::far = 100.0f;
 float        GeneralData::speed = 1.0f;
 float        GeneralData::sensitivity = 0.05f;

    /*
    * 用于透视投影
    * aspect:宽高比
    * fovy:视野角度
    * 从aspect和fovy可以计算出left，right，bottom，top
    */
 float        GeneralData::aspect = 16.0f / 9.0f;
 float        GeneralData::fovy = glm::radians(45.0f);

    /*
    * 用于正交投影
    */
 float        GeneralData::left = -10.0f;
 float        GeneralData::right = 10.0f;
 float        GeneralData::bottom = -10.0f;
 float        GeneralData::top = 10.0f;

