#include <iostream>
#include "FindPath.h"

int main() {
	size_t num;
	std::cout << "맵의 크기를 정해주십시오 (1보다 커야하며, NxN 사이즈로 생성합니다.) : ";
	std::cin >> num;

	if (num <= 1) {
		std::cout << "잘못된 입력입니다.\n";
		return 0;
	}

	char **map = new char*[num];
	for (size_t i = 0; i < num; ++i) {
		map[i] = new char[num];
	}

	Point startPoint(-1, -1);
	Point endPoint(-1, -1);

	std::string s;

	std::cout << "맵을 입력해주십시오.\n0 : 통로, 1 : 벽, 8 : 시작지점, 9 : 도착지점이며, 시작지점과 도착지점은 반드시 1개씩 존재해야 합니다.\n";

	size_t startCnt = 0;
	size_t endCnt = 0;

	for (size_t i = 0; i < num; ++i) {
		s.clear();
		std::cin >> s;

		if (s.size() != num) {
			std::cout << "잘못된 크기를 입력했습니다.\n";
			return 0;
		}

		for (size_t j = 0; j < num; ++j) {
			if (s[j] != ROAD && s[j] != WALL && s[j] != STARTPOINT && s[j] != ENDPOINT) {
				std::cout << "잘못된 입력입니다.\n";
				return 0;
			}

			map[i][j] = s[j];

			if (STARTPOINT == map[i][j]) {
				startPoint = Point(j, i);
				++startCnt;
			}

			else if (ENDPOINT == map[i][j]) {
				endPoint = Point(j, i);
				++endCnt;
			}
		}
	}

	if (1 != startCnt || 1 != endCnt) {
		std::cout << "시작지점이나 도착지점의 입력이 잘못됐습니다.\n";
		return 0;
	}

	AStar pathManager;
	pathManager.MakeMap(map, num, startPoint, endPoint);

	std::cout << "\n==========================\n\n";

	for (size_t i = 0; i < num; ++i) {
		for (size_t j = 0; j < num; j++) {
			std::cout << map[i][j];
		}
		std::cout << "\n";
	}

	for (size_t i = 0; i < num; ++i) {
		delete[] map[i];
	}
	delete[] map;

}