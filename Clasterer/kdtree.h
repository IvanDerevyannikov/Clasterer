#pragma once
#include "point.h" 
#include <algorithm>
#include <functional>
#include <stack>


using pointVec = std::vector<std::reference_wrapper<ClasterPoint>>;


class KdTree {

	struct Node;

public:
	
	class KdTreeIterator {
	public:
		KdTreeIterator(Node* element);

		Node* operator++();

		Node* operator->() {
			return iter.top();
		}

		bool operator !=(Node* compare) {
			if (iter.empty()) return nullptr != compare;
			return iter.top() != compare;
		}

	private:
		std::stack<Node*> iter;

	};



	KdTree() :root(nullptr) {}
	KdTree(std::vector<ClasterPoint> points, int depth);

	KdTree(const KdTree& other);
	KdTree(KdTree&& other) noexcept;

	KdTree& operator=(const KdTree& other);
	KdTree& operator=(KdTree&& other) noexcept;

	~KdTree();


	Node* getRoot() {
		return root;
	}

	pointVec findNeighbors(const ClasterPoint& point, const double& radius);

	void print();


private:
	struct Node {

		Node(ClasterPoint point, int depth) :pnt(point), depth(depth) {};
		ClasterPoint pnt;
		int depth;
		Node* left=nullptr;
		Node* right=nullptr;
	};

	
	Node* makeTree(std::vector<ClasterPoint>::iterator first,
		std::vector<ClasterPoint>::iterator last,int depth);

	Node* copy(Node* copied);

	void deleteNode(Node* delited);

	void addtoNeib(const ClasterPoint& point, const double& radius, Node* compnt, pointVec& neighnors);

	void printNode(Node* printed);

	double metrics(const cv::Point& i, const cv::Point& j) {
		return  sqrt(pow(i.x - j.x, 2) + pow(i.y - j.y, 2));
	}

	Node* root;
};