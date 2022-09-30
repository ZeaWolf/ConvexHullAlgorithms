#include "covexhullwindow.h"

void MainWindow::CreateLayout()
{
    //RECT rc;
    //GetClientRect(m_hwnd, &rc);

    //gwnd = CreateWindowEx(
    //    0,							// Optional window styles.
    //    L"Static",					    // Window class
    //    L"Graph",	                            // Window text
    //    WS_CHILD | WS_VISIBLE,				// Window style
    //    0, 0, 350, rc.bottom,                     // Size and position
    //    m_hwnd,									// Parent window
    //    (HMENU)ID_GRAPH,					    // Menu
    //    GetModuleHandle(NULL),					// Instance handle
    //    NULL									// Additional application data
    //);

    hb1 = CreateWindowEx(//windowW
        0,										// Optional window styles.
        L"Button",					            // Window class
        L"Minkowski Difference",	            // Window text
        WS_CHILD | WS_VISIBLE, // | BS_OWNERDRAW,					// Window style
        20, 30, 300, 50,                        // Size and position
        m_hwnd,									// Parent window
        (HMENU)ID_MD_BUTTON,					// Menu
        GetModuleHandle(NULL),					// Instance handle
        NULL									// Additional application data
    );

    hb2 = CreateWindowEx(
        0,										// Optional window styles.
        L"Button",					            // Window class
        L"Minkowski Sum",	                    // Window text
        WS_CHILD | WS_VISIBLE, // | BS_OWNERDRAW,					// Window style
        20, 130, 300, 50,                       // Size and position
        m_hwnd,									// Parent window
        (HMENU)ID_MS_BUTTON,					// Menu
        GetModuleHandle(NULL),					// Instance handle
        NULL									// Additional application data
    );

    hb3 = CreateWindowEx(
        0,										// Optional window styles.
        L"Button",					            // Window class
        L"Quickhull",	                        // Window text
        WS_CHILD | WS_VISIBLE, // | BS_OWNERDRAW,					// Window style
        20, 230, 300, 50,                       // Size and position
        m_hwnd,									// Parent window
        (HMENU)ID_Q_BUTTON,					    // Menu
        GetModuleHandle(NULL),					// Instance handle
        NULL									// Additional application data
    );

    hb4 = CreateWindowEx(
        0,										// Optional window styles.
        L"Button",					            // Window class
        L"Point Convex Hull",	                // Window text
        WS_CHILD | WS_VISIBLE, // | BS_OWNERDRAW,					// Window style
        20, 330, 300, 50,                       // Size and position
        m_hwnd,									// Parent window
        (HMENU)ID_PCH_BUTTON,					// Menu
        GetModuleHandle(NULL),					// Instance handle
        NULL									// Additional application data
    );

    hb5 = CreateWindowEx(
        0,										// Optional window styles.
        L"Button",					            // Window class
        L"GJK",	                                // Window text
        WS_CHILD | WS_VISIBLE, // | BS_OWNERDRAW,					// Window style
        20, 430, 300, 50,                       // Size and position
        m_hwnd,									// Parent window
        (HMENU)ID_GJK_BUTTON,					// Menu
        GetModuleHandle(NULL),					// Instance handle
        NULL									// Additional application data
    );
};

