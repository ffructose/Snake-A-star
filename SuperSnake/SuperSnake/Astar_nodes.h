
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <list>

using namespace std;

class Astar_class
{
private:

	struct sNode
	{
		bool isObstacle = false;		  // �� ����� ���������
		bool isLearned = false;			  // �� ������� ��� �����
		float distanceNow;				  // ³������ �� ���� ����� �����
		float distanceAnother;			  // ³������ �� ���� ���� ��� ����� ������
		int x;							  // ���������� ����� �
		int y;                            // ���������� ����� �
		vector<sNode*> vector_Neighbours; // ������ � ������� �������
		sNode* parent;					  // ������ �����
	};

	sNode* nodes = nullptr;
	const int W_star = 54; // 54 (0; 54) [1; 53] [0; 54)
	const int H_star = 30; // 30 (2; 30) [3; 29] [3; 30)

	sNode* startNode = nullptr;
	sNode* endNode = nullptr;


	// ���������� ������� �� ������ � � � �� ������� �� �������� �������
	float distance(sNode* a, sNode* b);

	void Astar_Solve();

public:

	void Astar_Create();

	int Astar_Update(int s_1_x, int s_1_y, int s_n_1_x, int s_n_1_y, int s_0_x, int s_0_y, int f_x, int f_y);

};
