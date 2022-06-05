#pragma once
#include "framework.h"


class EdgeGraph
{
private:
	int point1;				// номер первой точки ребра
	int point2;				// номер второй точки ребра
	TCHAR name[20];			// строка для списка в ListBox
public:
	EdgeGraph();
	EdgeGraph(int p1, int p2);
	LPTSTR getString();
	void SetPoints(int pt1, int pt2);
	bool compareEdge(EdgeGraph* ed);
	int IsEntry(int num);

};

