#ifndef READFILESNAMES
#define READFILESNAMES
#define IMAGE_TYPE ("*.jpg")

#include <io.h>
#include <opencv2\opencv.hpp>
#include <string>

bool readFilesNames(const std::string& foldname,std::vector<std::string>& filenames);//��foldnameָ�����ļ����а�˳���ȡͼƬ����ȡ�ɹ�����true�����ҽ�ͼƬ·��������train.txt����ȡʧ�ܷ���false

#endif