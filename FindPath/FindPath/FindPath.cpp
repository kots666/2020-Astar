#include "FindPath.h"
#include <iostream>

Point::Point(int i, int k) : x(i), y(k) {}

Point& Point::operator+(Point& rhs) {
	x += rhs.x;
	y += rhs.y;
	return *this;
}

bool Point::operator==(Point& rhs) {
	if (x == rhs.x && y == rhs.y) return true;
	return false;
}

Node::Node(Point nP, Node* parent, const Point& start, const Point& end) : nodePoint(nP), parentNode(parent) {
	gScore = ((start.x > nodePoint.x) ? start.x - nodePoint.x : nodePoint.x - start.x) + ((start.y > nodePoint.y) ? start.y - nodePoint.y : nodePoint.y - start.y);
	hScore = ((end.x > nodePoint.x) ? end.x - nodePoint.x : nodePoint.x - end.x) + ((end.y > nodePoint.y) ? end.y - nodePoint.y : nodePoint.y - end.y);
	fScore = gScore + hScore;
}

int Node::GetFScore() const {
	return fScore;
}

Point Node::GetPoint() {
	return nodePoint;
}

Node* Node::GetParentNode() {
	return parentNode;
}

void Node::SwapNode(Node* other) {
	*this = *other;
}

void AStar::AddToOpenList(Node* n) {
	for (auto it : CloseList) {
		if (IsOverlapped(it->GetPoint(), n->GetPoint())) return;
	}

	for (auto it : OpenList) {
		if (IsOverlapped(it->GetPoint(), n->GetPoint())) {
			if (it->GetFScore() > n->GetFScore()) it->SwapNode(n);
			return;
		}
	}
	OpenList.push_back(n);
}

Node* AStar::AddToCloseList() {
	if (OpenList.empty()) {
		std::cout << "길을 찾을 수 없습니다.\n";
		return nullptr;
	}
	auto it = OpenList.begin();
	int min = (*it)->GetFScore();
	for (auto p = OpenList.begin(); p != OpenList.end(); ++p) {
		if (min > (*p)->GetFScore()) {
			min = (*p)->GetFScore();
			it = p;
		}
	}
	CloseList.push_back(*it);
	Node* n = *it;

	OpenList.erase(it);

	return n;
}

bool AStar::CanKeepGo(const Point& p) {
	if (p.x < 0 || p.y < 0 || p.x >= count || p.y >= count) return false;
	if (WALL == map[p.y][p.x]) return false;
	return true;
}

void AStar::FindPath() {
	Node *node = new Node(startPoint, nullptr, startPoint, startPoint);
	CloseList.push_back(node);
	Node *n = (*CloseList.begin());

	RecursiveFindPath(n);

	Point changePoint;
	Node* nextParent = nullptr;
	auto p = CloseList.rbegin();
	nextParent = (*p)->GetParentNode();

	for (; p != CloseList.rend(); ++p) {
		changePoint = (*p)->GetPoint();
		if (IsOverlapped(changePoint, nextParent->GetPoint())) {
			if (map[changePoint.y][changePoint.x] != STARTPOINT && map[changePoint.y][changePoint.x] != ENDPOINT) {
				map[changePoint.y][changePoint.x] = FINDROAD;
				nextParent = (*p)->GetParentNode();
			}
		}
	}
}

void AStar::RecursiveFindPath(Node* n) {
	if (IsOverlapped(n->GetPoint(), endPoint)) return;

	for (int i = 0; i < 4; ++i) {
		Point tempPoint = n->GetPoint();
		tempPoint = tempPoint + Direction[i];
		if (CanKeepGo(tempPoint)) {
			Node* newNode = new Node(tempPoint, n, startPoint, endPoint);
			AddToOpenList(newNode);
		}
	}
	n = AddToCloseList();
	if (n == nullptr) return;
	RecursiveFindPath(n);
}

AStar::AStar() {
	Direction[UP] = Point(0, 1);
	Direction[DOWN] = Point(0, -1);
	Direction[LEFT] = Point(-1, 0);
	Direction[RIGHT] = Point(1, 0);
	count = 0;
}

void AStar::MakeMap(char ** m, int num, const Point & start, const Point & end) {
	map = m;
	count = num;
	startPoint = start;
	endPoint = end;

	FindPath();
}

AStar::~AStar() {
	if (!OpenList.empty()) {
		for (auto p = OpenList.begin(); p != OpenList.end(); ++p) {
			delete *p;
		}
		OpenList.clear();
	}
	if (!CloseList.empty()) {
		for (auto p = CloseList.begin(); p != CloseList.end(); ++p) {
			delete *p;
		}
		CloseList.clear();
	}
}

bool IsOverlapped(const Point& lhs, const Point& rhs) {
	if (lhs.x == rhs.x && lhs.y == rhs.y) return true;
	return false;
}