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

#define PI 3.14159265

class Graph
{
private:
	std::vector<PointGraph*> listPoint;		// список точек
	int cntLevels;							// число уровней
	int Width;								// ширина окна вывода
	int Height;								// высота окна вывода
	double* levelsX;							// указатель на массив с координатами ’ дл€ уровней
	double maxXGraph;						// максимальна€ координата ’ дл€ возможности вписани€ графа в окно
	int type;								// тип отрисовки графа
	double radius;							// радиус приращени€ дл€ радиального графа

	void CalcPoints(int num, PointGraph* parent);
	void CalculateXY();
	double CalculateForPoints(PointGraph* parent);
	int GetListLevel(std::vector<PointGraph*> *points, int level);
	void GetChild(int num, std::vector<PointGraph*>* list_Pt);
	void PaintGraph(HINSTANCE hInstance, HWND hDlg);
	void initPoints();


public:
	std::vector<EdgeGraph> listEdge;
	int cntPoints;							// количество точек
	int cntEdges;							// количество ребер

	Graph();
	~Graph();
	int TestValidGraph();
	void RenderGraph(int width, int hight, HINSTANCE hInstance, HWND hDlg, int type);
	void PaintLevel(HDC hdc);
};

