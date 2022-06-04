#pragma once
class PointGraph
{
public:
	int number;
	int level;
	int cntChild;
	PointGraph *parent;
	float x;
	float y;

public:
	PointGraph();

};

