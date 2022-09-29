#include "covexhullwindow.h"

float DPIScale::scaleX = 1.0f;
float DPIScale::scaleY = 1.0f;
D2D1::ColorF::Enum colors[] = { D2D1::ColorF::Yellow, D2D1::ColorF::Salmon, D2D1::ColorF::LimeGreen };

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
    return DPIScale::PixelsToDipsY( o[1] - (y * o[3]));
};

float MainWindow::ConvertDipToX(float x) {
    float o[4];
    GetGraphInfo(o);
    return (DPIScale::DipsToPixelsX(x) - o[0]) / o[2];
};

float MainWindow::ConvertDipToY(float y) {
    float o[4];
    GetGraphInfo(o);
    return (o[1] - DPIScale::DipsToPixelsX(y)) / o[3];
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

        if (mode != InitialMode)
        {

            DrawGraph(pRenderTargetM, pBrushM);

            // cirle
            for (auto i = ellipses1.begin(); i != ellipses1.end(); ++i)
                (*i)->Draw(pRenderTargetM, pBrushM);

            for (auto j = ellipses2.begin(); j != ellipses2.end(); ++j)
                (*j)->Draw(pRenderTargetM, pBrushM);

            if (Selection1())
            {
                pBrushM->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
                pRenderTargetM->DrawEllipse(Selection1()->ellipse, pBrushM, 2.0f);
                pBrushM->SetColor(D2D1::ColorF(D2D1::ColorF::Green));
                pRenderTargetM->FillEllipse(Selection1()->ellipse, pBrushM);
            }

            if (Selection2())
            {
                pBrushM->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
                pRenderTargetM->DrawEllipse(Selection2()->ellipse, pBrushM, 2.0f);
                pBrushM->SetColor(D2D1::ColorF(D2D1::ColorF::Green));
                pRenderTargetM->FillEllipse(Selection2()->ellipse, pBrushM);
            }
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

        for (auto i = ellipses1.begin(); i != ellipses1.end(); ++i)
        {
            (*i)->ellipse.point.x = ConvertXToDip((*i)->xpos);
            (*i)->ellipse.point.y = ConvertYToDip((*i)->ypos);
        }

        for (auto i = ellipses2.begin(); i != ellipses2.end(); ++i)
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

    ClearSelection1();
    ClearSelection2();

    if (HitTest1(dipX, dipY))
    {
        SetCapture(m_hwnd);
        ptMouseM = Selection1()->ellipse.point;
        ptMouseM.x -= dipX;
        ptMouseM.y -= dipY;
    }

    if (HitTest2(dipX, dipY))
    {
        SetCapture(m_hwnd);
        ptMouseM = Selection2()->ellipse.point;
        ptMouseM.x -= dipX;
        ptMouseM.y -= dipY;
    }
    InvalidateRect(m_hwnd, NULL, FALSE);
}

void MainWindow::OnLButtonUp()
{
    if (Selection1())
    {
        ClearSelection1();
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
    if (Selection2())
    {
        ClearSelection2();
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
    ReleaseCapture();
}


void MainWindow::OnMouseMove(int pixelX, int pixelY, DWORD flags)
{
    const float dipX = DPIScale::PixelsToDipsX(pixelX);
    const float dipY = DPIScale::PixelsToDipsY(pixelY);

    if ((flags & MK_LBUTTON) && (Selection1()))
    {
        Selection1()->xpos = ConvertDipToX(Selection1()->ellipse.point.x = dipX + ptMouseM.x);
        Selection1()->ypos = ConvertDipToY(Selection1()->ellipse.point.y = dipY + ptMouseM.y);
        InvalidateRect(m_hwnd, NULL, FALSE);
    }

    if ((flags & MK_LBUTTON) && (Selection2()))
    {
        Selection2()->xpos = ConvertDipToX(Selection2()->ellipse.point.x = dipX + ptMouseM.x);
        Selection2()->ypos = ConvertDipToY(Selection2()->ellipse.point.y = dipY + ptMouseM.y);
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
    }
    return;
};

HRESULT MainWindow::InsertPoints(D2D1_POINT_2F arr1[], D2D1_POINT_2F arr2[], int size1, int size2)
{
    try
    {
        
        for (int i = 0; i < size1; i++) {
            selection1 = ellipses1.insert(ellipses1.end(), shared_ptr<MyEllipse>(new MyEllipse()));

            Selection1()->ellipse.point = D2D1::Point2F(
                ConvertXToDip(Selection1()->xpos = arr1[i].x),
                ConvertYToDip(Selection1()->ypos = arr1[i].y)
            );
            Selection1()->ellipse.radiusX = Selection1()->ellipse.radiusY = 5.0f;
            Selection1()->color = D2D1::ColorF(D2D1::ColorF::Blue);
            ClearSelection1();
        }

        if (arr2 != NULL) {
            for (int i = 0; i < size2; i++) {
                selection2 = ellipses2.insert(ellipses2.end(), shared_ptr<MyEllipse>(new MyEllipse()));

                Selection2()->ellipse.point = D2D1::Point2F(
                    ConvertXToDip(Selection2()->xpos = arr2[i].x),
                    ConvertYToDip(Selection2()->ypos = arr2[i].y)
                );
                Selection2()->ellipse.radiusX = Selection2()->ellipse.radiusY = 5.0f;
                Selection2()->color = D2D1::ColorF(D2D1::ColorF::Blue);
                ClearSelection2();
            }
        }

        //nextColor = (nextColor + 1) % ARRAYSIZE(colors);
    }
    catch (std::bad_alloc)
    {
        return E_OUTOFMEMORY;
    }
    return S_OK;
};


BOOL MainWindow::HitTest1(float x, float y)
{
    for (auto i = ellipses1.rbegin(); i != ellipses1.rend(); ++i)
    {
        if ((*i)->HitTest(x, y))
        {
            selection1 = (++i).base();
            return TRUE;
        }
    }
    return FALSE;
}

BOOL MainWindow::HitTest2(float x, float y)
{
    for (auto i = ellipses2.rbegin(); i != ellipses2.rend(); ++i)
    {
        if ((*i)->HitTest(x, y))
        {
            selection2 = (++i).base();
            return TRUE;
        }
    }
    return FALSE;
}


void MainWindow::SetMode(Mode m)
{
    mode = m;
    //https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadcursora
    LPWSTR cursor = IDC_ARROW; //IDC_CROSS, IDC_HAND, IDC_SIZEALL, IDC_HAND, 
    switch (mode)
    {
    case InitialMode:
        break;

    case MinkowskiDifferenceMode:
        ellipses1.clear();
        ellipses2.clear();
        GenerateInitialPoints(MD1, MDSIZE);
        GenerateInitialPoints(MD2, MDSIZE);
        InsertPoints(MD1, MD2, MDSIZE, MDSIZE);
        break;

    case MinkowskiSumSelectMode:
        ellipses1.clear();
        ellipses2.clear();
        GenerateInitialPoints(MS1, MSSIZE);
        GenerateInitialPoints(MS2, MSSIZE);
        InsertPoints(MS1, MS2, MSSIZE, MSSIZE);
        break;

    case QuickhullMode:
        ellipses1.clear();
        ellipses2.clear();
        GenerateInitialPoints(Q, QSIZE);
        InsertPoints(Q, NULL, QSIZE, 0);
        break;

    case PointConvexHullMode:
        ellipses1.clear();
        ellipses2.clear();
        GenerateInitialPoints(PC1, 1);
        GenerateInitialPoints(PC2, PCSIZE);
        InsertPoints(PC1, PC2, 1, PCSIZE);
        break;

    case GJKMode:
        ellipses1.clear();
        ellipses2.clear();
        GenerateInitialPoints(GJK1, GJKSIZE);
        GenerateInitialPoints(GJK2, GJKSIZE);
        InsertPoints(GJK1, GJK2, GJKSIZE, GJKSIZE);
        break;
    }

    hCursor = LoadCursor(NULL, cursor);
    SetCursor(hCursor);
    OnPaintM();
    OnPaintG();
}


// Create the main window
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
    srand(time(nullptr));
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

        CreateLayout();
        SetMode(InitialMode);
        return 0;

    case WM_DESTROY:
        DiscardGraphicsResourcesM();
        DiscardGraphicsResourcesG();
        SafeRelease(&pFactory);
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
