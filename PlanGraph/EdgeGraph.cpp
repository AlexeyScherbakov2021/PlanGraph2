#include "EdgeGraph.h"



	EdgeGraph::EdgeGraph()
	{
		EdgeGraph::point1 = 0;
		EdgeGraph::point2 = 0;
	}


	LPTSTR EdgeGraph::getString()
	{
		_stprintf_s(EdgeGraph::name, 20, _T(" %2d : %2d "), EdgeGraph::point1, EdgeGraph::point2);
		//std::string s = std::to_string(EdgeGraph::point1) + " : " + std::to_string(EdgeGraph::point2);
		return EdgeGraph::name;
	}

	void EdgeGraph::SetPoints(int pt1, int pt2)
	{
		EdgeGraph::point1 = pt1;
		EdgeGraph::point2 = pt2;
	}

	//bool EdgeGraph::compareEdge(EdgeGraph *ed)
	//{
	//	if (EdgeGraph::point1 == ed->point1 && EdgeGraph::point2 == ed->point2)
	//		return TRUE;

	//	return FALSE;
	//}