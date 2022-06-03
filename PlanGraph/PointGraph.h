#pragma once
class PointGraph
{
public:
	int number;
	int level;
	int cntChild;
	PointGraph *parent;
	int x;
	int y;

public:
	PointGraph();

};

