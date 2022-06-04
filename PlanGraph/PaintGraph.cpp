#include "framework.h"
#include "Graph.h"

extern Graph graph;

TCHAR szClass[] = _T("classGraph");

LRESULT  CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
ATOM MyRegisterClass(HINSTANCE hInstance);


void Graph::PaintGraph(HINSTANCE hInstance, HWND hDlg)
{
    ATOM res = MyRegisterClass(hInstance);

    int wScreen = GetSystemMetrics(SM_CXSCREEN);
    int hScreen = GetSystemMetrics(SM_CYSCREEN);


    HWND hWnd = CreateWindow(szClass, _T("Планарный граф"), WS_OVERLAPPEDWINDOW,
        (wScreen - Width) / 2, (hScreen - Height) / 2, Width + 10, Height + 10, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return;
    }

    ShowWindow(hWnd, SW_SHOWDEFAULT);
    UpdateWindow(hWnd);
}



LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT lpRect;
    LONG x, y;

    switch (message)
    {

    case WM_INITDIALOG:
        x = GetSystemMetrics(SM_CXSCREEN);
        y = GetSystemMetrics(SM_CYSCREEN);
        GetWindowRect(hWnd, &lpRect);
        SetWindowPos(hWnd, HWND_TOP, (x - lpRect.right) / 2, (y - lpRect.bottom) / 2, 200, 200, SWP_NOSIZE | SWP_SHOWWINDOW);

        return (INT_PTR)TRUE;


    case WM_PAINT:
        {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
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


void Graph::PaintLevel(HDC hdc)
{
    int x;
    int y;

    for (unsigned int i = 0; i < listPoint.size(); i++)
    {
        if (listPoint[i]->parent != NULL)
        {
            MoveToEx(hdc, (int)listPoint[i]->x, (int)listPoint[i]->y, NULL);
            LineTo(hdc, (int)listPoint[i]->parent->x, (int)listPoint[i]->parent->y);
        }
    }

    RECT rc;
    TCHAR buffer[4];

    for (unsigned int i = 0; i < listPoint.size(); i++)
    {
        x = (int)listPoint[i]->x - RADIUS_CIRCLE;
        y = (int)listPoint[i]->y - RADIUS_CIRCLE;
        Ellipse(hdc, x, y, (int)listPoint[i]->x + RADIUS_CIRCLE, (int)listPoint[i]->y + RADIUS_CIRCLE);
        rc.left = x;
        rc.top = y;
        rc.right = x + RADIUS_CIRCLE * 2;
        rc.bottom = y + RADIUS_CIRCLE * 2;

        _itot_s(listPoint[i]->number, buffer, 4, 10);
        DrawText(hdc, buffer, lstrlen(buffer), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

}
