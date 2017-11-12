#include <iostream>
#include <fstream>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <string>
//#include <direct.h>
#include "gainFoldname.h"
#include "readFilesNames.h"
#include "onMouseParam.h"

#define X_SCALE (0.3)
#define Y_SCALE (0.3)

using namespace cv;
using namespace std;

const string ImgProcessWin = "标注人脸框";
int key_for_switch;

static void onMouse(int,int,int,int,void*);

void main()
{
	string foldname;
	Mat copyImage;
	double normalization_x;
	double normalization_y;
	onMouseParam dataImage;
	vector<string> filenames;
	bool notargetFile = false;
	cout << "this program aims to do DataPreprocess for YOLO" << endl;
	foldname = gainFoldname();//String gainFoldname()返回当前exe所在文件夹路径
	//cout << "当前文件夹绝对路径输出：" << foldname << endl;
	if (!readFilesNames(foldname, filenames))cout << "there is no such file." << endl;//bool readImageFiles(const Mat& outImage,String foldname)在foldname指定的文件夹中按顺序读取图片，读取成功返回true，并且将图片路径增加至train.txt，读取失败返回false
	string txtname = foldname + "\\" + "train.txt";
	string labelname;
	ofstream traintxt(txtname);
	namedWindow(ImgProcessWin,WINDOW_AUTOSIZE);
	setMouseCallback(ImgProcessWin, onMouse, (void*)&dataImage);
	key_for_switch = 0;
	for (auto iter = filenames.begin(); iter != filenames.end(); ++iter)
	{
		//cout << *iter << endl;//
		dataImage.image = imread(foldname+"\\"+*iter);
		//cout << (*dataImage).image << endl;//
		//imshow("", (*dataImage).image);//
		//cout << 1234 << endl;//
		//Mat imgshow = dataImage.image;
		resize(dataImage.image, dataImage.image, Size(0, 0), X_SCALE, Y_SCALE);//调整输出的图片大小
		copyImage = ((dataImage).image).clone();
		normalization_x = 1.0 / (double)copyImage.cols;
		normalization_y = 1.0 / (double)copyImage.rows;
		imshow(ImgProcessWin, (dataImage).image);
		key_for_switch = waitKey(0);
		cout << "press r to redraw this pic\npress s for saving the rectangle points\npress esc fot exit." << endl;
		if (27 == key_for_switch)break;
		bool redraw = false;
		//int i;//
		switch (key_for_switch)
		{
		case 's':
			labelname = (*iter);
			labelname.pop_back();
			labelname.pop_back();
			labelname.pop_back();
			labelname.pop_back();
			_mkdir("label");
			labelname = foldname + "\\label\\" + labelname + ".txt";
			break;
		case 'S':
			labelname = (*iter);
			labelname.pop_back();
			labelname.pop_back();
			labelname.pop_back();
			labelname.pop_back();
			_mkdir("label");
			labelname = foldname + "\\label\\" + labelname + ".txt";
			break;
		default:
			(dataImage).image = copyImage.clone();
			--iter;
			redraw = true;
			dataImage.rect_leftup.clear();
			dataImage.rect_rightdown.clear();
			break;
		}
		if (redraw)continue;
		ofstream label(labelname);
		for (int a = 0; a < ((dataImage).rect_leftup).size(); ++a)
			label << 1 << " " << (((double)(((dataImage).rect_rightdown)[a].x) / 2.0 + ((double)(((dataImage).rect_leftup)[a].x)) / 2.0)*normalization_x) << " " << (((double)(((dataImage).rect_rightdown)[a].y) / 2.0 + (double)(((dataImage).rect_leftup)[a].y) / 2.0)*normalization_y) << " " << ((double)((((dataImage).rect_rightdown)[a].x) - ((((dataImage).rect_leftup)[a].x)))*normalization_x) << " " << ((double)((((dataImage).rect_rightdown)[a].y) - ((((dataImage).rect_leftup)[a].y)))*normalization_y) << endl;
		dataImage.rect_leftup.clear();
		dataImage.rect_rightdown.clear();
		traintxt << foldname + "\\" + *iter << endl;
		label.close();
	}
	traintxt.close();
	cout << "all of your images are processed,congratulation!" << endl;
}

static void onMouse(int event, int x, int y, int flag, void* param)
{
	static bool lbuttondown = false;
	static Point curpoint,firpoint;
	static Mat temp;
	switch (key_for_switch)
	{
	default:
		temp = (*((onMouseParam*)param)).image.clone();
	}
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		firpoint=Point(x, y);
		//cout << x << y << endl;//
		lbuttondown = true;
		break;
	case EVENT_LBUTTONUP:
		lbuttondown = false;
		curpoint=Point(x, y);
		imshow(ImgProcessWin, (*((onMouseParam*)param)).image);
		(*((onMouseParam*)param)).rect_leftup.push_back(firpoint);
		(*((onMouseParam*)param)).rect_rightdown.push_back(curpoint);
		rectangle((*((onMouseParam*)param)).image, firpoint, curpoint, Scalar(0, 0, 255), 1, 4, 0);
		imshow(ImgProcessWin, (*((onMouseParam*)param)).image);
		temp = (*((onMouseParam*)param)).image.clone();
		break;
	case EVENT_MOUSEMOVE:
		if (true == lbuttondown)
		{
			/*curpoint.x = x;
			curpoint.y = y;*/
			curpoint=Point(x, y);
			rectangle((*((onMouseParam*)param)).image, firpoint, curpoint, Scalar(0, 0, 255), 1, 4, 0);
			imshow(ImgProcessWin, (*((onMouseParam*)param)).image);
			(*((onMouseParam*)param)).image = temp.clone();
		}
		break;
	}
}