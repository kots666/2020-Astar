#include <iostream>
#include "FindPath.h"

int main() {
	size_t num;
	std::cout << "���� ũ�⸦ �����ֽʽÿ� (1���� Ŀ���ϸ�, NxN ������� �����մϴ�.) : ";
	std::cin >> num;

	if (num <= 1) {
		std::cout << "�߸��� �Է��Դϴ�.\n";
		return 0;
	}

	char **map = new char*[num];
	for (size_t i = 0; i < num; ++i) {
		map[i] = new char[num];
	}

	Point startPoint(-1, -1);
	Point endPoint(-1, -1);

	std::string s;

	std::cout << "���� �Է����ֽʽÿ�.\n0 : ���, 1 : ��, 8 : ��������, 9 : ���������̸�, ���������� ���������� �ݵ�� 1���� �����ؾ� �մϴ�.\n";

	size_t startCnt = 0;
	size_t endCnt = 0;

	for (size_t i = 0; i < num; ++i) {
		s.clear();
		std::cin >> s;

		if (s.size() != num) {
			std::cout << "�߸��� ũ�⸦ �Է��߽��ϴ�.\n";
			return 0;
		}

		for (size_t j = 0; j < num; ++j) {
			if (s[j] != ROAD && s[j] != WALL && s[j] != STARTPOINT && s[j] != ENDPOINT) {
				std::cout << "�߸��� �Է��Դϴ�.\n";
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
		std::cout << "���������̳� ���������� �Է��� �߸��ƽ��ϴ�.\n";
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