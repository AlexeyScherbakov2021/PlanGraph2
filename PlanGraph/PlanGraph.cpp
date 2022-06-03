// PlanGraph.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "PlanGraph.h"
#include "EdgeGraph.h"
#include "PointGraph.h"
#include "Graph.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HWND hDlg = NULL;
HWND listBox = NULL;
HWND combo1 = NULL;
HWND combo2 = NULL;
//int cntPoints = 0;
std::vector<EdgeGraph> listEdge;
std::vector<PointGraph*> listPoint;
bool IsTestValid = FALSE;
Graph graph;

// Отправить объявления функций, включенных в этот модуль кода:
//ATOM                MyRegisterClass(HINSTANCE hInstance);
//BOOL                InitInstance(HINSTANCE, int);
INT_PTR CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//void removeFromList(EdgeGraph* ed);
bool TestValid();
//void CalcPoints(int num, PointGraph* parent);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    //LoadStringW(hInstance, IDC_PLANGRAPH, szWindowClass, MAX_LOADSTRING);
    //MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    /*if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }*/

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PLANGRAPH));

    MSG msg;

    hDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, WndProc);

    listBox = GetDlgItem(hDlg, IDC_LIST_EDGE);
    combo1 = GetDlgItem(hDlg, IDC_COMBO_T1);
    combo2 = GetDlgItem(hDlg, IDC_COMBO_T2);

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;

}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
/*ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PLANGRAPH));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PLANGRAPH);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}
*/
//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
/*BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}
*/

//-----------------------------------------------------------------------------
INT_PTR CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT lpRect;
    LONG x, y;
    int point1;
    int point2;
    TCHAR buf[20];
    EdgeGraph* edge;
    LPTSTR s;

    switch (message)
    {
    case WM_INITDIALOG:
        x = GetSystemMetrics(SM_CXSCREEN);
        y = GetSystemMetrics(SM_CYSCREEN);
        GetWindowRect(hWnd, &lpRect);
        SetWindowPos(hWnd, HWND_TOP, (x - lpRect.right) / 2, (y - lpRect.bottom) / 2, 200, 200, SWP_NOSIZE | SWP_SHOWWINDOW);

        return (INT_PTR)TRUE;


    case WM_CTLCOLORSTATIC:
        if (((HWND)lParam) == GetDlgItem(hWnd, IDC_STATIC_TEST))
        {
            if(IsTestValid)
                SetTextColor((HDC)wParam, RGB(0, 128, 0));
            else
                SetTextColor((HDC)wParam, RGB(255, 0, 0));
            SetBkMode((HDC)wParam, TRANSPARENT);
            return (LRESULT)GetStockObject(COLOR_WINDOW);
        }
        break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {

            case IDCANCEL:
                DestroyWindow(hWnd);
                break;

            case IDC_EDIT_COUNT:

                if (HIWORD(wParam) == 0x200)
                {
                    // потеря фокуса для количества точек
                    graph.cntPoints = GetDlgItemInt(hWnd, IDC_EDIT_COUNT, FALSE, FALSE);

                    // получить combo T1
                    ComboBox_ResetContent(combo1);
                    ComboBox_ResetContent(combo2);
                    // удаление ссылок на ребра
                    while (ListBox_GetCount(listBox))
                    {
                        edge = (EdgeGraph*)ListBox_GetItemData(listBox, 0);
                        delete edge;
                        x = ListBox_DeleteString(listBox, 0);
                    }

                    for (int i = 1; i <= graph.cntPoints; i++)
                    {
                        _itot_s(i, buf, 10);
                        ComboBox_AddString(combo1, buf);
                        ComboBox_AddString(combo2, buf);
                    }
                }

                break;

                // добавление ребра в список
            case IDC_BUTTON_ADD:

                point1 = ComboBox_GetCurSel(combo1) + 1;
                point2 = ComboBox_GetCurSel(combo2) + 1;

                edge = new EdgeGraph();
                edge->SetPoints(point1, point2);
                s = edge->getString();
                // проверка на уже вставленную строку
                if( ListBox_FindString(listBox, -1, s) < 0 && point1 != point2)
                {
                    x = ListBox_AddString(listBox, s);
                    ListBox_SetItemData(listBox, x, edge);
                    IsTestValid = TestValid();
                }

                break;

            case IDC_BUTTON_RENDER:
                graph.RenderGraph(500, 500, hInst, hDlg);

                break;

            case IDC_BUTTON_DELETE:
                x = ListBox_GetCurSel(listBox);
                if (x >= 0)
                {
                    edge = (EdgeGraph*)ListBox_GetItemData(listBox, x);
                    delete edge;
                    ListBox_DeleteString(listBox, x);
                }
                IsTestValid = TestValid();
                break;

            case IDC_BUTTON_TEST:
                IsTestValid = TestValid();
                break;

            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;

            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;

            default:
                return FALSE;
            }
        }
        break;

    //case WM_PAINT:
    //    {
    //        PAINTSTRUCT ps;
    //        HDC hdc = BeginPaint(hWnd, &ps);
    //        // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
    //        EndPaint(hWnd, &ps);
    //    }
    //    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        //return DefWindowProc(hWnd, message, wParam, lParam);
        return FALSE;
    }
    return FALSE;
}


// Обработчик сообщений для окна "О программе".
//-----------------------------------------------------------------------------
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

//-----------------------------------------------------------------------------
bool TestValid()
{
    //bool result = TRUE;
    EdgeGraph* ed;

    HWND hwnd = GetDlgItem(hDlg, IDC_STATIC_TEST);

    graph.cntEdges = ListBox_GetCount(listBox);

    graph.listEdge.clear();
    for (int i = 0; i < graph.cntEdges; i++)
    {
        ed = (EdgeGraph*)ListBox_GetItemData(listBox, i);
        graph.listEdge.push_back(*ed);
    }

    int res = graph.TestValidGraph();

    if (res == 0)
    {
        SetDlgItemText(hDlg, IDC_STATIC_TEST, _T("Корректно."));
        ShowWindow(hwnd, FALSE);
        ShowWindow(hwnd, TRUE);
        return TRUE;
    }

    if (res == CYCLED_SIDE)
    {
        SetDlgItemText(hDlg, IDC_STATIC_TEST, _T("Есть циклические грани."));
        ShowWindow(hwnd, FALSE);
        ShowWindow(hwnd, TRUE);
    }

    if (res == INVALID_EDGE)
    {
        SetDlgItemText(hDlg, IDC_STATIC_TEST, _T("Неверное количество ребер."));
        ShowWindow(hwnd, FALSE);
        ShowWindow(hwnd, TRUE);
    }

    return FALSE;
}
