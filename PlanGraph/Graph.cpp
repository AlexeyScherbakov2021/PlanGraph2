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

void Graph::initPoints()
{
    int listInitEdge[][2] =
    {
        {1,2},
        {1,3},
        {2,4},
        {2,5},
        {2,6},
        {3,7},
        {3,8},
        {4,9},
        {4,10},
        {4,11},
        {5,12},
        {5,13},
        {5,14},
        {5,15},
        {8,16},
        {8,17},
        {8,18},
        {9,19},
        {9,20},
        {11,21},
        {11,22},
        {12,23},
        {12,24}
    };

    int n = sizeof(listInitEdge) / 8;

    EdgeGraph* ed;

    for (int i = 0; i < n; i++)
    {
        ed = new EdgeGraph(listInitEdge[i][0], listInitEdge[i][1]);
        listEdge.push_back(*ed);

    }

    cntEdges = listEdge.size();
    cntPoints = cntEdges + 1;

}

//-----------------------------------------------------------------------------
int Graph::TestValidGraph()
{
    initPoints();

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
float Graph::CalculateForPoints(PointGraph* parent)
{
    std::vector<PointGraph*>* points = new std::vector<PointGraph*>();

    float maxX = 0;
    float minX = 1000000000;

    // если нет дочерних точек, возвращаем -1
    if (parent->cntChild == 0)
        return -1;

    // получаем список дочерних точек
    GetChild(parent->number, points);

    for (unsigned int i = 0; i < points->size(); i++)
    {
        // рекурсивный вызов для каждой дочерней точки
        float res = CalculateForPoints((*points)[i]);

        if (res < 0)
        {
            // если дочерних не было, то 
            if (levelsX[(*points)[i]->level] < levelsX[(*points)[i]->level - 1])
            {
                // если Х верхнего уровня больше, то считаем от него
                (*points)[i]->x = levelsX[(*points)[i]->level - 1] + 1;
            }
            else
                // иначе берем следующую координату своего уровня
                (*points)[i]->x = levelsX[(*points)[i]->level];
        }
        else
        {
            // был возврат координвты Х от дочерних точек
            (*points)[i]->x = res;
        }
        // увеличиваем координату своего уровня
        levelsX[(*points)[i]->level] = (*points)[i]->x + 1;

        if (maxX < (*points)[i]->x)
            maxX = (*points)[i]->x;
        if (minX > (*points)[i]->x)
            minX = (*points)[i]->x;

        (*points)[i]->y = (float)(*points)[i]->level;

    }

    // запоминаем максимальную координату Х
    if (maxXGraph < maxX)
        maxXGraph = maxX;

    // если была всего одна точка
    if (minX == maxX)
        return -1;

    // получаем середину между крайними точками
    float result = (maxX - minX) / 2 + minX;

    return result;

}


//-----------------------------------------------------------------------------
void Graph::CalculateXY(int width, int height)
{
    std::vector<PointGraph*>* points = new std::vector<PointGraph*>();
    unsigned int cntMaxPoints = 0;
    //int calcLevel;
    int maxLevel = 0;
    //int delta;
    //int curX;
    //int curY;

    levelsX = new float[cntLevels + 1];
    for (int i = 0; i <= cntLevels; i++)
        levelsX[i] = 0;

    maxXGraph = 0;

    listPoint[0]->x = CalculateForPoints(listPoint[0]);

    float scaleX = (float)(width - 2 * MARGIN) / maxXGraph;
    float scaleY = (float)(height - 2 * MARGIN) / (float)(cntLevels + 1);

    for (unsigned int i = 0; i < listPoint.size(); i++)
    {
        listPoint[i]->x = listPoint[i]->x * scaleX + MARGIN;
        listPoint[i]->y = listPoint[i]->y * scaleY + MARGIN;
    }

    delete levelsX;
    return;
}



//-----------------------------------------------------------------------------
// получение дочерних точек
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
// получение списка точек для уровня
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
// рисование графа
//-----------------------------------------------------------------------------
void Graph::RenderGraph(int width, int hight, HINSTANCE hInstance, HWND hDlg)
{
    CalculateXY(width, hight);
    PaintGraph(width, hight, hInstance, hDlg);

}
