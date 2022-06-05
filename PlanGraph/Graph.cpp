#include "Graph.h"

// �����������, �������������� ����
Graph::Graph()
{
    cntEdges = 0;
    cntPoints = 0;
    cntLevels = 0;
    Width = 500;
    Height = 500;
    type = LEVEL_TYPE;
}

// ���������� ��� �������� ��������� �������� �� ������
Graph::~Graph()
{
    for (unsigned int i = 0; i < listPoint.size(); i++)
        delete listPoint[i];
}



//-----------------------------------------------------------------------------
// ��������� ������ �����
//-----------------------------------------------------------------------------
void Graph::CalcPoints(int num, PointGraph* parent)
{
    PointGraph* pg = new PointGraph();
    pg->number = num;
    if (parent == NULL)
        pg->level = 0;
    else
        pg->level = parent->level + 1;

    // ��������� ���������� �������
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
            // ��������� ����� ��� ���������� �� ������
            listEdge[i].SetPoints(0, 0);
            CalcPoints(point2, pg);
        }
    }
}

//-----------------------------------------------------------------------------
// ������� ��� ����������� ������������� �����
//-----------------------------------------------------------------------------
void Graph::initPoints()
{
    int listInitEdge[][2] =
    {
        //{1,2},
        //{1,3},
        //{1,4},
        //{2,5},
        //{2,6},
        //{2,7},
        //{3,8},
        //{3,9},
        //{3,10},
        //{4,11},
        //{4,12},
        //{4,13},
        //{4,14},
        //{4,15},
        //{5,16},
        //{5,17},
        //{5,18},
        //{5,19},
        //{5,20},
        //{6,21},
        //{6,22},
        //{7,23},
        //{7,24},
        //{7,25},
        //{9,26},
        //{9,27},
        //{9,28},
        // 
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
//  ������� ����� �� ������������
//-----------------------------------------------------------------------------
int Graph::TestValidGraph()
{
    // ����������� ������������� ����� (������ ��� ������� ������)
    initPoints();

    // ����� ����� ������ ���� ����� ���������� ����� - 1
    if (cntPoints - 1 != cntEdges)
        return INVALID_EDGE;

    // �������� ������ ����� 
    for (unsigned int i = 0; i < listPoint.size(); i++)
        delete listPoint[i];
    listPoint.clear();

    CalcPoints(1, NULL);

    // �������� �� ������������� ���� �����
    // �������� �������
    int* testArray = new int[listPoint.size()];
    // ��������� �������
    ZeroMemory(testArray, listPoint.size() * sizeof(int));
    // ���� ��� �������� ���������� ����� � �������� �������
    for (unsigned int i = 0; i < listPoint.size(); i++)
    {
        testArray[listPoint[i]->number - 1]++;
        if (testArray[listPoint[i]->number - 1] > 1)
        {
            // ���� ������������� ����� ����� ������ ����, �� ��������� ������������
            return CYCLED_SIDE;
        }
    }

    return EDGE_OK;
}

//-----------------------------------------------------------------------------
// ����������� ������ ��������� ��� ���� �����
//-----------------------------------------------------------------------------
double Graph::CalculateForPoints(PointGraph* parent)
{
    std::vector<PointGraph*>* points = new std::vector<PointGraph*>();

    // ���������� ��� ���������� ������������� � ���������� �������� �
    double maxX = 0;
    double minX = 1000000000;

    // ���� ��� �������� �����, ���������� -1
    if (parent->cntChild == 0)
        return -1;

    // �������� ������ �������� �����
    GetChild(parent->number, points);

    // ��������� ��� ���� �������� �����
    for (unsigned int i = 0; i < points->size(); i++)
    {
        // ����������� ����� ��� ������ �������� �����
        double res = CalculateForPoints((*points)[i]);

        if (res < 0)
        {
            // ���� �������� �� ����, �� ���������� � ��������� �������� � �� ������ ���� ����� 0,5 ������
            if (levelsX[(*points)[i]->level] < levelsX[(*points)[i]->level - 1] - 0.5)
            {
                // ���� � �������� ������ ������, �� ������� �� ����
                (*points)[i]->x = levelsX[(*points)[i]->level - 1];
            }
            else
                // ����� ����� ��������� ���������� ������ ������
                (*points)[i]->x = levelsX[(*points)[i]->level];
        }
        else
        {
            // ��� ������� ���������� � �� �������� �����
            (*points)[i]->x = res;
        }
        // ����������� ���������� ������ ������
        levelsX[(*points)[i]->level] = (*points)[i]->x + 1;

        // ���������� ����������� � ������������ �������� �
        if (maxX < (*points)[i]->x)
            maxX = (*points)[i]->x;
        if (minX > (*points)[i]->x)
            minX = (*points)[i]->x;

        // ���������� �������� Y, ��� ����� ������
        (*points)[i]->y = (double)(*points)[i]->level;

    }

    // ���������� ������������ ���������� � � ���������� ������
    if (maxXGraph < maxX)
        maxXGraph = maxX;

    // ���� ���� ����� ���� �����, �� ���������� -1
    if (minX == maxX)
        return -1;

    // �������� �������� ����� �������� ������� ��� �������� � ��� ������������ �����
    double result = (maxX - minX) / 2 + minX;

    return result;

}

//-----------------------------------------------------------------------------
// ������� ��� ������� � � Y �����
//-----------------------------------------------------------------------------
void Graph::CalculateXY()
{
    std::vector<PointGraph*>* points = new std::vector<PointGraph*>();
    unsigned int cntMaxPoints = 0;
    int maxLevel = 0;

    // ������� ������ ��� ���������� ��������� ��������� � ��� ������� ������
    levelsX = new double[cntLevels + 1];
    // �������� ���
    for (int i = 0; i <= cntLevels; i++)
        levelsX[i] = 0;

    // �������� ������������ �������� �
    maxXGraph = 0;
    // ��� ������ ����� ������������� ���������� Y = 0
    listPoint[0]->y = 0;
    // ��������� ���������� ��������� � �������� ��������
    listPoint[0]->x = CalculateForPoints(listPoint[0]);

    if (type == RADIAL_TYPE)
    { 
        // ��������� ��� ����������� ����

        // ������ � ��������� ������� �
        double Deg = 2 * PI / (maxXGraph + 1);
        // ��������� ����� � ����������� ������ ����
        int  minSize = Width;
        if (minSize > Height)
            minSize = Height;

        // ��������� ���������� ������ �����������
        int center = minSize / 2;
        // ��������� ������ ���������� �����������
        radius = (double)((minSize - MARGIN * 2.0) / 2.0) / (double)(cntLevels);
        // ������������ �������� ������� �� ���������� ��� �����������
        for (unsigned int i = 0; i < listPoint.size(); i++)
        {
            listPoint[i]->y = sin(listPoint[i]->x * Deg) * radius * listPoint[i]->level + center;
            listPoint[i]->x = cos(listPoint[i]->x * Deg) * radius * listPoint[i]->level + center;
        }
    }
    else
    {
        // ��������� ��� ���������� ����

        // ����������� ������� ��������� �������� ������ � ��������
        double scaleX = (double)(Width - 2 * MARGIN) / maxXGraph;
        double scaleY = (double)(Height - 2 * MARGIN) / (double)(cntLevels + 1);

        // ������������ ���������� ���� �����
        for (unsigned int i = 0; i < listPoint.size(); i++)
        {
            listPoint[i]->x = listPoint[i]->x * scaleX + MARGIN;
            listPoint[i]->y = listPoint[i]->y * scaleY + MARGIN;
        }
    }

    // �������� ���������� �������
    delete levelsX;
    return;
}



//-----------------------------------------------------------------------------
// ��������� �������� ����� � list_Pt
//-----------------------------------------------------------------------------
void Graph::GetChild(int num, std::vector<PointGraph*>* list_Pt)
{
    // ������� ������ ����� �����������
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
// ��������� ������ ����� ��� ������ � list_Pt
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
// ��������� �����
//-----------------------------------------------------------------------------
void Graph::RenderGraph(int width, int height, HINSTANCE hInstance, HWND hDlg, int type)
{
    // ���������� ������ � ������ � ��� ��������� ��������� ���� � ���������� ������
    Width = width;
    Height = height;
    this->type = type;
    // ������� ���������
    CalculateXY();
    // �������� ���� � ��������� �����
    PaintGraph(hInstance);

}
