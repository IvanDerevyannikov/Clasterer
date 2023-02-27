#include "image.h"
#include <utility>
#include <algorithm>




Image::Image(std::string imageName) {
    im = cv::imread(imageName, cv::IMREAD_COLOR);
}

Image::Image(QString imageName) {

    im = cv::imread(imageName.toStdString(), cv::IMREAD_COLOR);    
}



cv::Mat Image::kMeans(const int& clasterCount) {


    cv::Mat kMeansImage(im.rows, im.cols,CV_8UC3, 
                            cv::Scalar(255, 255, 255));

    std::vector<cv::Point> points=findcvPoint();
    

    auto metrics = [](const cv::Point& i, const cv::Point& j)->double {
        return  sqrt(pow(i.x - j.x, 2) + pow(i.y - j.y, 2));
    };


    std::vector<std::vector<cv::Point>> clasters;
    clasters.resize(clasterCount);
    std::srand(std::time(0));

    std::vector<cv::Point> centers;
    for (int i = 0; i < clasterCount; ++i) {
        centers.push_back(points[rand()%points.size()]);
    }

   
    for (; ;) {

        for (cv::Point i : points) {
            std::vector<double> range;
            for (int k = 0; k < clasterCount; ++k) {
                range.push_back(metrics(centers[k], i));
            }
            auto it = std::min_element(std::begin(range), std::end(range));
            int index = std::distance(std::begin(range), it);
            clasters[index].push_back(i);
        }
        std::vector<cv::Point> prevCenters=centers;

        for (int k = 0; k < clasterCount; ++k) {
            int xCoord = 0;
            int yCoord = 0;
            for (int i = 0; i < clasters[k].size(); ++i) {
                xCoord += clasters[k][i].x;
                yCoord += clasters[k][i].y;
            }
            if (clasters[k].size() != 0) {
                xCoord /= clasters[k].size();
                yCoord /= clasters[k].size();
            }
           
            centers[k] = cv::Point(xCoord, yCoord);
        }
        if (centers == prevCenters) break;

        for (int k = 0; k < clasterCount; ++k) {
            clasters[k].clear();
        }
    }

    for (int k = 0; k < clasterCount; ++k) {
        cv::Vec3b randomColor(rand() % 255, rand() % 255, rand() % 255);
        for (auto& point : clasters[k]) {
            kMeansImage.at<cv::Vec3b>(point) = randomColor;
        }
    }
    for (int k = 0; k < clasterCount; ++k) {
        kMeansImage.at<cv::Vec3b>(centers[k]) = cv::Vec3b(0, 0, 0);
    }

    return kMeansImage;

}



cv::Mat Image::DBSCAN(const double& epsilon, const int& m) {

    cv::Mat DBSCANImage(im.rows, im.cols, CV_8UC3, 
                          cv::Scalar(255, 255, 255));

   ClasterDBSCAN dbscan(buildKdTree(), epsilon, m);
  
   int numberOfClasts= dbscan.clasterizeData();

   std::vector<cv::Vec3b> color(numberOfClasts);

   std::srand(std::time(0));
   for (auto& i : color) {
       i = cv::Vec3b(rand() % 200, rand() % 200, rand() % 200);
   }
  
   KdTree::KdTreeIterator iter(dbscan.getKd().getRoot());

   while (iter != nullptr) {
       if (iter->pnt.clasterId == 0 || iter->pnt.clasterId==100) {
           DBSCANImage.at<cv::Vec3b>(iter->pnt.pnt.x, iter->pnt.pnt.y) = cv::Vec3b(0, 0, 0);
           ++iter;
           continue;
       }
       DBSCANImage.at<cv::Vec3b>(iter->pnt.pnt.x,iter->pnt.pnt.y) = color[iter->pnt.clasterId-1];
        ++iter;
    }

    //cv::imshow("Test", DBSCANImage);
    cv::cvtColor(DBSCANImage, DBSCANImage,cv::COLOR_BGR2RGB);
    return DBSCANImage;
}


std::vector<ClasterPoint> Image::findPoint()
{
    cv::Mat greyMat;
    cv::cvtColor(im, greyMat, cv::COLOR_BGR2GRAY);
    cv::threshold(greyMat, greyMat, 230, 255, 0);

    std::vector<ClasterPoint> points;
    for (int i = 0; i < greyMat.rows; ++i) {
        for (int j = 0; j < greyMat.cols; ++j) {
            if (greyMat.at<unsigned char>(i, j) != 255) points.push_back(ClasterPoint(cv::Point(i, j), 0));
        }
    }
    return points;
}


std::vector<cv::Point> Image::findcvPoint()
{
    cv::Mat greyMat;
    cv::cvtColor(im, greyMat, cv::COLOR_BGR2GRAY);
    cv::threshold(greyMat, greyMat, 230, 255, 0);

    std::vector<cv::Point> points;
    for (int i = 0; i < greyMat.rows; ++i) {
        for (int j = 0; j < greyMat.cols; ++j) {
            if (greyMat.at<unsigned char>(i, j) == 0) points.push_back(cv::Point(j, i));
        }
    }
    return points;
}


KdTree Image::buildKdTree() {
    std::vector<ClasterPoint> points = findPoint();

    KdTree kd(points, 0);

    return kd;

}
