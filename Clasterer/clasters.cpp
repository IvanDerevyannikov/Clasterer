#include "clasters.h"


void ClasterDBSCAN::addToClaster(pointVec& neighborsPts, const int& Id) {

    while(!neighborsPts.empty()) {
        ClasterPoint& last(neighborsPts.back());
        last.isNeibor = 0;
        neighborsPts.pop_back();
        last.clasterId = Id;

        if (!last.isVisited) {

            last.isVisited = true;
            pointVec neibsOfNeib = kd.findNeighbors(last,rad);

            if (neibsOfNeib.size() >= minQuan) {
                for (const auto& i : neibsOfNeib) {
                    if (i.get().isNeibor==true) continue;
                    i.get().isNeibor = true;
                    neighborsPts.push_back(i);
                }
             }
        }

    }
}


int ClasterDBSCAN::clasterizeData() {
    
    int clastId = 1;
    KdTree::KdTreeIterator iterator(kd.getRoot());
    while (iterator!=nullptr) {
        if (iterator->pnt.isVisited) {
            ++iterator;
            continue;
        }
        iterator->pnt.isVisited = true;
        pointVec neighbors = kd.findNeighbors(iterator->pnt,rad);
        for (const auto& i : neighbors) {
            i.get().isNeibor = true;
        }

        if (neighbors.size() < minQuan) iterator->pnt.clasterId = 100;
        else {
            iterator->pnt.clasterId = clastId;
            addToClaster(neighbors, clastId);
            ++clastId;
        }
    }
    return clastId;
}

