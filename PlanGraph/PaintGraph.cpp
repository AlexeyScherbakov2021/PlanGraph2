#include "framework.h"
#include "Graph.h"

extern Graph graph;

TCHAR szClass[] = _T("classGraph");

LRESULT  CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
ATOM MyRegisterClass(HINSTANCE hInstance);


void Graph::PaintGraph(HINSTANCE hInstance, HWND hDlg)
{
    // регистрация класса окна
    ATOM res = MyRegisterClass(hInstance);

    // ширина и высота экрана
    int wScreen = GetSystemMetrics(SM_CXSCREEN);
    int hScreen = GetSystemMetrics(SM_CYSCREEN);

    // создание окна
    HWND hWnd = CreateWindow(szClass, _T("Планарный граф"), WS_OVERLAPPEDWINDOW,
        (wScreen - Width) / 2, (hScreen - Height) / 2, Width + 20, Height + 40, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
        return;

    // отображение окна
    ShowWindow(hWnd, SW_SHOWDEFAULT);
    UpdateWindow(hWnd);
}


//------------------------------------------------------------------------------------
// фнукция обратного вызова для обработки событий окна
//------------------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //RECT lpRect;
    //LONG x, y;

    switch (message)
    {
        // событие отрисоки окна
    case WM_PAINT:
        {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // вывзываем функцию рисования
        graph.PaintLevel(hdc);
        EndPaint(hWnd, &ps);
        }
        break;
       

    case WM_DESTROY:
        break;

    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);

    }

    return FALSE;
}


//------------------------------------------------------------------------------------
// функция регистрации класса окна
//------------------------------------------------------------------------------------
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}


//------------------------------------------------------------------------------------
// отрисовка графа
//------------------------------------------------------------------------------------
void Graph::PaintLevel(HDC hdc)
{
    int x;
    int y;

    if (type == RADIAL_TYPE)
    {
        // для радиального типа рисуем концентрические окружности

        // для удобста вводим диаметр для максимального уровня
        int diametr = (int)(radius * 2 * cntLevels);

        // будем вписывать в минимальлный размер
        int minSize = Width;
        if (minSize > Height)
            minSize = Height;

        // выбираем объект пера для рисования
        HGDIOBJ original = SelectObject(hdc, GetStockObject(DC_PEN));
        // задаем серый цвет
        SetDCPenColor(hdc, RGB(192, 192, 192));

        // рисование окружностей
        for (int i = 0; i < cntLevels; i++)
        {
            int left = (minSize - diametr) / 2;
            int right = left + diametr;
            Ellipse(hdc, left, left, right, right);
            // уменьшаем радиус
            diametr -= (int)(radius * 2);
        }
        // восстанавливаем объект пера рисования
        SelectObject(hdc, original);
    }

    // рисование линий соединения точек
    for (unsigned int i = 0; i < listPoint.size(); i++)
    {
        // рисуем от дочерней к родительской точке, поэтому точку без родителя пропускаем
        if (listPoint[i]->parent != NULL)
        {
            // переход в координату точки
            MoveToEx(hdc, (int)listPoint[i]->x, (int)listPoint[i]->y, NULL);
            // рисование линии в координату родительской точки
            LineTo(hdc, (int)listPoint[i]->parent->x, (int)listPoint[i]->parent->y);
        }
    }

    RECT rc;
    TCHAR buffer[4];

    // рисование точек
    for (unsigned int i = 0; i < listPoint.size(); i++)
    {
        // вычисляем координату для окружности точки
        x = (int)listPoint[i]->x - RADIUS_CIRCLE;
        y = (int)listPoint[i]->y - RADIUS_CIRCLE;
        // рисуем окружность
        Ellipse(hdc, x, y, (int)listPoint[i]->x + RADIUS_CIRCLE, (int)listPoint[i]->y + RADIUS_CIRCLE);

        // заполняем координаты квадрата для вывода текста
        rc.left = x;
        rc.top = y;
        rc.right = x + RADIUS_CIRCLE * 2;
        rc.bottom = y + RADIUS_CIRCLE * 2;

        // преобразуем номер точки в строку
        _itot_s(listPoint[i]->number, buffer, 4, 10);
        // рисуем номер точки
        DrawText(hdc, buffer, lstrlen(buffer), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

}
