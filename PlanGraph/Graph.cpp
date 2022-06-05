#include "Graph.h"

// конструктор, инициализируем поля
Graph::Graph()
{
    cntEdges = 0;
    cntPoints = 0;
    cntLevels = 0;
    Width = 500;
    Height = 500;
    type = LEVEL_TYPE;
}

// деструктор для удаления созданных объектов из памяти
Graph::~Graph()
{
    for (unsigned int i = 0; i < listPoint.size(); i++)
        delete listPoint[i];
}



//-----------------------------------------------------------------------------
// формирует список точек
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
            // обнуление ребра для исключения из поиска
            listEdge[i].SetPoints(0, 0);
            CalcPoints(point2, pg);
        }
    }
}

//-----------------------------------------------------------------------------
// фнкуция для программной инициализации ребер
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
//  проврка графа на правильность
//-----------------------------------------------------------------------------
int Graph::TestValidGraph()
{
    // программная инициализация ребер (убрать для рабочей версии)
    initPoints();

    // число ребер должно быть равно количеству точек - 1
    if (cntPoints - 1 != cntEdges)
        return INVALID_EDGE;

    // очищение списка точек 
    for (unsigned int i = 0; i < listPoint.size(); i++)
        delete listPoint[i];
    listPoint.clear();

    CalcPoints(1, NULL);

    // проверка на использование всех точек
    // созданеи массива
    int* testArray = new int[listPoint.size()];
    // обнуление массива
    ZeroMemory(testArray, listPoint.size() * sizeof(int));
    // цикл для подсчета количества точек в соданном массиве
    for (unsigned int i = 0; i < listPoint.size(); i++)
    {
        testArray[listPoint[i]->number - 1]++;
        if (testArray[listPoint[i]->number - 1] > 1)
        {
            // если задействована точка более одного раза, то произошло зацикливание
            return CYCLED_SIDE;
        }
    }

    return EDGE_OK;
}

//-----------------------------------------------------------------------------
// рекурсивный расчет координат для всех точек
//-----------------------------------------------------------------------------
double Graph::CalculateForPoints(PointGraph* parent)
{
    std::vector<PointGraph*>* points = new std::vector<PointGraph*>();

    // переменные для накопления максимального и минимально значения Х
    double maxX = 0;
    double minX = 1000000000;

    // если нет дочерних точек, возвращаем -1
    if (parent->cntChild == 0)
        return -1;

    // получаем список дочерних точек
    GetChild(parent->number, points);

    // выполняем для всех дочерних точек
    for (unsigned int i = 0; i < points->size(); i++)
    {
        // рекурсивный вызов для каждой дочерней точки
        double res = CalculateForPoints((*points)[i]);

        if (res < 0)
        {
            // если дочерних не было, то сравниваем с последнее значение Х из уровня выше минус 0,5 единиц
            if (levelsX[(*points)[i]->level] < levelsX[(*points)[i]->level - 1] - 0.5)
            {
                // если Х верхнего уровня больше, то считаем от него
                (*points)[i]->x = levelsX[(*points)[i]->level - 1];
            }
            else
                // иначе берем следующую координату своего уровня
                (*points)[i]->x = levelsX[(*points)[i]->level];
        }
        else
        {
            // был возврат координаты Х от дочерних точек
            (*points)[i]->x = res;
        }
        // увеличиваем координату своего уровня
        levelsX[(*points)[i]->level] = (*points)[i]->x + 1;

        // запоминаем минимальное и максимальное значение Х
        if (maxX < (*points)[i]->x)
            maxX = (*points)[i]->x;
        if (minX > (*points)[i]->x)
            minX = (*points)[i]->x;

        // записываем значение Y, оно равно уровню
        (*points)[i]->y = (double)(*points)[i]->level;

    }

    // запоминаем максимальную координату Х в переменной класса
    if (maxXGraph < maxX)
        maxXGraph = maxX;

    // если была всего одна точка, то возвращаем -1
    if (minX == maxX)
        return -1;

    // получаем середину между крайними точками для передачи Х для родительской точки
    double result = (maxX - minX) / 2 + minX;

    return result;

}

//-----------------------------------------------------------------------------
// функция для расчета Х и Y точек
//-----------------------------------------------------------------------------
void Graph::CalculateXY()
{
    std::vector<PointGraph*>* points = new std::vector<PointGraph*>();
    unsigned int cntMaxPoints = 0;
    int maxLevel = 0;

    // создаем массив для сохранения последних координат Х для каждого уровня
    levelsX = new double[cntLevels + 1];
    // обнуляем его
    for (int i = 0; i <= cntLevels; i++)
        levelsX[i] = 0;

    // обнуляем максимальное значение Х
    maxXGraph = 0;
    // для первой точки устанавливаем координату Y = 0
    listPoint[0]->y = 0;
    // выполняем вычисление координат в условных единицах
    listPoint[0]->x = CalculateForPoints(listPoint[0]);

    if (type == RADIAL_TYPE)
    { 
        // выполняем для радиального типа

        // радиан в расчетной единице Х
        double Deg = 2 * PI / (maxXGraph + 1);
        // вписывать будем в минимальный размер окна
        int  minSize = Width;
        if (minSize > Height)
            minSize = Height;

        // вычисляем координаты центра окружностей
        int center = minSize / 2;
        // вычисляем радиус приращения окружностей
        radius = (double)((minSize - MARGIN * 2.0) / 2.0) / (double)(cntLevels);
        // пересчитваем условные единицы на координаты для окружностей
        for (unsigned int i = 0; i < listPoint.size(); i++)
        {
            listPoint[i]->y = sin(listPoint[i]->x * Deg) * radius * listPoint[i]->level + center;
            listPoint[i]->x = cos(listPoint[i]->x * Deg) * radius * listPoint[i]->level + center;
        }
    }
    else
    {
        // выполняем для уровневого типа

        // рассчитваем масштаб пересчета условных единиц в экранные
        double scaleX = (double)(Width - 2 * MARGIN) / maxXGraph;
        double scaleY = (double)(Height - 2 * MARGIN) / (double)(cntLevels + 1);

        // пересчитваем координаты всех точек
        for (unsigned int i = 0; i < listPoint.size(); i++)
        {
            listPoint[i]->x = listPoint[i]->x * scaleX + MARGIN;
            listPoint[i]->y = listPoint[i]->y * scaleY + MARGIN;
        }
    }

    // удаление созданного массива
    delete levelsX;
    return;
}



//-----------------------------------------------------------------------------
// получение дочерних точек в list_Pt
//-----------------------------------------------------------------------------
void Graph::GetChild(int num, std::vector<PointGraph*>* list_Pt)
{
    // очистка списка перед заполнением
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
// получение списка точек для уровня в list_Pt
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
void Graph::RenderGraph(int width, int height, HINSTANCE hInstance, HWND hDlg, int type)
{
    // запоминаем ширину и высоту и тип отрисовки заданного окна в переменных класса
    Width = width;
    Height = height;
    this->type = type;
    // рассчет координат
    CalculateXY();
    // создание окна и рисование графа
    PaintGraph(hInstance);

}
