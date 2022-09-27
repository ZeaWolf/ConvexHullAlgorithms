#include "graphwindow.h"
//
//
//void OnPaint(HWND hWnd) {
//    PAINTSTRUCT ps;
//    BeginPaint(hWnd, &ps);
//    HDC hdc = BeginPaint(hWnd, &ps);
//    // All painting occurs here, between BeginPaint and EndPaint.
//    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 4));
//    EndPaint(hWnd, &ps);
// };
//
////static LRESULT CALLBACK GraphWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
////{
////    switch (Message)
////    {
////    case WM_DESTROY:
////        PostQuitMessage(0);
////        return 0;
////
////    case WM_PAINT:
////        OnPaint(hWnd);
////        return 0;
////    };
////    return 0;
////};
//
//HWND CreateGraph(HWND hwnd)
//{
//    return CreateWindow(L"Graph_Window",
//        L"Convex Hull Algorithms - Graph",
//        WS_CHILDWINDOW | WS_VISIBLE,
//        500, 10, 2000, 5000,
//        hwnd, NULL, GetModuleHandle(NULL), NULL);
//};

        






//void GraphWindow::OnPaint()
//{
//    //HRESULT hr = CreateGraphicsResources();
//    //if (SUCCEEDED(hr))
//    //{
//    PAINTSTRUCT ps;
//    BeginPaint(m_hwnd, &ps);
//    HDC hdc = BeginPaint(m_hwnd, &ps);
//    // All painting occurs here, between BeginPaint and EndPaint.
//    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
//    EndPaint(m_hwnd, &ps);
//    EndPaint(m_hwnd, &ps);
//    //}
//};
//
//LRESULT GraphWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//    switch (uMsg)
//    {
//
//
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        return 0;
//
//    case WM_PAINT:
//        OnPaint();
//        return 0;
//
//    /*case WM_SIZE:
//        Resize();
//        return 0;
//
//    case WM_LBUTTONDOWN:
//        OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
//        return 0;
//
//    case WM_LBUTTONUP:
//        OnLButtonUp();
//        return 0;
//
//    case WM_MOUSEMOVE:
//        OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
//        return 0;
//
//    case WM_SETCURSOR:
//        if (LOWORD(lParam) == HTCLIENT)
//        {
//            SetCursor(hCursor);
//            return TRUE;
//        }
//        break;
//
//    case WM_KEYDOWN:
//        OnKeyDown((UINT)wParam);
//        return 0;
//
//    case WM_COMMAND:
//        switch (LOWORD(wParam))
//        {
//        case ID_DRAW_MODE:
//            SetMode(DrawMode);
//            break;
//
//        case ID_SELECT_MODE:
//            SetMode(SelectMode);
//            break;
//
//        case ID_TOGGLE_MODE:
//            if (mode == DrawMode)
//            {
//                SetMode(SelectMode);
//            }
//            else
//            {
//                SetMode(DrawMode);
//            }
//            break;
//
//        case ID_MD_BUTTON:
//            MessageBox(m_hwnd, L"click", L"title", MB_ICONINFORMATION);
//            break;
//
//        case ID_MS_BUTTON:
//            MessageBox(m_hwnd, L"1111click", L"111title", MB_ICONINFORMATION);
//            break;
//
//        }
//        return 0;*/
//    }
//    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
//};