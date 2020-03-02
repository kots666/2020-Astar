#include "FindPath.h"
#include <iostream>

Point::Point(int xValue, int yValue) : x(xValue), y(yValue) {}

Point& Point::operator+(Point& rhs) {
	x += rhs.x;
	y += rhs.y;
	return *this;
}

bool Point::operator==(Point& rhs) {
	if (x == rhs.x && y == rhs.y) return true;
	return false;
}

Node::Node(Point atPoint, Node* parent, const Point& start, const Point& end) : nodePoint(atPoint), parentNode(parent) {
	gScore = ((start.x > nodePoint.x) ? start.x - nodePoint.x : nodePoint.x - start.x) + ((start.y > nodePoint.y) ? start.y - nodePoint.y : nodePoint.y - start.y);
	hScore = ((end.x > nodePoint.x) ? end.x - nodePoint.x : nodePoint.x - end.x) + ((end.y > nodePoint.y) ? end.y - nodePoint.y : nodePoint.y - end.y);
	fScore = gScore + hScore;
}

int Node::GetFScore() const {
	return fScore;
}

int Node::GetHScore() const {
	return hScore;
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
		std::cout << "\n길을 찾을 수 없습니다.\n목적지와 가장 가까운 길까지를 표시합니다.\n";
		isLockedIn = true;
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

	Point nowPoint;
	Point parentPoint;
	auto p = CloseList.rbegin();
	parentPoint = (*p)->GetParentNode()->GetPoint();

	if (isLockedIn) {
		auto p = CloseList.begin();
		++p;
		int min = (*p)->GetHScore();
		Point minPoint = (*p)->GetPoint();

		while (p != CloseList.end()) {
			if (min > (*p)->GetHScore()) {
				min = (*p)->GetHScore();
				minPoint = (*p)->GetPoint();
			}
			++p;
		}
		
		for (auto k = CloseList.rbegin(); k != CloseList.rend(); ++k) {
			if (IsOverlapped(minPoint, (*k)->GetPoint())) {
				if (map[minPoint.y][minPoint.x] != STARTPOINT && map[minPoint.y][minPoint.x] != ENDPOINT) {
					map[minPoint.y][minPoint.x] = FINDROAD;
					if ((*k)->GetParentNode() == nullptr) break;
					minPoint = (*k)->GetParentNode()->GetPoint();
				}
			}
		}

	}

	else {
		Point nowPoint;
		Point parentPoint;
		auto p = CloseList.rbegin();
		parentPoint = (*p)->GetParentNode()->GetPoint();


		for (; p != CloseList.rend(); ++p) {
			nowPoint = (*p)->GetPoint();
			if (IsOverlapped(nowPoint, parentPoint)) {
				if (map[nowPoint.y][nowPoint.x] != STARTPOINT && map[nowPoint.y][nowPoint.x] != ENDPOINT) {
					map[nowPoint.y][nowPoint.x] = FINDROAD;
					if ((*p)->GetParentNode() == nullptr) break;
					parentPoint = (*p)->GetParentNode()->GetPoint();
				}
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
	isLockedIn = false;
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