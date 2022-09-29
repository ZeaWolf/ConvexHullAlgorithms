#include "covexhullwindow.h"

float DPIScale::scaleX = 1.0f;
float DPIScale::scaleY = 1.0f;
D2D1::ColorF::Enum colors[] = { D2D1::ColorF::Yellow, D2D1::ColorF::Salmon, D2D1::ColorF::LimeGreen };

float randx[] = { 1, 2, 0, 5, 6, -5 };
float randy[] = { 2, 5, 6, -2, 0, -5};

void MainWindow::GetGraphInfo(float p[]) {
    RECT rc1, rc2;
    GetClientRect(m_hwnd, &rc1);
    GetClientRect(gwnd, &rc2);

    p[0] = rc2.right + ((float)(rc1.right - rc2.right) / 2);  // Origin x
    p[1] = (float)rc1.bottom / 2;                                // Origin y
    p[2] = (float)(rc1.right - rc2.right) / GRID_NUM;         // x scale
    p[3] = (float) rc1.bottom / GRID_NUM;                         // y scale

    return;
};

float MainWindow::ConvertXToDip(float x) {
    float o[4];
    GetGraphInfo(o);
    return DPIScale::PixelsToDipsX((x * o[2]) + o[0]);
};

float MainWindow::ConvertYToDip(float y) {
    float o[4];
    GetGraphInfo(o);
    return DPIScale::PixelsToDipsY((y * o[3]) + o[1]);
};

float MainWindow::ConvertDipToX(float x) {
    float o[4];
    GetGraphInfo(o);
    return (DPIScale::DipsToPixelsX(x) - o[0]) / o[2];
};

float MainWindow::ConvertDipToY(float y) {
    float o[4];
    GetGraphInfo(o);
    return (DPIScale::DipsToPixelsX(y) - o[1]) / o[3];
};

HRESULT MainWindow::CreateGraphicsResourcesM()
{
    HRESULT hr = S_OK;
    if (pRenderTargetM == NULL)
    {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        hr = pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hwnd, size),
            &pRenderTargetM);

        if (SUCCEEDED(hr))
        {
            const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
            hr = pRenderTargetM->CreateSolidColorBrush(color, &pBrushM);
        }
    }
    return hr;
}

void MainWindow::DiscardGraphicsResourcesM()
{
    SafeRelease(&pRenderTargetM);
    SafeRelease(&pBrushM);

}

void MainWindow::OnPaintM()
{
    HRESULT hr = CreateGraphicsResourcesM();
    if (SUCCEEDED(hr))
    {
        PAINTSTRUCT ps;

        /*UpdateWindow(hb1);
        UpdateWindow(hb2);
        UpdateWindow(hb3);
        UpdateWindow(hb4);
        UpdateWindow(hb5);*/

        BeginPaint(m_hwnd, &ps);

        pRenderTargetM->BeginDraw();

        pRenderTargetM->Clear(D2D1::ColorF(D2D1::ColorF::Gold));

        DrawGraph(pRenderTargetM, pBrushM);
        
        // cirle
        for (auto i = ellipses.begin(); i != ellipses.end(); ++i)
        {
            (*i)->Draw(pRenderTargetM, pBrushM);
        }

        if (Selection())
        {
            pBrushM->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
            pRenderTargetM->DrawEllipse(Selection()->ellipse, pBrushM, 2.0f);
        }


        hr = pRenderTargetM->EndDraw();
        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
        {
            DiscardGraphicsResourcesM();
        }
        EndPaint(m_hwnd, &ps);
    }

    return;
}

void MainWindow::ResizeM()
{
    if (pRenderTargetM != NULL)
    {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        for (auto i = ellipses.begin(); i != ellipses.end(); ++i)
        {
            (*i)->ellipse.point.x = ConvertXToDip((*i)->xpos);
            (*i)->ellipse.point.y = ConvertYToDip((*i)->ypos);
        }

        pRenderTargetM->Resize(size);

        InvalidateRect(m_hwnd, NULL, FALSE);
    }
}



