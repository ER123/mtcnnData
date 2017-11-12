#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <stdio.h>
#include <sys/stat.h>
//#include <direct.h>
#include "gainFoldname.h"
#include "readFilesNames.h"
#include "onMouseParam.h"

//#define X_SCALE (1.3)
//#define Y_SCALE (1.3)

using namespace cv;
using namespace std;

const string ImgProcessWin = "drawYOLOlandmarks";
int key_for_switch;
vector<string> picnames;
vector<string> picaddress;


static void onMouse(int,int,int,int,void*);

int main(int argc,char* argv[])
{
	string foldname;
	Mat copyImage;
	double normalization_x;
	double normalization_y;
	double faceR = 0;
    	double eyesdis = 0;
    	double mouthdis = 0;
	double leyelmouth = 0;
    	double reyermouth = 0;
    	//double leyermouth = 0;
    	//double reyelmouth = 0;
	double maxdis;
	double mindis;
	double factor;
    	double bboxx = 0;//label
    	double bboxy = 0;//label
    	double bboxw = 0;//label
    	double bboxh = 0;//label
	Point facec;
        //double zoom_x = 0;
        //double zoom_y = 0;
	onMouseParam dataImage;
	//vector<string>* filenames;
	bool notargetFile = false;
        //int count = 1;
	cout << "this program aims to do DataPreprocess for YOLO" << endl;
        if(argc == 1)
	foldname = gainFoldname();
        if(argc == 2)
        foldname = argv[1];
        if(argc > 2)
        {;cout << "argc = " << argc << "\nusage:./makeYOLOlandmarks [dir]" <<endl;return 0;}
	if('/' == foldname.back())foldname.pop_back();
        //cout << "argc = " << argc << endl;
        //cout << foldname << endl;
	if (!readFilesNames(foldname)){cout << "there is no such dir." << endl;return 0;}
	//cout<<foldname<<endl;//###
	string txtname = foldname + "/" + "train.txt";
	string labelname;
	ofstream traintxt(txtname);
	namedWindow(ImgProcessWin,WINDOW_AUTOSIZE);
	setMouseCallback(ImgProcessWin, onMouse, (void*)&dataImage);
	key_for_switch = 0;
	for (auto iter = picnames.begin(),picdiriter = picaddress.begin(); iter != picnames.end(); ++iter, ++picdiriter)
	{
		dataImage.count = 1;
		dataImage.image = imread(*picdiriter);
                //zoom_x = dataImage.image.cols / 800;
                //zoom_y = dataImage.image.rows / 800;
		if(dataImage.image.rows > 1000)resize(dataImage.image, dataImage.image, Size(round(1000 * ((double)dataImage.image.cols / (double)dataImage.image.rows)), 1000), 0, 0);
                if(dataImage.image.cols > 1500)resize(dataImage.image, dataImage.image, Size(1500, round(1500 * ((double)dataImage.image.rows / (double)dataImage.image.cols))), 0, 0);
		copyImage = ((dataImage).image).clone();
		normalization_x = 1.0 / (double)copyImage.cols;
		normalization_y = 1.0 / (double)copyImage.rows;
		//cal label.txt's name
		labelname = (*iter);
		labelname.pop_back();
		labelname.pop_back();
		labelname.pop_back();
		labelname.pop_back();
		if(0 == iter - picnames.begin())mkdir((foldname + "/label").c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		labelname = foldname + "/label/" + labelname + ".txt";

		imshow(ImgProcessWin, (dataImage).image);
		waitKey(0);

		ofstream label(labelname);
		label.precision(12);
		for (int a = 0; a < ((dataImage).rmouth).size(); ++a)
		{
			bboxx = (double)(dataImage.leye[a].x + dataImage.reye[a].x + dataImage.lmouth[a].x + dataImage.rmouth[a].x) / 4.0 / (double)dataImage.image.cols;//label
            		bboxy = (double)(dataImage.leye[a].y + dataImage.reye[a].y + dataImage.lmouth[a].y + dataImage.rmouth[a].y) / 4.0 / (double)dataImage.image.rows;//label

            		//calculate distence
            		eyesdis = (double)((dataImage.reye[a].x - dataImage.leye[a].x) * (dataImage.reye[a].x-dataImage.leye[a].x) + (dataImage.reye[a].y-dataImage.leye[a].y) * (dataImage.reye[a].y-dataImage.leye[a].y));
            		eyesdis = sqrt(eyesdis);
            		mouthdis = (double)((dataImage.rmouth[a].x - dataImage.lmouth[a].x) * (dataImage.rmouth[a].x-dataImage.lmouth[a].x) + (dataImage.rmouth[a].y-dataImage.lmouth[a].y) * (dataImage.rmouth[a].y-dataImage.lmouth[a].y));
            		mouthdis = sqrt(mouthdis);
            		leyelmouth = (double)((dataImage.lmouth[a].x - dataImage.leye[a].x) * (dataImage.lmouth[a].x-dataImage.leye[a].x) + (dataImage.lmouth[a].y-dataImage.leye[a].y) * (dataImage.lmouth[a].y-dataImage.leye[a].y));
            		leyelmouth = sqrt(leyelmouth);
            		reyermouth = (double)((dataImage.rmouth[a].x - dataImage.reye[a].x) * (dataImage.rmouth[a].x-dataImage.reye[a].x) + (dataImage.rmouth[a].y-dataImage.reye[a].y) * (dataImage.rmouth[a].y-dataImage.reye[a].y));
            		reyermouth = sqrt(reyermouth);
            		//leyermouth = (double)((dataImage.leye[a].x - dataImage.rmouth[a].x) * (dataImage.leye[a].x-dataImage.rmouth[a].x) + (dataImage.leye[a].y-dataImage.rmouth[a].y) * (dataImage.leye[a].y-dataImage.rmouth[a].y));
            		//leyermouth = sqrt(leyermouth);
            		//reyelmouth = (double)((dataImage.reye[a].x - dataImage.lmouth[a].x) * (dataImage.reye[a].x-dataImage.lmouth[a].x) + (dataImage.reye[a].y-dataImage.lmouth[a].y) * (dataImage.reye[a].y-dataImage.lmouth[a].y));
            		//reyelmouth = sqrt(reyelmouth);
			maxdis = eyesdis;
            		maxdis > mouthdis ? : maxdis = mouthdis;
            		//leyelmouth > reyermouth ? : leyelmouth = reyermouth;
            		//leyermouth > reyelmouth ? : leyermouth = reyelmouth;
            		maxdis > leyelmouth ? : maxdis = leyelmouth;
           	 	//maxdis > leyermouth ? : maxdis = leyermouth;
           	 	maxdis > reyermouth ? : maxdis = reyermouth;
           	 	//maxdis > reyelmouth ? : maxdis = reyelmouth;
			/*mindis = eyesdis;
            		mindis > mouthdis ? mindis = mouthdis; : ;
            		mindis > leyelmouth ? mindis = leyelmouth; : ;
           	 	mindis > leyermouth ? mindis = leyermouth; : ;
           	 	mindis > reyermouth ? mindis = reyermouth; : ;
           	 	mindis > reyelmouth ? mindis = reyelmouth; : ;
			factor = mindis / (maxdis + mindis)*/
            		faceR = maxdis * 2.0;
			faceR = faceR * 1.4;
			//faceR = eyesdis * 3;
            		bboxw = faceR / (double)dataImage.image.cols;//label
            		bboxh = faceR / (double)dataImage.image.rows;//label
			//bboxw = faceR / (double)dataImage.image.cols;
            		//bboxh = faceR / (double)dataImage.image.rows;//label
			facec.x = bboxx * dataImage.image.cols;//
			facec.y = bboxy * dataImage.image.rows;//
			rectangle(dataImage.image,Point(facec.x - faceR / 2,facec.y - faceR / 2),Point(facec.x + faceR / 2,facec.y + faceR / 2),Scalar(0,0,255),1);//
			//imshow(ImgProcessWin,dataImage.image);
			//waitKey(150);
			label << 0 << " " << bboxx << " " << bboxy << " " << bboxw << " " << bboxh << endl;
			//label << 0 << " " << (((double)(((dataImage).rect_rightdown)[a].x) / 2.0 + ((double)(((dataImage).rect_leftup)[a].x)) / 2.0)*normalization_x) << " " << (((double)(((dataImage).rect_rightdown)[a].y) / 2.0 + (double)(((dataImage).rect_leftup)[a].y) / 2.0)*normalization_y) << " " << ((double)((((dataImage).rect_rightdown)[a].x) - ((((dataImage).rect_leftup)[a].x)))*normalization_x) << " " << ((double)((((dataImage).rect_rightdown)[a].y) - ((((dataImage).rect_leftup)[a].y)))*normalization_y) << endl;
		}
		label.close();
                //cout << "key_for_switch = " << key_for_switch << endl;
		cout << "press r to redraw this pic\npress s for saving the rectangle points\npress esc fot exit." << endl;
		imshow(ImgProcessWin,dataImage.image);
		key_for_switch = waitKey(0);key_for_switch = key_for_switch - 1048576;
                //cout << "key_for_switch = " << key_for_switch << endl;

		if (27 == key_for_switch){remove(labelname.c_str());break;}
		bool redraw = false;
		//int i;//
		switch (key_for_switch)
		{
		case 's':
			break;
		case 'S':
			break;
		case 'l':
			remove((*picdiriter).c_str());
			remove(labelname.c_str());
			redraw = true;
			break;
		case 'L':
			remove((*picdiriter).c_str());
			remove(labelname.c_str());
			redraw = true;
			break;
		default:
			remove(labelname.c_str());
			(dataImage).image = copyImage.clone();
			--iter; --picdiriter;
                        dataImage.count = 1;
			redraw = true;
		        dataImage.leye.clear();
			dataImage.reye.clear();
			//dataImage.nose.clear();
			dataImage.lmouth.clear();
			dataImage.rmouth.clear();
			break;
		}

		dataImage.leye.clear();
		dataImage.reye.clear();
		//dataImage.nose.clear();
		dataImage.lmouth.clear();
		dataImage.rmouth.clear();
		if(redraw)continue;
		traintxt << *picdiriter << endl;
	}
	traintxt.close();
	cout << "all of your images are processed,congratulation!" << endl;
    return 0;
}

static void onMouse(int event, int x, int y, int flag, void* param)
{
	static bool lbuttondown = false;
	static Point curpoint,firpoint;
	static Mat temp;
	//switch (key_for_switch)
	//{
	//default:
		//temp = (*((onMouseParam*)param)).image.clone();
	//}
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
                if(!lbuttondown)
                {
		    firpoint = Point(x, y);
                    switch ((*((onMouseParam*)param)).count)
                    {
                    case 1:
                            (*((onMouseParam*)param)).leye.push_back(firpoint);(*((onMouseParam*)param)).count++;rectangle((*((onMouseParam*)param)).image, Point(firpoint.x-1,firpoint.y-1), Point(firpoint.x+1,firpoint.y+1), Scalar(0, 0, 255), -1);break;
                    case 2:
                            (*((onMouseParam*)param)).reye.push_back(firpoint);(*((onMouseParam*)param)).count++;rectangle((*((onMouseParam*)param)).image, Point(firpoint.x-1,firpoint.y-1), Point(firpoint.x+1,firpoint.y+1), Scalar(0, 0, 255), -1);break;
                    case 3:
                            (*((onMouseParam*)param)).count++;//(*((onMouseParam*)param)).nose.push_back(firpoint);(*((onMouseParam*)param)).count++;rectangle((*((onMouseParam*)param)).image, Point(firpoint.x-1,firpoint.y-1), Point(firpoint.x+1,firpoint.y+1), Scalar(0, 0, 255), -1);break;//this is for drawing nose landmark
                    case 4:
                            (*((onMouseParam*)param)).lmouth.push_back(firpoint);(*((onMouseParam*)param)).count++;rectangle((*((onMouseParam*)param)).image, Point(firpoint.x-1,firpoint.y-1), Point(firpoint.x+1,firpoint.y+1), Scalar(0, 0, 255), -1);break;
                    case 5:
                            (*((onMouseParam*)param)).rmouth.push_back(firpoint);(*((onMouseParam*)param)).count = 1;rectangle((*((onMouseParam*)param)).image, Point(firpoint.x-1,firpoint.y-1), Point(firpoint.x+1,firpoint.y+1), Scalar(0, 0, 255), -1);break;
                    }
                    //rectangle((*((onMouseParam*)param)).image, firpoint, curpoint, Scalar(0, 0, 255), -1);
                    imshow(ImgProcessWin, (*((onMouseParam*)param)).image);
	 	    //cout << x << y << endl;//
		    lbuttondown = true;
                }
		break;
	case EVENT_LBUTTONUP:
		lbuttondown = false;
		//curpoint=Point(x, y);
		//imshow(ImgProcessWin, (*((onMouseParam*)param)).image);
		//(*((onMouseParam*)param)).rect_leftup.push_back(firpoint);
		//(*((onMouseParam*)param)).rect_rightdown.push_back(curpoint);
		//rectangle((*((onMouseParam*)param)).image, firpoint, curpoint, Scalar(0, 0, 255), 1, 4, 0);
		imshow(ImgProcessWin, (*((onMouseParam*)param)).image);
		//temp = (*((onMouseParam*)param)).image.clone();
		break;
	case EVENT_MOUSEMOVE:
		//if (true == lbuttondown)
		//{
			/*curpoint.x = x;
			curpoint.y = y;*/
			//curpoint=Point(x, y);
			//rectangle((*((onMouseParam*)param)).image, firpoint, curpoint, Scalar(0, 0, 255), 1, 4, 0);
		imshow(ImgProcessWin, (*((onMouseParam*)param)).image);
			//(*((onMouseParam*)param)).image = temp.clone();
		//}
		break;
	}
}
