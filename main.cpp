#include "covexhullwindow.h"

float DPIScale::scaleX = 1.0f;
float DPIScale::scaleY = 1.0f;
D2D1::ColorF::Enum colors[] = { D2D1::ColorF::Yellow, D2D1::ColorF::Salmon, D2D1::ColorF::LimeGreen };

void MainWindow::GetGraphInfo(float p[]/*, float offsetx, float scale*/) {
    RECT rc1;
    GetClientRect(m_hwnd, &rc1);

    p[0] = (BOUNDARY + ((float)(rc1.right - BOUNDARY) / 2)) + xoffset;     // Origin x
    p[1] = (float)(rc1.bottom / 2) + yoffset;                                // Origin y
    p[2] = (float)(rc1.right - BOUNDARY) / GridNum;            // x scale
    p[3] = (float) rc1.bottom / GridNum;                         // y scale

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

void MainWindow::ZoomIn() {
    if (GridNum > IN_LIMIT)
        GridNum -= 1;
    RefreshPoints();
    OnPaintM();
    return;
}

void MainWindow::ZoomOut() {
    if (GridNum < OUT_LIMIT)
        GridNum += 1;
    RefreshPoints();
    OnPaintM();
    return;
}

void MainWindow::OnPaintM()
{
    HRESULT hr = CreateGraphicsResourcesM();
    if (SUCCEEDED(hr))
    {
        PAINTSTRUCT ps;
        BeginPaint(m_hwnd, &ps);
        pRenderTargetM->BeginDraw();
        pRenderTargetM->Clear(D2D1::ColorF(D2D1::ColorF::Gold));

        switch (mode)
        {
        case InitialMode:
            break;
        case MinkowskiDifferenceMode:
            ShowMinkowskiDifference();
            break;
        case MinkowskiSumMode:
            ShowMinkowskiSum();
            break;
        case QuickhullMode:
            ShowQuickhull();
            break;
        case PointConvexHullMode:
            ShowPointConvexHull();
            break;
        case GJKMode:
            ShowGJK();
            break;
        }

        // Draw rectangle under Buttons
        RECT rc;
        GetClientRect(m_hwnd, &rc);
        D2D1_RECT_F rectangle = D2D1::RectF(0.0F,0.0F,BOUNDARY,rc.bottom);
        pBrushM->SetColor(D2D1::ColorF(D2D1::ColorF::White));
        pRenderTargetM->DrawRectangle(&rectangle, pBrushM, 5.0F);
        pBrushM->SetColor(D2D1::ColorF(D2D1::ColorF::DarkGoldenrod));
        pRenderTargetM->FillRectangle(&rectangle, pBrushM);


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
    ClearSelection3();

    if (HitTest1(dipX, dipY))
    {
        SetCapture(m_hwnd);
        ptMouseM = Selection1()->ellipse.point;
        ptMouseM.x -= dipX;
        ptMouseM.y -= dipY;
    }

    else if (HitTest2(dipX, dipY))
    {
        SetCapture(m_hwnd);
        ptMouseM = Selection2()->ellipse.point;
        ptMouseM.x -= dipX;
        ptMouseM.y -= dipY;
    }

    //move polygon
    else {
        D2D1_POINT_2F pt = D2D1::Point2F(ConvertDipToX(dipX), ConvertDipToY(dipY));

        switch (mode)
        {
        case InitialMode:
            break;
        case MinkowskiDifferenceMode:
            if (Func::DoPointConvex(pt, &MDconvex1))
            {
                SetCapture(m_hwnd);
                SetSelectPoly(&MDraw1, pt);
            }
            else if (Func::DoPointConvex(pt, &MDconvex2))
            {
                SetCapture(m_hwnd);
                SetSelectPoly(&MDraw2, pt);
            }
            else if (dipX > DPIScale::PixelsToDipsX(BOUNDARY))
            {
                SetCapture(m_hwnd);
                ScreenMove = true;
                ptMouseM.x = ConvertDipToX(dipX);
                ptMouseM.y = ConvertDipToY(dipY);

            }
            break;
        case MinkowskiSumMode:
            if (Func::DoPointConvex(pt, &MSconvex1))
            {
                SetCapture(m_hwnd);
                SetSelectPoly(&MSraw1, pt);
            }
            else if (Func::DoPointConvex(pt, &MSconvex2))
            {
                SetCapture(m_hwnd);
                SetSelectPoly(&MSraw2, pt);
            }
            else if (dipX > DPIScale::PixelsToDipsX(BOUNDARY))
            {
                SetCapture(m_hwnd);
                ScreenMove = true;
                ptMouseM.x = ConvertDipToX(dipX);
                ptMouseM.y = ConvertDipToY(dipY);

            }
            break;
        case QuickhullMode:
            if (Func::DoPointConvex(pt, &Qresult))
            {
                SetCapture(m_hwnd);
                SetSelectPoly(&Qraw, pt);
            }
            else if (dipX > DPIScale::PixelsToDipsX(BOUNDARY))
            {
                SetCapture(m_hwnd);
                ScreenMove = true;
                ptMouseM.x = ConvertDipToX(dipX);
                ptMouseM.y = ConvertDipToY(dipY);

            }
            break;
        case PointConvexHullMode:
            if (Func::DoPointConvex(pt, &PCconvex))
            {
                SetCapture(m_hwnd);
                SetSelectPoly(&PCconvex, pt);
            }
            else if (dipX > DPIScale::PixelsToDipsX(BOUNDARY))
            {
                SetCapture(m_hwnd);
                ScreenMove = true;
                ptMouseM.x = ConvertDipToX(dipX);
                ptMouseM.y = ConvertDipToY(dipY);

            }
            break;
        case GJKMode:
            if (Func::DoPointConvex(pt, &GJKconvex1))
            {
                SetCapture(m_hwnd);
                SetSelectPoly(&GJKraw1, pt);
            }
            else if (Func::DoPointConvex(pt, &GJKconvex2))
            {
                SetCapture(m_hwnd);
                SetSelectPoly(&GJKraw2, pt);
            }
            else if (dipX > DPIScale::PixelsToDipsX(BOUNDARY))
            {
                SetCapture(m_hwnd);
                ScreenMove = true;
                ptMouseM.x = ConvertDipToX(dipX);
                ptMouseM.y = ConvertDipToY(dipY);

            }
            break;
        }
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
    if (SelectPoly != NULL) {
        ClearSelection3();
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
    if (ScreenMove) {
        ScreenMove = false;
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
    ReleaseCapture();
}


void MainWindow::OnMouseMove(int pixelX, int pixelY, DWORD flags)
{
    const float dipX = DPIScale::PixelsToDipsX(pixelX);
    const float dipY = DPIScale::PixelsToDipsY(pixelY);

    std::vector<D2D1_POINT_2F>* p1 = nullptr, * p2 = nullptr;
    switch (mode)
    {
    case InitialMode:
        break;
    case MinkowskiDifferenceMode:
        p1 = &MDraw1;
        p2 = &MDraw2;
        break;
    case MinkowskiSumMode:
        p1 = &MSraw1;
        p2 = &MSraw2;
        break;
    case QuickhullMode:
        p1 = &Qraw;
        break;
    case PointConvexHullMode:
        p1 = &PCtarget;
        p2 = &PCconvex;
        break;
    case GJKMode:
        p1 = &GJKraw1;
        p2 = &GJKraw2;
        break;
    }

    if ((flags & MK_LBUTTON) && (Selection1()))
    {
        UpdatePoint(p1, Selection1()->xpos, Selection1()->ypos, ConvertDipToX(Selection1()->ellipse.point.x = dipX + ptMouseM.x), ConvertDipToY(Selection1()->ellipse.point.y = dipY + ptMouseM.y));
        Selection1()->xpos = ConvertDipToX(Selection1()->ellipse.point.x = dipX + ptMouseM.x);
        Selection1()->ypos = ConvertDipToY(Selection1()->ellipse.point.y = dipY + ptMouseM.y);
        if (mode == PointConvexHullMode)
        {
            if (Func::DoPointConvex(PCtarget[0], &PCconvex))
                Selection1()->color = D2D1::ColorF(D2D1::ColorF::Green);
            else
                Selection1()->color = D2D1::ColorF(D2D1::ColorF::Red);
        }
        InvalidateRect(m_hwnd, NULL, FALSE);
    }

    else if ((flags & MK_LBUTTON) && (Selection2()))
    {
        UpdatePoint(p2, Selection2()->xpos, Selection2()->ypos, ConvertDipToX(Selection2()->ellipse.point.x = dipX + ptMouseM.x), ConvertDipToY(Selection2()->ellipse.point.y = dipY + ptMouseM.y));
        Selection2()->xpos = ConvertDipToX(Selection2()->ellipse.point.x = dipX + ptMouseM.x);
        Selection2()->ypos = ConvertDipToY(Selection2()->ellipse.point.y = dipY + ptMouseM.y);
        InvalidateRect(m_hwnd, NULL, FALSE);
    }

    else if ((flags & MK_LBUTTON) && (SelectPoly != NULL))
    {
        UpdatePoly(D2D1::Point2F(dipX, dipY));

        RefreshPoints();
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
    else if ((flags & MK_LBUTTON) && (ScreenMove))
    {
        if ( ((ConvertDipToX(dipX) - ptMouseM.x) > 0 && xoffset < 1000) ||
            ((ConvertDipToX(dipX) - ptMouseM.x) < 0 && xoffset > -1000) )
            xoffset += (ConvertDipToX(dipX) - ptMouseM.x);
        if (((ConvertDipToY(dipY) - ptMouseM.y) < 0 && yoffset < 1000) ||
            ((ConvertDipToY(dipY) - ptMouseM.y) > 0 && yoffset > -1000))
            yoffset -= (ConvertDipToY(dipY) - ptMouseM.y);
        RefreshPoints();
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
};


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

HRESULT MainWindow::InsertPoints(std::vector<D2D1_POINT_2F>* vec1, std::vector<D2D1_POINT_2F>* vec2)
{
    try
    {
        for (int i = 0; i < vec1->size(); i++) {
            selection1 = ellipses1.insert(ellipses1.end(), shared_ptr<MyEllipse>(new MyEllipse()));

            Selection1()->ellipse.point = D2D1::Point2F(
                ConvertXToDip(Selection1()->xpos = (*vec1)[i].x),
                ConvertYToDip(Selection1()->ypos = (*vec1)[i].y)
            );
            Selection1()->ellipse.radiusX = Selection1()->ellipse.radiusY = 5.0f;
            Selection1()->color = D2D1::ColorF(D2D1::ColorF::Blue);
            if (mode == PointConvexHullMode)
            {
                if (Func::DoPointConvex(PCtarget[0], &PCconvex))
                    Selection1()->color = D2D1::ColorF(D2D1::ColorF::Green);
                else
                    Selection1()->color = D2D1::ColorF(D2D1::ColorF::Red);
            }
            ClearSelection1();
        }

        if (vec2 != NULL) {
            for (int i = 0; i < vec2->size(); i++) {
                selection2 = ellipses2.insert(ellipses2.end(), shared_ptr<MyEllipse>(new MyEllipse()));

                Selection2()->ellipse.point = D2D1::Point2F(
                    ConvertXToDip(Selection2()->xpos = (*vec2)[i].x),
                    ConvertYToDip(Selection2()->ypos = (*vec2)[i].y)
                );
                Selection2()->ellipse.radiusX = Selection2()->ellipse.radiusY = 5.0f;
                Selection2()->color = D2D1::ColorF(D2D1::ColorF::Green);
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
    ClearSelection3();
    ScreenMove = false;
    xoffset = 0;
    yoffset = 0;

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
        MDraw1.clear();
        MDraw2.clear();
        MDconvex1.clear();
        MDconvex2.clear();
        MDresult.clear();
        GenerateInitialPoints(&MDraw1, MDSIZE, 0, 5);
        GenerateInitialPoints(&MDraw2, MDSIZE, 5, 10);
        SortPoints(&MDraw1);
        SortPoints(&MDraw2);
        InsertPoints(&MDraw1, &MDraw2);
        break;

    case MinkowskiSumMode:
        ellipses1.clear();
        ellipses2.clear();
        MSraw1.clear();
        MSraw2.clear();
        MSconvex1.clear();
        MSconvex2.clear();
        MSresult.clear();
        GenerateInitialPoints(&MSraw1, MSSIZE, 0, 5);
        GenerateInitialPoints(&MSraw2, MSSIZE, 5, 10);
        SortPoints(&MSraw1);
        SortPoints(&MSraw2);
        InsertPoints(&MSraw1, &MSraw2);
        break;

    case QuickhullMode:
        ellipses1.clear();
        ellipses2.clear();
        Qraw.clear();
        Qresult.clear();
        GenerateInitialPoints(&Qraw, QSIZE, -8, 8);
        SortPoints(&Qraw);
        InsertPoints(&Qraw, NULL);
        break;

    case PointConvexHullMode:
        ellipses1.clear();
        ellipses2.clear();
        PCtarget.clear();
        PCraw.clear();
        PCconvex.clear();
        GenerateInitialPoints(&PCtarget, 1, -10, 10);
        GenerateInitialPoints(&PCraw, PCSIZE, -10, 10);
        SortPoints(&PCraw);
        Func::DoQuickhull(&PCraw, &PCconvex);
        //SortPoints(&PCconvex);
        InsertPoints(&PCtarget, NULL); /////
        break;

    case GJKMode:
        ellipses1.clear();
        ellipses2.clear();
        GJKraw1.clear();
        GJKraw2.clear();
        GJKconvex1.clear();
        GJKconvex2.clear();
        GJKresult.clear();
        GenerateInitialPoints(&GJKraw1, GJKSIZE, 0, 5);
        GenerateInitialPoints(&GJKraw2, GJKSIZE, 5, 10);
        SortPoints(&GJKraw1);
        SortPoints(&GJKraw2);
        InsertPoints(&GJKraw1, &GJKraw2);
        break;
    }

    hCursor = LoadCursor(NULL, cursor);
    SetCursor(hCursor);
    OnPaintM();
    //OnPaintG();
}


// Create the main window
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
    srand(time(nullptr));
    MainWindow win;

    if (!win.Create(L"Convex Hull Algorithms", WS_OVERLAPPEDWINDOW| WS_CLIPCHILDREN))
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
        //DiscardGraphicsResourcesG();
        SafeRelease(&pFactory);
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        OnPaintM();
        //OnPaintG();
        return 0;

    case WM_SIZE:
        ResizeM();
        //ResizeG();
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

    case WM_MOUSEWHEEL:
        if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
            ZoomIn();
        }
        else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) {
            ZoomOut();
        }
        break;
    //case WM_CTLCOLORBTN:
    //    OnPaintB();
    //    break;

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
            SetMode(MinkowskiSumMode);
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
