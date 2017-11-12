#include <opencv2/opencv.hpp>

class onMouseParam
{
public:
	/*onMouseParam()
	{
		image =cv::Mat::zeros(4, 4, CV_8UC1);
	};*/
	cv::Mat image;
	std::vector<cv::Point> leye;
	std::vector<cv::Point> reye;
	std::vector<cv::Point> nose;
	std::vector<cv::Point> lmouth;
	std::vector<cv::Point> rmouth;
        int count;
        //onMouseParam()
        //{
        //count = 1; 
        //}
};
