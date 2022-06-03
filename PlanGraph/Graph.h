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

class Graph
{
private:
	std::vector<PointGraph*> listPoint;
	int cntLevels;
	int Width;
	int Height;

	void CalcPoints(int num, PointGraph* parent);
	void CalculateXY(int width, int hight);
	int GetListLevel(std::vector<PointGraph*> *points, int level);
	void GetChild(int num, std::vector<PointGraph*>* list_Pt);
	void CalculateTop(int calcLevel, int delta, int width);
	void CalculateBottom(int calcLevel,int delta);
	void PaintGraph(int width, int hight, HINSTANCE hInstance, HWND hDlg);

public:
	std::vector<EdgeGraph> listEdge;
	int cntPoints;
	int cntEdges;

	Graph();
	int TestValidGraph();
	void RenderGraph(int width, int hight, HINSTANCE hInstance, HWND hDlg);
	void PaintLevel(HDC hdc);
};

