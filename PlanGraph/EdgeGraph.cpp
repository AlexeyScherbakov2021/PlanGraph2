#include "EdgeGraph.h"

	EdgeGraph::EdgeGraph()
	{
		EdgeGraph::point1 = 0;
		EdgeGraph::point2 = 0;
	}

	//------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------
	EdgeGraph::EdgeGraph(int p1, int p2)
	{
		EdgeGraph::point1 = p1;
		EdgeGraph::point2 = p2;
	}


	//------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------
	LPTSTR EdgeGraph::getString()
	{
		int p1 = EdgeGraph::point1;
		int p2 = EdgeGraph::point2;

		if (p2 < p1)
		{
			p2 = EdgeGraph::point1;
			p1 = EdgeGraph::point2;
		}

		_stprintf_s(EdgeGraph::name, 20, _T(" %2d : %2d "), p1, p2);
		return EdgeGraph::name;
	}

	//------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------
	void EdgeGraph::SetPoints(int pt1, int pt2)
	{
		EdgeGraph::point1 = pt1;
		EdgeGraph::point2 = pt2;
	}

	//------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------
	bool EdgeGraph::compareEdge(EdgeGraph *ed)
	{
		if (EdgeGraph::point1 == ed->point1 && EdgeGraph::point2 == ed->point2)
			return TRUE;

		return FALSE;
	}

	int EdgeGraph::IsEntry(int num)
	{
		if (EdgeGraph::point1 == num)
			return EdgeGraph::point2;

		if (EdgeGraph::point2 == num)
			return EdgeGraph::point1;

		return -1;
	}

