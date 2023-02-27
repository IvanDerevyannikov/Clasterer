#pragma once

#include <opencv2/opencv.hpp>


struct ClasterPoint {
public:
	ClasterPoint(cv::Point point, int id=0) :pnt(point), clasterId(id) {}

	cv::Point pnt;
	int clasterId;
	bool isVisited=0;
	bool isNeibor = 0;


	operator cv::Point() {
		return pnt;
	}

	int getCoord(int num) const {
		if (num%2 == 1)
			return pnt.y;
		return pnt.x;
	}

};