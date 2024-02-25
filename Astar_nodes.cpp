#include "Astar_nodes.h"

void Astar_class::Astar_Create()
{
	// ������� 2� ����� �����
	nodes = new sNode[W_star * H_star];

	for (int x = 1; x < W_star; x++)
		for (int y = 3; y < H_star; y++)
		{
			nodes[y * W_star + x].x = x;  // ����� ����� ������ ��� � � ���������� , ����� � �������, ����������:
			nodes[y * W_star + x].y = y;  // nodes[����������_� * ������_���� + ����������_�].(x/y)
			nodes[y * W_star + x].isObstacle = false;
			nodes[y * W_star + x].parent = nullptr;
			nodes[y * W_star + x].isLearned = false;
		}

	// ������� ��'���� �� ������� (GRID)
	// ��� ����� �� ������������ ����� ������ ����� � ������ �� �� ���� ������.
	for (int x = 1; x < W_star; x++)
		for (int y = 3; y < H_star; y++)
		{
			if (y > 3)
				nodes[y * W_star + x].vector_Neighbours.push_back(&nodes[(y - 1) * W_star + (x + 0)]);
			if (y < H_star - 1)
				nodes[y * W_star + x].vector_Neighbours.push_back(&nodes[(y + 1) * W_star + (x + 0)]);
			if (x > 1)
				nodes[y * W_star + x].vector_Neighbours.push_back(&nodes[(y + 0) * W_star + (x - 1)]);
			if (x < W_star - 1)
				nodes[y * W_star + x].vector_Neighbours.push_back(&nodes[(y + 0) * W_star + (x + 1)]);
		}

	// �������� ������� ����������, ���� ����
	startNode = &nodes[(H_star / 2) * W_star + 1];
	endNode = &nodes[(H_star / 2) * W_star + W_star - 2];
}

// ���������� ������� �� ������ � � � �� ������� �� �������� �������
float Astar_class::distance(sNode* a, sNode* b)
{
	return sqrt((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
}

void Astar_class::Astar_Solve()
{
	// ������ ���� ���� ��� ����������� ������ � ���� (������� ���������)
	for (int x = 1; x < W_star; x++)
		for (int y = 3; y < H_star; y++)
		{
			nodes[y * W_star + x].isLearned = false;
			nodes[y * W_star + x].distanceNow = INFINITY;
			nodes[y * W_star + x].distanceAnother = INFINITY;
			nodes[y * W_star + x].parent = nullptr;
		}

	// ����� ��������� ����� ��� ����������� (������ �����)
	sNode* currentNode = startNode;
	startNode->distanceAnother = 0.0f;
	startNode->distanceNow = distance(startNode, endNode);

	// ���������� ����� ������ ����� � ������ , ��� ���� ��������
	// � ������, �� ����� ������ ����������, ��� �� ��������.
	std::list<sNode*> list_notTestedNodes;
	list_notTestedNodes.push_back(startNode);

	// ���� � ������ � �����, ������� ������� �� ����� ����� ����� ���.
	// ����� ����������� ���� �� ������ �� �������� �����.

	// ����� ����������� ���� 
	while (!list_notTestedNodes.empty() && currentNode != endNode)
	{
		currentNode = list_notTestedNodes.front();

		// �������� ����� ����� ���
		currentNode->isLearned = true;

		// ���������� ������� ����� �����
		for (auto neighbourNode : currentNode->vector_Neighbours)
		{
			// ���� ����� �� �� ����������, � �� �� ���������, ����� � ������
			if (!neighbourNode->isLearned && neighbourNode->isObstacle == 0)
				list_notTestedNodes.push_back(neighbourNode);

			// ���������� ������� �������� ������� �� ����� �� ������� (�����)
			float maybeLowerGoal = currentNode->distanceAnother + distance(currentNode, neighbourNode);

			if (maybeLowerGoal < neighbourNode->distanceAnother)
			{
				neighbourNode->parent = currentNode;
				neighbourNode->distanceNow = maybeLowerGoal;
				neighbourNode->distanceAnother = neighbourNode->distanceNow + distance(neighbourNode, endNode);
			}
		}

		// ������ ������ ��� ������� ���� ����� � ��������� ��������
		list_notTestedNodes.sort([](const sNode* lhs, const sNode* rhs)
			{
				return lhs->distanceNow < rhs->distanceNow;
			}
		);

		// ������� � ������ ��� �������� �����
		while (!list_notTestedNodes.empty() && list_notTestedNodes.front()->isLearned)
			list_notTestedNodes.pop_front();

		// ���� ����� ���� ����� � ������ - �����
		if (list_notTestedNodes.empty())
			break;
	}
}

int Astar_class::Astar_Update(int s_1_x, int s_1_y, int s_n_1_x, int s_n_1_y, int s_0_x, int s_0_y, int f_x, int f_y)
{
	// ���������� ����� - ������ �����
	startNode = &nodes[s_0_y * W_star + s_0_x];

	// ������� ����� - ��������
	endNode = &nodes[f_y * W_star + f_x];

	// ��������� (��� �����)
	nodes[s_1_y * W_star + s_1_x].isObstacle = true;

	// �������� ������ ��������� � ��������
	nodes[s_n_1_y * W_star + s_n_1_x].isObstacle = false;

	Astar_Solve();

	if (endNode != nullptr)
	{
		sNode* p = endNode;
		sNode* prev = p;

		// ���������� �� �������� �� ������ 
		while (p->parent != nullptr)
		{
			prev = p;
			// ������ ����� - �� �����
			p = p->parent;
		}

		// �������� �������� ������
		if (prev->x > s_0_x)
			return 2;
		else if (prev->x < s_0_x)
			return 1;
		else if (prev->y > s_0_y)
			return 0;
		else if (prev->y < s_0_y)
			return 3;
	}
}