void MainWindow::DrawGraph(ID2D1HwndRenderTarget* pRT, ID2D1SolidColorBrush* pB){
    pB->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
    D2D1_SIZE_F size = pRenderTargetM->GetSize();

    float o[4];
    GetGraphInfo(o);

    for (int i = 0; i < OUT_LIMIT; i++)
    {
        if (i == 0) {
            pRenderTargetM->DrawLine(
                D2D1::Point2F(DPIScale::PixelsToDipsX(o[0]), 0.0f),
                D2D1::Point2F(DPIScale::PixelsToDipsX(o[0]), size.height),
                pBrushM,
                1.5f
            );
            pRenderTargetM->DrawLine(
                D2D1::Point2F(DPIScale::PixelsToDipsX((float)BOUNDARY), DPIScale::PixelsToDipsY(o[1])),
                D2D1::Point2F(size.width, DPIScale::PixelsToDipsY(o[1])),
                pBrushM,
                1.5f
            );
        }
        else {
            pRenderTargetM->DrawLine(
                D2D1::Point2F(DPIScale::PixelsToDipsX(o[0] + (i*o[2])), 0.0f),
                D2D1::Point2F(DPIScale::PixelsToDipsX(o[0] + (i*o[2])), size.height),
                pBrushM,
                0.5f
            );
            pRenderTargetM->DrawLine(
                D2D1::Point2F(DPIScale::PixelsToDipsX((float)BOUNDARY), DPIScale::PixelsToDipsY(o[1] + (i * o[3]))),
                D2D1::Point2F(size.width, DPIScale::PixelsToDipsY(o[1] + (i * o[3]))),
                pBrushM,
                0.5f
            );
            pRenderTargetM->DrawLine(
                D2D1::Point2F(DPIScale::PixelsToDipsX(o[0] - (i * o[2])), 0.0f),
                D2D1::Point2F(DPIScale::PixelsToDipsX(o[0] - (i * o[2])), size.height),
                pBrushM,
                0.5f
            );
            pRenderTargetM->DrawLine(
                D2D1::Point2F(DPIScale::PixelsToDipsX((float)BOUNDARY), DPIScale::PixelsToDipsY(o[1] - (i * o[3]))),
                D2D1::Point2F(size.width, DPIScale::PixelsToDipsY(o[1] - (i * o[3]))),
                pBrushM,
                0.5f
            );
        }
    }
}


//HRESULT MainWindow::CreateGraphicsResourcesG()
//{
//    HRESULT hr = S_OK;
//    if (pRenderTargetG == NULL)
//    {
//        RECT rc;
//        GetClientRect(gwnd, &rc);
//
//        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
//
//        hr = pFactory->CreateHwndRenderTarget(
//            D2D1::RenderTargetProperties(),
//            D2D1::HwndRenderTargetProperties(gwnd, size),
//            &pRenderTargetG);
//
//        if (SUCCEEDED(hr))
//        {
//            const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0); //
//            hr = pRenderTargetG->CreateSolidColorBrush(color, &pBrushG);
//        }
//    }
//    return hr;
//}

//void MainWindow::DiscardGraphicsResourcesG()
//{
//    SafeRelease(&pRenderTargetG);
//    SafeRelease(&pBrushG);
//
//}

//void MainWindow::OnPaintG()
//{
//    HRESULT hr = CreateGraphicsResourcesG();
//    if (SUCCEEDED(hr))
//    {
//        PAINTSTRUCT ps;
//        BeginPaint(gwnd, &ps);
//
//        pRenderTargetG->BeginDraw();
//
//        pRenderTargetG->Clear(D2D1::ColorF(D2D1::ColorF::DarkGoldenrod));
//
//        hr = pRenderTargetG->EndDraw();
//        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
//        {
//            DiscardGraphicsResourcesG();
//        }
//        EndPaint(gwnd, &ps);
//    }
//    return;
//}

//void MainWindow::OnPaintB() {
//    Button_SetText(hb1, L"Minkowski Difference");
//    Button_SetText(hb2, L"Minkowski Sum");
//    Button_SetText(hb3, L"Quickhull");
//    Button_SetText(hb4, L"Point Convex Hull");
//    Button_SetText(hb5, L"GJK");
//
//    return;
//}

//void MainWindow::ResizeG()
//{
//    if (pRenderTargetG != NULL)
//    {
//        RECT rc;
//        GetClientRect(m_hwnd, &rc);
//
//        SetWindowPos(gwnd, NULL, 0, 0, 350, rc.bottom, SWP_NOMOVE | SWP_NOZORDER);
//
//        InvalidateRect(gwnd, NULL, FALSE);
//    }
//}



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