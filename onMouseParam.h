#include <opencv2\opencv.hpp>

class onMouseParam
{
public:
	/*onMouseParam()
	{
		image =cv::Mat::zeros(4, 4, CV_8UC1);
	};*/
	cv::Mat image;
	std::vector<cv::Point> rect_leftup;
	std::vector<cv::Point> rect_rightdown;
};