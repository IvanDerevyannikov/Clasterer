#pragma once
#include "point.h"
#include <vector>
#include <algorithm>
#include "kdtree.h"


class ClasterDBSCAN {
public:

	ClasterDBSCAN(KdTree points, double radius, int minQuantity) {
		kd = std::move(points);
		rad = radius;
		minQuan = minQuantity;
	}

	int clasterizeData();

	KdTree& getKd() {
		return kd;
	} 
private:
	
	void addToClaster(pointVec& neighborsPts, const int& Id);
	KdTree kd;
	double rad;
	int minQuan;
};
