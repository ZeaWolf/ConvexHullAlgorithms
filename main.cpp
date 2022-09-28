#include "covexhullwindow.h"

float DPIScale::scaleX = 1.0f;
float DPIScale::scaleY = 1.0f;
D2D1::ColorF::Enum colors[] = { D2D1::ColorF::Yellow, D2D1::ColorF::Salmon, D2D1::ColorF::LimeGreen };

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
        BeginPaint(m_hwnd, &ps);

        pRenderTargetM->BeginDraw();

        pRenderTargetM->Clear(D2D1::ColorF(D2D1::ColorF::DarkGoldenrod));

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

        pRenderTargetM->Resize(size);

        InvalidateRect(m_hwnd, NULL, FALSE);
    }
}

void MainWindow::OnLButtonDown(int pixelX, int pixelY, DWORD flags)
{
    const float dipX = DPIScale::PixelsToDipsX(pixelX);
    const float dipY = DPIScale::PixelsToDipsY(pixelY);

    if (mode == DrawMode)
    {
        POINT pt = { pixelX, pixelY };

        if (DragDetect(m_hwnd, pt))
        {
            SetCapture(m_hwnd);

            // Start a new ellipse.
            InsertEllipse(dipX, dipY);
        }
    }
    else
    {
        ClearSelection();

        if (HitTest(dipX, dipY))
        {
            SetCapture(m_hwnd);

            ptMouseM = Selection()->ellipse.point;
            ptMouseM.x -= dipX;
            ptMouseM.y -= dipY;

            SetMode(DragMode);
        }
    }
    InvalidateRect(m_hwnd, NULL, FALSE);
}

void MainWindow::OnLButtonUp()
{
    if ((mode == DrawMode) && Selection())
    {
        ClearSelection();
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
    else if (mode == DragMode)
    {
        SetMode(SelectMode);
    }
    ReleaseCapture();
}


void MainWindow::OnMouseMove(int pixelX, int pixelY, DWORD flags)
{
    const float dipX = DPIScale::PixelsToDipsX(pixelX);
    const float dipY = DPIScale::PixelsToDipsY(pixelY);

    if ((flags & MK_LBUTTON) && Selection())
    {
        if (mode == DrawMode)
        {
            // Resize the ellipse.
            const float width = (dipX - ptMouseM.x) / 2;
            const float height = (dipY - ptMouseM.y) / 2;
            const float x1 = ptMouseM.x + width;
            const float y1 = ptMouseM.y + height;

            Selection()->ellipse = D2D1::Ellipse(D2D1::Point2F(x1, y1), width, height);
        }
        else if (mode == DragMode)
        {
            // Move the ellipse.
            Selection()->ellipse.point.x = dipX + ptMouseM.x;
            Selection()->ellipse.point.y = dipY + ptMouseM.y;
        }
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
}


void MainWindow::OnKeyDown(UINT vkey)
{
    switch (vkey)
    {
    case VK_BACK:
    case VK_DELETE:
        if ((mode == SelectMode) && Selection())
        {
            ellipses.erase(selection);
            ClearSelection();
            SetMode(SelectMode);
            InvalidateRect(m_hwnd, NULL, FALSE);
        };
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
}

HRESULT MainWindow::InsertEllipse(float x, float y)
{
    try
    {
        selection = ellipses.insert(
            ellipses.end(),
            shared_ptr<MyEllipse>(new MyEllipse()));

        Selection()->ellipse.point = ptMouseM = D2D1::Point2F(x, y);
        Selection()->ellipse.radiusX = Selection()->ellipse.radiusY = 2.0f;
        Selection()->color = D2D1::ColorF(colors[nextColor]);

        nextColor = (nextColor + 1) % ARRAYSIZE(colors);
    }
    catch (std::bad_alloc)
    {
        return E_OUTOFMEMORY;
    }
    return S_OK;
}


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
    if ((mode == SelectMode) && Selection())
    {
        Selection()->ellipse.point.x += x;
        Selection()->ellipse.point.y += y;
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
}

void MainWindow::SetMode(Mode m)
{
    mode = m;

    LPWSTR cursor = NULL;
    switch (mode)
    {
    case DrawMode:
        cursor = IDC_CROSS;
        break;

    case SelectMode:
        cursor = IDC_HAND;
        break;

    case DragMode:
        cursor = IDC_SIZEALL;
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

        SetMode(DrawMode);
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
        case ID_DRAW_MODE:
            SetMode(DrawMode);
            break;

        case ID_SELECT_MODE:
            SetMode(SelectMode);
            break;

        case ID_TOGGLE_MODE:
            if (mode == DrawMode)
            {
                SetMode(SelectMode);
            }
            else
            {
                SetMode(DrawMode);
            }
            break;

        case ID_MD_BUTTON:
            MessageBox(m_hwnd, L"Click MD", L"Title", MB_ICONINFORMATION);
            break;

        case ID_MS_BUTTON:
            MessageBox(m_hwnd, L"Click MS", L"Title", MB_ICONINFORMATION);
            break;

        case ID_Q_BUTTON:
            MessageBox(m_hwnd, L"Click Q", L"Title", MB_ICONINFORMATION);
            break;

        case ID_PCH_BUTTON:
            MessageBox(m_hwnd, L"Click PCH", L"Title", MB_ICONINFORMATION);
            break;

        case ID_GJK_BUTTON:
            MessageBox(m_hwnd, L"Click GJK", L"Title", MB_ICONINFORMATION);
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