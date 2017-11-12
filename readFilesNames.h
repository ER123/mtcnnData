#ifndef READFILESNAMES
#define READFILESNAMES
#define IMAGE_TYPE ("*.jpg")

#include <io.h>
#include <opencv2\opencv.hpp>
#include <string>

bool readFilesNames(const std::string& foldname,std::vector<std::string>& filenames);//在foldname指定的文件夹中按顺序读取图片，读取成功返回true，并且将图片路径增加至train.txt，读取失败返回false

#endif