#include "Draw.h"

void Draw::Reset()
{
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &(Draw::pFactory));
    HRESULT hr = S_OK;
    RECT rc;
    GetClientRect(hwnd, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

    hr = Draw::pFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(Draw::hwnd, size),
        &(Draw::pRenderTarget));

    D2D1_COLOR_F c = D2D1::ColorF(D2D1::ColorF::Green);

    Draw::pRenderTarget->CreateSolidColorBrush(c, &(Draw::pBrush));
}

D2D1_COLOR_F Draw::ConvertString(std::string colorStr)
{
    std::string black = "black";
    std::string blue = "blue";
    std::string green = "green";
    std::string white = "white";
    std::string red = "red";

    if (colorStr.compare(black) == 0) return D2D1::ColorF(D2D1::ColorF::Black);
    if (colorStr.compare(blue) == 0 ) return D2D1::ColorF(D2D1::ColorF::Blue);
    if (colorStr.compare(green) == 0) return D2D1::ColorF(D2D1::ColorF::Green);
    if (colorStr.compare(white) == 0) return D2D1::ColorF(D2D1::ColorF::White);
    if (colorStr.compare(red) == 0) return D2D1::ColorF(D2D1::ColorF::Red);
    return D2D1::ColorF(D2D1::ColorF::White);
}

void Draw::DrawPoint(point p, std::string color)
{
    D2D1_COLOR_F c = Draw::ConvertString(color);
    Draw::pRenderTarget->CreateSolidColorBrush(c, &(Draw::pBrush));
    D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(p.x, p.y), 4, 4);
    Draw::pRenderTarget->FillEllipse(ellipse, pBrush);
}

void Draw::DrawLine(point p1, point p2, std::string color)
{
    D2D1_COLOR_F c = Draw::ConvertString(color);
    Draw::pRenderTarget->CreateSolidColorBrush(c, &(Draw::pBrush));
    Draw::pRenderTarget->DrawLine(
        D2D1::Point2F(p1.x, p1.y),
        D2D1::Point2F(p2.x, p2.y),
        Draw::pBrush,
        3.0f
    );
}

void Draw::BeginDraw()
{
    Draw::Reset();
    Draw::pRenderTarget->BeginDraw();
    Draw::pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
}

void Draw::EndDraw()
{
    Draw::pRenderTarget->EndDraw();
}

pixel Draw::PointToPixel(point pt)
{
    pixel px = { pt.x * Draw::magnify + Draw::xff, pt.y * Draw::magnify + Draw::yff };
    return px;
}

point Draw::PixelToPoint(pixel px)
{
    point pt = { (px.x - Draw::xff) / Draw::magnify, (px.y - Draw::yff) / Draw::magnify };
    return pt;
}