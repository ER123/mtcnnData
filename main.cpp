#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;
using namespace cv;

Mat srcImage, dstImage, copy_Image, temp_Image;
String Picture_Name;
vector<string> filePath;
int num_pic_name = 0;
string save_path = "C:\\Users\\salaslyrin\\Desktop\\faceDataset\\EastStation\\trainPics\\select\\";

void on_mouse(int event, int x, int y, int flags, void *ustc)  
{
	static Point pre_pt = (-1, -1); 
	static Point cur_pt = (-1, -1);
	char temp[16];
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		srcImage.copyTo(copy_Image); 
		sprintf(temp, "(%d,%d)", x, y);
		pre_pt = Point(x, y);
		putText(copy_Image, temp, pre_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);  
		circle(copy_Image, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0); 
		imshow("img", copy_Image);
	}
	else if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))
	{
		copy_Image.copyTo(temp_Image);
		sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);
		putText(temp_Image, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255)); 
		imshow("img", temp_Image);
	}
	else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))  
	{
		copy_Image.copyTo(temp_Image);
		sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);
		putText(temp_Image, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));  
		rectangle(temp_Image, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);  
		imshow("img", temp_Image);
	}
	else if (event == CV_EVENT_LBUTTONUP)  
	{
		srcImage.copyTo(copy_Image);
		sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);
		putText(copy_Image, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
		circle(copy_Image, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
		rectangle(copy_Image, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);
		imshow("img", copy_Image);
		copy_Image.copyTo(temp_Image);
		int width = abs(pre_pt.x - cur_pt.x);
		int height = abs(pre_pt.y - cur_pt.y);
		if (width == 0 || height == 0)
		{
			printf("width == 0 || height == 0");
			return;
		}
		dstImage = srcImage(Rect(min(cur_pt.x, pre_pt.x), min(cur_pt.y, pre_pt.y), width, height));
		namedWindow("dst");
		imshow("dst", dstImage);
		waitKey(0);

		char c = waitKey(0);  
		if (c == 's')
		{
			Picture_Name = save_path + to_string(num_pic_name) + ".jpg";
			imwrite(Picture_Name, dstImage);
			/*String Name = Picture_Name.substr(0, Picture_Name.length() - 4);
			imshow(Name, dstImage);
			waitKey(0);*/
			num_pic_name++;
		}
		else
			cout << "未对选取图像进行保存" << endl;
		waitKey(0);
	}
}

vector<string> readLabel(string labelPath)
{
	vector<string> path;
	ifstream infile;
	infile.open(labelPath.data());
	assert(infile.is_open());

	string s, str;
	while (!infile.eof())
	{
		getline(infile, s);

		int idx = 0;
		if (!s.empty())
		{
			stringstream ss(str);
			ss << s;

			while (getline(ss, str, ' '))
			{
				if (idx == 0)
				{
					path.push_back(str);
				}
				break;
			}
		}
		
	}
	return path;
}

void main()
{
	string labelPath = "C:\\Users\\salaslyrin\\Desktop\\faceDataset\\EastStation\\labels.txt";
	filePath = readLabel(labelPath);

	for (int i = 0; i < filePath.size()-1; i++)
	{
		srcImage = imread(filePath[i]);
		srcImage.copyTo(copy_Image);
		namedWindow("img");
		imshow("img", copy_Image);
		setMouseCallback("img", on_mouse, 0);
		waitKey(0);
	}
	
}
