#pragma once
#include "point.h"
#include "clasters.h"
#include <QString>

class Image {
	
public: 
	Image(std::string imageName);
	Image(QString imageName);
	Image() = default;

	cv::Mat kMeans(const int& clasterCount);
	cv::Mat DBSCAN(const double& epsilon, const int& m);

	std::vector<ClasterPoint> findPoint();
	std::vector<cv::Point> findcvPoint();
	KdTree buildKdTree();
	cv::Mat& getIm() {
		return im;
	}
private:
	cv::Mat im;

};