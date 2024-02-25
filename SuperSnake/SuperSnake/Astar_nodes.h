
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
		bool isObstacle = false;		  // Чи вузол перешкода
		bool isLearned = false;			  // Чи вивчено вже вузол
		float distanceNow;				  // Відстань до кінця прямо зараз
		float distanceAnother;			  // Відстань до кінця якщо піти іншим вузлом
		int x;							  // Координата вузла х
		int y;                            // Координата вузла у
		vector<sNode*> vector_Neighbours; // Звязок з сусідніми вузлами
		sNode* parent;					  // Батько вузла
	};

	sNode* nodes = nullptr;
	const int W_star = 54; // 54 (0; 54) [1; 53] [0; 54)
	const int H_star = 30; // 30 (2; 30) [3; 29] [3; 30)

	sNode* startNode = nullptr;
	sNode* endNode = nullptr;


	// розраховую відстань між вузлом а і б по діагоналі за теоремою піфагора
	float distance(sNode* a, sNode* b);

	void Astar_Solve();

public:

	void Astar_Create();

	int Astar_Update(int s_1_x, int s_1_y, int s_n_1_x, int s_n_1_y, int s_0_x, int s_0_y, int f_x, int f_y);

};
