#pragma once
#include <string>
#include <list>

enum Direction { UP = 0, DOWN = 1, RIGHT = 2, LEFT = 3 };
const char ROAD = '0';
const char WALL = '1';
const char FINDROAD = '5';
const char STARTPOINT = '8';
const char ENDPOINT = '9';

// ����� ��ġ ���� ���� ����ü
struct Point {
	int x;
	int y;
	Point() = default;
	Point(int xValue, int yValue);
	Point& operator +(Point& rhs);
	bool operator ==(Point& rhs);
};

// OpenList�� CloseList�� �� Node
class Node {
private:
	Point nodePoint;
	int fScore;
	int gScore;
	int hScore;
	Node* parentNode;
public:
	Node(Point atPoint, Node* parent, const Point& start, const Point& end);

	int GetFScore() const;
	int GetHScore() const;
	Point GetPoint();
	Node* GetParentNode();

	// ������ Node�� other�� ��ü
	void SwapNode(Node* other);
};

class AStar {
private:
	std::list<Node*> OpenList;
	std::list<Node*> CloseList;
	char** map;
	Point Direction[4];
	Point startPoint;
	Point endPoint;
	int count;
	bool isLockedIn;

	void AddToOpenList(Node* n);
	Node* AddToCloseList();
	bool CanKeepGo(const Point& p);

	void FindPath();
	void RecursiveFindPath(Node* n);
public:
	AStar();

	// map �����͸� �޾ƿ�
	void MakeMap(char** m, int num, const Point& start, const Point& end);

	~AStar();
};

bool IsOverlapped(const Point& lhs, const Point& rhs);