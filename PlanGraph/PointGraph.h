#pragma once
class PointGraph
{
private:
	int number;
	int level;
	int cntChild;
	PointGraph *parent;

public:
	PointGraph();

};

