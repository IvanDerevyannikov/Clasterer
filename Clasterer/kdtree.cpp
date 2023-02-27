#include "kdtree.h"


KdTree::KdTreeIterator::KdTreeIterator(Node* element) {
	iter.push(element);
	while (element->left != nullptr) {
		iter.push(element->left);
		element = element->left;
	}
}

KdTree::Node* KdTree::KdTreeIterator::operator++() {
	Node* current = iter.top();
	iter.pop();
	if (current->right != nullptr) {
		iter.push(current->right);
		current = current->right;
		while (current->left != nullptr) {
			iter.push(current->left);
			current = current->left;
		}
	}

	if (iter.empty()) return nullptr;
	return iter.top();
}





KdTree::KdTree(std::vector<ClasterPoint> points, int depth) {
	
	auto compare = [depth](const ClasterPoint& first, const ClasterPoint& second) {
		if (depth % 2 == 0) {
			return first.pnt.x < second.pnt.x;
		}
		else {
			return first.pnt.y < second.pnt.y;
		}
	};

	std::sort(points.begin(), points.end(), compare);

	ClasterPoint median = points[points.size() / 2];

	root = new Node(median, depth);
	root->left = makeTree(points.begin(),points.begin() + points.size() / 2-1,
						  depth + 1);
	root->right = makeTree(points.begin() + points.size() / 2+1, 
						   points.end()-1, depth + 1);

}


KdTree::KdTree(const KdTree& other)
{

	root = new Node(other.root->pnt, other.root->depth);

	root->left = copy(other.root->left);
	root->right = copy(other.root->right);

}


KdTree::KdTree(KdTree&& other) noexcept {

	this->root = other.root;
	other.root = nullptr;
}


KdTree& KdTree::operator=(const KdTree& other) {

	if (this->root == other.root) return *this;

	root = new Node(other.root->pnt, other.root->depth);

	root->left = copy(other.root->left);
	root->right = copy(other.root->right);

	return *this;

}


KdTree& KdTree::operator=(KdTree&& other) noexcept {

	this->root = other.root;
	other.root = nullptr;
	return *this;
}


KdTree::~KdTree() {

	deleteNode(root);
}


pointVec KdTree::findNeighbors(const ClasterPoint& point, const double& radius)
{
	pointVec neighbors;

	addtoNeib(point, radius, root, neighbors);

	return neighbors;
}


void KdTree::print() {
	printNode(root);
}





KdTree::Node* KdTree::makeTree(std::vector<ClasterPoint>::iterator first,
	std::vector<ClasterPoint>::iterator last, int depth) {

	auto compare = [depth](const ClasterPoint& first, const ClasterPoint& second) {
		if (depth % 2 == 0) {
			return first.pnt.x < second.pnt.x;
		}
		else {
			return first.pnt.y < second.pnt.y;
		}
	};

	std::sort(first, last + 1, compare);
	ClasterPoint median = *(first + (last - first) / 2);
	auto medianpos = first + (last - first) / 2;
	Node* pval = new Node(median, depth);
	if (first < medianpos) {
		pval->left = makeTree(first, medianpos - 1, depth + 1);
	}
	if (last > medianpos) {
		pval->right = makeTree(medianpos + 1, last, depth + 1);
	}
	return pval;
}


KdTree::Node* KdTree::copy(Node* copied) {
	Node* val = new Node(copied->pnt, copied->depth);
	if (copied->left != nullptr) {
		val->left = copy(copied->left);
	}
	if (copied->right != nullptr) {
		val->right = copy(copied->right);
	}
	return val;
}


void KdTree::deleteNode(Node* delited)
{
	if (delited == nullptr) return;

	deleteNode(delited->left);
	deleteNode(delited->right);
	delete delited;
}


void KdTree::addtoNeib(const ClasterPoint& point, const double& radius, Node* compnt, pointVec& neighbors)
{
	if (compnt == nullptr) return;

	double metrica = metrics(point.pnt, compnt->pnt.pnt);
	if (metrica < radius && metrica != 0) {
		neighbors.push_back(compnt->pnt);
	}


	if (abs(point.getCoord(compnt->depth) - compnt->pnt.getCoord(compnt->depth))
		<= radius) {
		addtoNeib(point, radius, compnt->left, neighbors);
		addtoNeib(point, radius, compnt->right, neighbors);
	}
	else if (abs(point.getCoord(compnt->depth) -
			 compnt->pnt.getCoord(compnt->depth)) > radius
			 && point.getCoord(compnt->depth) > compnt->pnt.getCoord(compnt->depth)) {

		addtoNeib(point, radius, compnt->right, neighbors);

	}
	else if (abs(point.getCoord(compnt->depth) -
			 compnt->pnt.getCoord(compnt->depth)) > radius
		     && point.getCoord(compnt->depth) < compnt->pnt.getCoord(compnt->depth)) {

		addtoNeib(point, radius, compnt->left, neighbors);
	}
}



std::ostream& operator << (std::ostream& out, const ClasterPoint& pnt) {
	out << pnt.pnt.x << ' ' << pnt.pnt.y;
	return out;
}

void KdTree::printNode(KdTree::Node* printed) {

	if (printed == nullptr) return;
	std::cout << printed->pnt << '\n';
	printNode(printed->left);
	printNode(printed->right);

}