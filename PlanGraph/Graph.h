#pragma once
#include "framework.h"
#include "PlanGraph.h"
#include "EdgeGraph.h"
#include "PointGraph.h"

#define INVALID_EDGE -1
#define CYCLED_SIDE -2
#define EDGE_OK 0

#define MARGIN 30
#define DELTA_Y 50

#define RADIUS_CIRCLE 12

#define LEVEL_TYPE 1
#define RADIAL_TYPE 2

class Graph
{
private:
	std::vector<PointGraph*> listPoint;
	int cntLevels;
	int Width;
	int Height;
	float* levelsX;
	float maxXGraph;
	int type;
	float radius;

	void CalcPoints(int num, PointGraph* parent);
	void CalculateXY();
	float CalculateForPoints(PointGraph* parent);
	int GetListLevel(std::vector<PointGraph*> *points, int level);
	void GetChild(int num, std::vector<PointGraph*>* list_Pt);
	void PaintGraph(HINSTANCE hInstance, HWND hDlg);
	void initPoints();


public:
	std::vector<EdgeGraph> listEdge;
	int cntPoints;
	int cntEdges;

	Graph();
	~Graph();
	int TestValidGraph();
	void RenderGraph(int width, int hight, HINSTANCE hInstance, HWND hDlg, int type);
	void PaintLevel(HDC hdc);
};