void MainWindow::OnLButtonDown(int pixelX, int pixelY, DWORD flags)
{
    const float dipX = DPIScale::PixelsToDipsX(pixelX);
    const float dipY = DPIScale::PixelsToDipsY(pixelY);

    //if (mode == DrawMode)
    //{
    //    POINT pt = { pixelX, pixelY };

    //    if (DragDetect(m_hwnd, pt))
    //    {
    //        SetCapture(m_hwnd);

    //        // Start a new ellipse.
    //        InsertEllipse(dipX, dipY);
    //    }
    //}
    //else
    //{
    //    ClearSelection();

    //    if (HitTest(dipX, dipY))
    //    {
    //        SetCapture(m_hwnd);

    //        ptMouseM = Selection()->ellipse.point;
    //        ptMouseM.x -= dipX;
    //        ptMouseM.y -= dipY;

    //        SetMode(DragMode);
    //    }
    //}

    ClearSelection();

    if (HitTest(dipX, dipY))
    {
        SetCapture(m_hwnd);
        ptMouseM = Selection()->ellipse.point;
        ptMouseM.x -= dipX;
        ptMouseM.y -= dipY;
    }
    InvalidateRect(m_hwnd, NULL, FALSE);
}

void MainWindow::OnLButtonUp()
{
    if (Selection())
    {
        ClearSelection();
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
    ReleaseCapture();
}


void MainWindow::OnMouseMove(int pixelX, int pixelY, DWORD flags)
{
    const float dipX = DPIScale::PixelsToDipsX(pixelX);
    const float dipY = DPIScale::PixelsToDipsY(pixelY);

    if ((flags & MK_LBUTTON) && Selection())
    {
        //if (mode == DrawMode)
        //{
        //    // Resize the ellipse.
        //    const float width = (dipX - ptMouseM.x) / 2;
        //    const float height = (dipY - ptMouseM.y) / 2;
        //    const float x1 = ptMouseM.x + width;
        //    const float y1 = ptMouseM.y + height;

        //    Selection()->ellipse = D2D1::Ellipse(D2D1::Point2F(x1, y1), width, height);
        //}
        //else if (mode == DragMode)
        //{
        //    // Move the ellipse.
        //    Selection()->ellipse.point.x = dipX + ptMouseM.x;
        //    Selection()->ellipse.point.y = dipY + ptMouseM.y;
        //}
        //Selection()->ellipse.point.x = dipX + ptMouseM.x;
        //Selection()->ellipse.point.y = dipY + ptMouseM.y;

        Selection()->xpos = ConvertDipToX(Selection()->ellipse.point.x = dipX + ptMouseM.x);
        Selection()->ypos = ConvertDipToY(Selection()->ellipse.point.y = dipY + ptMouseM.y);

        InvalidateRect(m_hwnd, NULL, FALSE);
    }
}


void MainWindow::OnKeyDown(UINT vkey)
{
    switch (vkey)
    {

    case VK_ESCAPE:
        SetMode(InitialMode);
        break;

    case VK_BACK:
    case VK_DELETE:
        ellipses.erase(selection);
        ClearSelection();
        InvalidateRect(m_hwnd, NULL, FALSE);
        break;

    case VK_LEFT:
        MoveSelection(-1, 0);
        break;

    case VK_RIGHT:
        MoveSelection(1, 0);
        break;

    case VK_UP:
        MoveSelection(0, -1);
        break;

    case VK_DOWN:
        MoveSelection(0, 1);
        break;
    }
};

HRESULT MainWindow::InsertPoints(float x[], float y[], int size)
{
    try
    {
        for (int i = 0; i < size; i++) {
            selection = ellipses.insert(
                ellipses.end(),
                shared_ptr<MyEllipse>(new MyEllipse()));

            //Selection()->ellipse.point = ptMouseM = D2D1::Point2F(x, y);
            //Selection()->xpos = x[i];
            //Selection()->ypos = y[i];
            Selection()->ellipse.point = D2D1::Point2F(
                ConvertXToDip(Selection()->xpos = x[i]), 
                ConvertYToDip(Selection()->ypos = y[i])
            );
            Selection()->ellipse.radiusX = Selection()->ellipse.radiusY = 5.0f;
            Selection()->color = D2D1::ColorF(D2D1::ColorF::Blue);
        }

        //nextColor = (nextColor + 1) % ARRAYSIZE(colors);
    }
    catch (std::bad_alloc)
    {
        return E_OUTOFMEMORY;
    }
    return S_OK;
};


BOOL MainWindow::HitTest(float x, float y)
{
    for (auto i = ellipses.rbegin(); i != ellipses.rend(); ++i)
    {
        if ((*i)->HitTest(x, y))
        {
            selection = (++i).base();
            return TRUE;
        }
    }
    return FALSE;
}

void MainWindow::MoveSelection(float x, float y)
{
    if (Selection())
    {
        Selection()->ellipse.point.x += x;
        Selection()->ellipse.point.y += y;
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
}

void MainWindow::SetMode(Mode m)
{
    mode = m;
    //https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadcursora
    LPWSTR cursor = NULL;
    switch (mode)
    {
    case InitialMode:
        cursor = IDC_ARROW;
        break;

    case MinkowskiDifferenceMode:
        cursor = IDC_CROSS;
        break;

    case MinkowskiSumSelectMode:
        cursor = IDC_HAND;
        break;

    case QuickhullMode:
        cursor = IDC_SIZEALL;
        break;

    case PointConvexHullMode:
        cursor = IDC_HAND;
        break;

    case GJKMode:
        cursor = IDC_HAND;
        break;
    }

    hCursor = LoadCursor(NULL, cursor);
    SetCursor(hCursor);
}


// Create the main window
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
    MainWindow win;

    if (!win.Create(L"Convex Hull Algorithms", WS_OVERLAPPEDWINDOW))
    {
        return 0;
    }

    HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCEL1));

    ShowWindow(win.Window(), nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(win.Window(), hAccel, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return 0;
};


// Handle Message Function
LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    
    switch (uMsg)
    {
    case WM_CREATE:
        if (FAILED(D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
        {
            return -1;  // Fail CreateWindowEx.
        }
        DPIScale::Initialize(pFactory);

        //if (FAILED(D2D1CreateFactory(
        //    D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactoryG)))
        //{
        //    return -1;  // Fail CreateWindowEx.
        //}
        //DPIScale::Initialize(pFactoryG);
        CreateLayout();
        SetMode(InitialMode);

        InsertPoints(randx, randy, 5);
        return 0;

    case WM_DESTROY:
        DiscardGraphicsResourcesM();
        DiscardGraphicsResourcesG();
        SafeRelease(&pFactory);
        //SafeRelease(&pFactoryG);
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        OnPaintM();
        OnPaintG();
        return 0;

    case WM_SIZE:
        ResizeM();
        ResizeG();
        return 0;

    case WM_LBUTTONDOWN:
        OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
        return 0;

    case WM_LBUTTONUP:
        OnLButtonUp();
        return 0;

    case WM_MOUSEMOVE:
        OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
        return 0;

    case WM_SETCURSOR:
        if (LOWORD(lParam) == HTCLIENT)
        {
            SetCursor(hCursor);
            return TRUE;
        }
        break;

    case WM_KEYDOWN:
        OnKeyDown((UINT)wParam);
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {

        case ID_MD_BUTTON:
            SetMode(MinkowskiDifferenceMode);
            break;

        case ID_MS_BUTTON:
            SetMode(MinkowskiSumSelectMode);
            break;

        case ID_Q_BUTTON:
            SetMode(QuickhullMode);
            break;

        case ID_PCH_BUTTON:
            SetMode(PointConvexHullMode);
            break;

        case ID_GJK_BUTTON:
            SetMode(GJKMode);
            break;
        }

        return 0;
    }
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
};


////
//LRESULT CALLBACK GraphWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//    switch (uMsg)
//    {
//    case WM_CREATE:
//        return 0;
//    case WM_CLOSE:
//    {
//        if (MessageBox(hwnd, L"Really quit?", L"My application", MB_OKCANCEL) == IDOK)
//        {
//            DestroyWindow(hwnd);
//        }
//        return 0;
//    }
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        return 0;
//
//    case WM_PAINT:
//        PAINTSTRUCT ps;
//        BeginPaint(hwnd, &ps);
//        HDC hdc = BeginPaint(hwnd, &ps);
//        // All painting occurs here, between BeginPaint and EndPaint.
//        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW +2));
//        EndPaint(hwnd, &ps);
//        return 0;
//    };
//    return 0;
//};