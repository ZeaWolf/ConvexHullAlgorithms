#include <windows.h>
#include <d2d1.h>
#include "Preset.h"
#pragma comment(lib, "d2d1")

class DrawHelper
{
public:
    HWND hwnd;
    ID2D1Factory* pFactory;
    ID2D1HwndRenderTarget* pRenderTarget;
    ID2D1SolidColorBrush* pBrush;

    DrawHelper(HWND) : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL)
    {
        DrawHelper::hwnd = h;
        DrawHelper::pFactory = NULL;
        DrawHelper::pRenderTarget = NULL;
        DrawHelper::pBrush = NULL;
        D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
        HRESULT hr = S_OK;
        RECT rc;
        GetClientRect(hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        hr = pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hwnd, size),
            &pRenderTarget);
    }

    void ShowPoint(point p, D2D1_COLOR_F color);

    void Draw(ID2D1HwndRenderTarget* pRenderTarget, ID2D1SolidColorBrush* pBrush, point p);
}
