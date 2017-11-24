#include "network.h"
#include "mtcnn.h"
#include "read_rect.h"
#include "write_res.h"
#include <time.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <opencv2/video/video.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#pragma comment(lib, "libopenblas.dll.a")

using namespace cv;

string label_name = "E:\\EastStation\\labels.txt";
string res_path = "E:\\EastStation\\res_labels.txt";

int box_area(box b)
{
	return (b.x2 - b.x1)*(b.y2 - b.y1);
}

float calc_iou(Rect rect, vector<box> gt)
{
	cout << "rect:" << rect.x << " " << rect.y << " " << rect.x + rect.width << " " << rect.y + rect.height << endl;
	vector<float> iou;
	for (int i = 0; i < gt.size(); i++)
	{
		//cout << "rect:" << rect.x << " " << rect.y << " " << rect.x + rect.width << " " << rect.y + rect.height << endl;
		//cout << "gt[i]:" << gt[i].x1 << " " << gt[i].x2 << " " << gt[i].y1 << " " << gt[i].y2 << endl;

		int minX = (rect.x + rect.width < gt[i].x2) ? rect.x + rect.width : gt[i].x2;
		int minY = (rect.y + rect.height < gt[i].y2) ? rect.y + rect.height : gt[i].y2;
		int maxX = (rect.x > gt[i].x1) ? rect.x : gt[i].x1;
		int maxY = (rect.y > gt[i].y1) ? rect.y : gt[i].y1;

		int iWidth = (minX - maxX + 1)>0 ? (minX - maxX + 1) : 0;
		int iHeight = (minY - maxY + 1) > 0 ? (minY - maxY + 1) : 0;

		int iArea = iHeight * iWidth;
		int uArea = rect.width * rect.height + box_area(gt[i]) - iArea;

		//cout << "minX:" << minX << " minY:" << minY << " maxX:" << maxX << " maxY:" << maxY << endl;
		//cout << "iWidth:" << iWidth << " iHeght:" << iHeight << endl;
		//cout << "iArea:" << iArea << " uArea:" << uArea << endl;
		//cout << endl;

		float iou_ = (float)iArea / (float)uArea;
		cout <<"iou_:" <<iou_ << endl;
		iou.push_back(iou_);
	}
	cout << endl;
	sort(iou.begin(), iou.end());
	for (int i = 0; i < iou.size(); i++)
	{
		cout << iou[i] <<" ";
	}
	cout << endl;
	cout << endl;
	return iou[iou.size()-1];
}

int main()
{
	vector<gt_box> gt = read_label(label_name);

	vector<vector<Point2f>> landmarks;
	vector<Rect> objs;
	Mat im;
	int count_num = 0;
	for (auto picture = gt.begin(); picture != gt.end(); count_num++, picture++)
	{
		if (count_num % 100 == 0)
		{
			cout << count_num << " pictures done!" << endl;
		}
		im = imread(picture->img_path);
		string img_path = picture->img_path;

		if (im.rows > 1000)resize(im, im, Size(round(800 * ((double)im.cols / (double)im.rows)), 800), 0, 0);
		if (im.cols > 1500)resize(im, im, Size(800, round(800 * ((double)im.rows / (double)im.cols))), 0, 0);
		mtcnn find(im.cols, im.rows);

		objs = find.detectObject(im, landmarks);

		calc_res res;
		for (int i = 0; i < objs.size(); i++)
		{
			//Rect objs_rect;
			//objs_rect.x = objs[i].x;
			//objs_rect.y = objs[i].y;
			//objs_rect.width = objs[i].width;
			//objs_rect.height = objs[i].height;
			float iou = calc_iou(objs[i], picture->rect);
			if (iou < 0.3)
			{
				ofstream outfile;
				outfile.open(res_path, ofstream::app);
				outfile << img_path << " " << objs[i].x << " " << objs[i].y << " " << objs[i].x + objs[i].width << " " << objs[i].y + objs[i].height << endl;
				outfile.close();
			}
		}

		//if (0 != landmarks.size())cout << "landmarks_number = " << landmarks[0].size() << endl;
		//for (int i = 0; i < objs.size(); ++i)
		//{
		//	rectangle(im, objs[i], Scalar(0, 255, 0), 2);
		//	rectangle(im, Point(landmarks[i][0].x - 2, landmarks[i][0].y - 2), Point(landmarks[i][0].x + 2, landmarks[i][0].y + 2), Scalar(0, 0, 255), -1);
		//	rectangle(im, Point(landmarks[i][1].x - 2, landmarks[i][1].y - 2), Point(landmarks[i][1].x + 2, landmarks[i][1].y + 2), Scalar(0, 0, 255), -1);
		//	rectangle(im, Point(landmarks[i][2].x - 2, landmarks[i][2].y - 2), Point(landmarks[i][2].x + 2, landmarks[i][2].y + 2), Scalar(0, 0, 255), -1);
		//	rectangle(im, Point(landmarks[i][3].x - 2, landmarks[i][3].y - 2), Point(landmarks[i][3].x + 2, landmarks[i][3].y + 2), Scalar(0, 0, 255), -1);
		//	rectangle(im, Point(landmarks[i][4].x - 2, landmarks[i][4].y - 2), Point(landmarks[i][4].x + 2, landmarks[i][4].y + 2), Scalar(0, 0, 255), -1);
		//}

		//if (!im.empty())
		//	imshow("demo", im);
		//waitKey(0);
	}

	return 0;
}