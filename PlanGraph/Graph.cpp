#include "Graph.h"

Graph::Graph()
{
    cntEdges = 0;
    cntPoints = 0;
    cntLevels = 0;
    Width = 500;
    Height = 500;
}


//-----------------------------------------------------------------------------
void Graph::CalcPoints(int num, PointGraph* parent)
{

    PointGraph* pg = new PointGraph();
    pg->number = num;
    if (parent == NULL)
        pg->level = 0;
    else
        pg->level = parent->level + 1;

    // получение количества уровней
    if (cntLevels < pg->level)
        cntLevels = pg->level;

    pg->parent = parent;
    pg->cntChild = 0;
    listPoint.push_back(pg);

    for (unsigned int i = 0; i < listEdge.size(); i++)
    {
        int point2 = listEdge[i].IsEntry(num);
        if (point2 > 0)
        {
            pg->cntChild++;
            listEdge[i].SetPoints(0, 0);
            CalcPoints(point2, pg);
        }
    }
}

//-----------------------------------------------------------------------------
int Graph::TestValidGraph()
{
    EdgeGraph* ed;
    ed = new EdgeGraph(1, 4);
    listEdge.push_back(*ed);

    ed = new EdgeGraph(1, 3);
    listEdge.push_back(*ed);

    ed = new EdgeGraph(2, 4);
    listEdge.push_back(*ed);

    ed = new EdgeGraph(8, 4);
    listEdge.push_back(*ed);

    ed = new EdgeGraph(3, 6);
    listEdge.push_back(*ed);

    ed = new EdgeGraph(9, 3);
    listEdge.push_back(*ed);

    ed = new EdgeGraph(10, 7);
    listEdge.push_back(*ed);

    ed = new EdgeGraph(5, 3);
    listEdge.push_back(*ed);

    ed = new EdgeGraph(10, 1);
    listEdge.push_back(*ed);

    ed = new EdgeGraph(8, 11);
    listEdge.push_back(*ed);

    ed = new EdgeGraph(8, 12);
    listEdge.push_back(*ed);

    ed = new EdgeGraph(13, 6);
    listEdge.push_back(*ed);

    ed = new EdgeGraph(14, 6);
    listEdge.push_back(*ed);

    cntEdges = listEdge.size();
    cntPoints = cntEdges + 1;

    // число ребер должно быть равно количеству точек - 1
    if (cntPoints - 1 != cntEdges)
        return INVALID_EDGE;

    // все точки должны быть задействованы
    listPoint.clear();
    CalcPoints(1, NULL);

    int* testArray = new int[listPoint.size()];
    ZeroMemory(testArray, listPoint.size() * sizeof(int));
    for (unsigned int i = 0; i < listPoint.size(); i++)
    {
        testArray[listPoint[i]->number - 1]++;
        if (testArray[listPoint[i]->number - 1] > 1)
        {
            return CYCLED_SIDE;
        }
    }

    return EDGE_OK;
}


//-----------------------------------------------------------------------------
void Graph::CalculateXY(int width, int hight)
{
    std::vector<PointGraph*> *points = new std::vector<PointGraph*>();
    unsigned int cntMaxPoints = 0;
    int calcLevel;
    int maxLevel = 0;
    int delta;
    int curX;
    int curY;

    // находим уровень с максимальным количеством точек
    for (int i = 0; i <= cntLevels; i++)
    {
        GetListLevel(points, i);
        if (cntMaxPoints < points->size())
        {
            cntMaxPoints = points->size();
            maxLevel = i;
        }
    }

    // расчет координат Х для максимального уровня
    GetListLevel(points, maxLevel);
    delta = (width - MARGIN - MARGIN) / (cntMaxPoints - 1);
    curX = MARGIN;
    curY = MARGIN + DELTA_Y * maxLevel;

    for(unsigned int i = 0; i < points->size(); i++)
    {
        (*points)[i]->y = curY;
        (*points)[i]->x = curX;
        curX += delta;
    }

    //calcLevel = maxLevel;
    //PointGraph* parent = NULL;

    // расчет координат для уровней ниже
    CalculateBottom(maxLevel, delta);

    // расчет координат для уровней выше
    CalculateTop(maxLevel, delta, width);
    
    delete points;
}
//-----------------------------------------------------------------------------
void Graph::CalculateTop(int calcLevel, int delta, int width)
{
    int curY;
    int curX;
    std::vector<PointGraph*>* childPoints = new std::vector<PointGraph*>();
    std::vector<PointGraph*>* points = new std::vector<PointGraph*>();
    PointGraph* parent = NULL;

    while (--calcLevel >= 0)
    {
        // получение всех точек уровня
        GetListLevel(points, calcLevel);
        curY = MARGIN + DELTA_Y * calcLevel;
        curX = 0;

        for (unsigned int i = 0; i < points->size(); i++)
        {
            (*points)[i]->y = curY;
            // получение дочерних точек
            GetChild((*points)[i]->number, childPoints);
            // получение Х от дочерних точек

            int max = 0;
            int min = INT_MAX;
            for (unsigned int j = 0; j < childPoints->size(); j++)
            {
                if ((*childPoints)[j]->x > max)
                    max = (*childPoints)[j]->x;

                if((*childPoints)[j]->x < min)
                    min = (*childPoints)[j]->x;
            }

            if (min == max)
            {
                if (min > width / 2)
                    curX = min - delta;
                else
                    curX = min + delta;
            }
            else
                curX = min + (max - min) / 2;
            (*points)[i]->x = curX;
        }
    }

    delete points;
    delete childPoints;
}

//-----------------------------------------------------------------------------
void Graph::CalculateBottom(int calcLevel, int delta)
{
    int curY;
    int curX;
    PointGraph* parent = NULL;
    std::vector<PointGraph*>* points = new std::vector<PointGraph*>();

    while (GetListLevel(points, ++calcLevel) > 0)
    {
        curY = MARGIN + DELTA_Y * calcLevel;
        curX = 0;
        for (unsigned int i = 0; i < points->size(); i++)
        {
            (*points)[i]->y = curY;

            if (parent != (*points)[i]->parent)
            {
                parent = (*points)[i]->parent;
                curX = parent->x - delta/3;
                if (curX < MARGIN)
                    curX = MARGIN;
            }

            (*points)[i]->x = curX;
            curX += delta/2;
        }
    }
    delete points;
}


//-----------------------------------------------------------------------------
void Graph::GetChild(int num, std::vector<PointGraph*>* list_Pt)
{
    list_Pt->clear();
    for (unsigned int i = 0; i < listPoint.size(); i++)
    {
        if (listPoint[i]->parent == NULL)
            continue;

        if (listPoint[i]->parent->number == num)
        {
            list_Pt->push_back(listPoint[i]);
        }
    }
}


//-----------------------------------------------------------------------------
int Graph::GetListLevel(std::vector<PointGraph*> *list_Pt, int level)
{
    int cnt = 0;

    list_Pt->clear();
    for (unsigned int i = 0; i < listPoint.size(); i++)
    {
        if (listPoint[i]->level == level)
        {
            list_Pt->push_back(listPoint[i]);
            cnt++;
        }
    }

    return cnt;
}


//-----------------------------------------------------------------------------
void Graph::RenderGraph(int width, int hight, HINSTANCE hInstance, HWND hDlg)
{
    CalculateXY(width, hight);
    PaintGraph(width, hight, hInstance, hDlg);

}
