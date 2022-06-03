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
	LPTSTR getString();
	void SetPoints(int pt1, int pt2);
	//bool compareEdge(EdgeGraph* ed);

};

