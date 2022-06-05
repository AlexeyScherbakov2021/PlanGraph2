#pragma once
class PointGraph
{
public:
	int number;
	int level;
	int cntChild;
	PointGraph *parent;
	double x;
	double y;

public:
	PointGraph();

};

