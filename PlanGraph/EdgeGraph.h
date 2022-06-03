#pragma once
#include "framework.h"


class EdgeGraph
{
private:
	int point1;
	int point2;
	TCHAR name[20];
public:
	EdgeGraph();
	EdgeGraph(int p1, int p2);
	LPTSTR getString();
	void SetPoints(int pt1, int pt2);
	bool compareEdge(EdgeGraph* ed);
	int IsEntry(int num);

};

