#include "Astar_nodes.h"

void Astar_class::Astar_Create()
{
	// Створюю 2Д масив вузлів
	nodes = new sNode[W_star * H_star];

	for (int x = 1; x < W_star; x++)
		for (int y = 3; y < H_star; y++)
		{
			nodes[y * W_star + x].x = x;  // Кожен вузол отримує свої х у координати , розмір в плитках, звертатись:
			nodes[y * W_star + x].y = y;  // nodes[координата_у * Ширина_поля + координата_х].(x/y)
			nodes[y * W_star + x].isObstacle = false;
			nodes[y * W_star + x].parent = nullptr;
			nodes[y * W_star + x].isLearned = false;
		}

	// Створюю зв'язок між вузлами (GRID)
	// Для вузла за координатами додаю сусідній вузол в вектор де він буде сусідом.
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

	// Початкові рандомні координати, потім зміню
	startNode = &nodes[(H_star / 2) * W_star + 1];
	endNode = &nodes[(H_star / 2) * W_star + W_star - 2];
}

// розраховую відстань між вузлом а і б по діагоналі за теоремою піфагора
float Astar_class::distance(sNode* a, sNode* b)
{
	return sqrt((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
}

void Astar_class::Astar_Solve()
{
	// онуляю деякі змінні щоб використати заново з нуля (залишаю перешкоди)
	for (int x = 1; x < W_star; x++)
		for (int y = 3; y < H_star; y++)
		{
			nodes[y * W_star + x].isLearned = false;
			nodes[y * W_star + x].distanceNow = INFINITY;
			nodes[y * W_star + x].distanceAnother = INFINITY;
			nodes[y * W_star + x].parent = nullptr;
		}

	// задаю поточному вузлу дані початкового (голови змійки)
	sNode* currentNode = startNode;
	startNode->distanceAnother = 0.0f;
	startNode->distanceNow = distance(startNode, endNode);

	// початковий вузол одразу додаю в список , щоб його дослідити
	// в процесі, всі вузли будуть додаватись, щоб їх дослідити.
	std::list<sNode*> list_notTestedNodes;
	list_notTestedNodes.push_back(startNode);

	// якщо в списку є вузли, значить існують ще шляхи якими можна піти.
	// пошук припиниться якщо ми дійдемо до кінцевого вузла.

	// шукаю найкоротший шлях 
	while (!list_notTestedNodes.empty() && currentNode != endNode)
	{
		currentNode = list_notTestedNodes.front();

		// дослдіжую вузол тільки раз
		currentNode->isLearned = true;

		// Перевіряємо кожного сусіда вузла
		for (auto neighbourNode : currentNode->vector_Neighbours)
		{
			// якщо вузол ще не досліджений, і він не перешкода, додаю в список
			if (!neighbourNode->isLearned && neighbourNode->isObstacle == 0)
				list_notTestedNodes.push_back(neighbourNode);

			// розраховую можливу найменшу відстань від вузла до нащадка (сусіда)
			float maybeLowerGoal = currentNode->distanceAnother + distance(currentNode, neighbourNode);

			if (maybeLowerGoal < neighbourNode->distanceAnother)
			{
				neighbourNode->parent = currentNode;
				neighbourNode->distanceNow = maybeLowerGoal;
				neighbourNode->distanceAnother = neighbourNode->distanceNow + distance(neighbourNode, endNode);
			}
		}

		// сортую список щоб першими були вузли з найменшою відстанню
		list_notTestedNodes.sort([](const sNode* lhs, const sNode* rhs)
			{
				return lhs->distanceNow < rhs->distanceNow;
			}
		);

		// викидаю зі списка вже досліджені вузли
		while (!list_notTestedNodes.empty() && list_notTestedNodes.front()->isLearned)
			list_notTestedNodes.pop_front();

		// якщо більше нема вузлів в списку - кінець
		if (list_notTestedNodes.empty())
			break;
	}
}

int Astar_class::Astar_Update(int s_1_x, int s_1_y, int s_n_1_x, int s_n_1_y, int s_0_x, int s_0_y, int f_x, int f_y)
{
	// початковий вузол - голова змійки
	startNode = &nodes[s_0_y * W_star + s_0_x];

	// кінцевий вузол - планетка
	endNode = &nodes[f_y * W_star + f_x];

	// перешкоди (тіло змійки)
	nodes[s_1_y * W_star + s_1_x].isObstacle = true;

	// прибираю статус перешкоди з хвостика
	nodes[s_n_1_y * W_star + s_n_1_x].isObstacle = false;

	Astar_Solve();

	if (endNode != nullptr)
	{
		sNode* p = endNode;
		sNode* prev = p;

		// проходжусь від планетки до голови 
		while (p->parent != nullptr)
		{
			prev = p;
			// батько вузла - це вузол
			p = p->parent;
		}

		// визначаю напрямок голови
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