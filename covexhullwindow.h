#define ID_MD_BUTTON		380
#define ID_MS_BUTTON		381
#define ID_Q_BUTTON			382
#define ID_PCH_BUTTON		383
#define ID_GJK_BUTTON		384
#define ID_GRAPH			385

#define IN_LIMIT            1
#define OUT_LIMIT           100
//#define GRID_NUM            60
#define LOWRAND             -8
#define UPPRAND             8

#define MDSIZE              6
#define MSSIZE              6
#define QSIZE               15
#define PCSIZE              15
#define GJKSIZE             6

#define BOUNDARY            275.0F

#include <windows.h>
#include <Windowsx.h>
#include <d2d1.h>

#include <list>
#include <memory>
using namespace std;

#pragma comment(lib, "d2d1")

#include "basewin.h"
#include "resource.h"

//random number generator
#include<cstdlib>
#include<ctime>
#include<vector>
#include <math.h>
#include "func.h"


template <class T> void SafeRelease(T** ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

class DPIScale
{
    static float scaleX;
    static float scaleY;

public:
    static void Initialize(ID2D1Factory* pFactory)
    {
        FLOAT dpiX, dpiY;
        #pragma warning(suppress : 4996)
        pFactory->GetDesktopDpi(&dpiX, &dpiY);
        scaleX = dpiX / 96.0f;
        scaleY = dpiY / 96.0f;
    }

    template <typename T>
    static float PixelsToDipsX(T x)
    {
        return static_cast<float>(x) / scaleX;
    }

    template <typename T>
    static float PixelsToDipsY(T y)
    {
        return static_cast<float>(y) / scaleY;
    }

    template <typename T>
    static float DipsToPixelsX(T x)
    {
        return static_cast<float>(x) * scaleX;
    }

    template <typename T>
    static float DipsToPixelsY(T y)
    {
        return static_cast<float>(y) * scaleY;
    }
};

//float DPIScale::scaleX = 1.0f;
//float DPIScale::scaleY = 1.0f;

struct MyEllipse
{
    D2D1_ELLIPSE    ellipse;
    D2D1_COLOR_F    color;
    float           xpos;
    float           ypos;

    void Draw(ID2D1RenderTarget* pRT, ID2D1SolidColorBrush* pBrush)
    {
        pBrush->SetColor(color);
        pRT->FillEllipse(ellipse, pBrush);
        pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
        pRT->DrawEllipse(ellipse, pBrush, 1.0f);
    }

    BOOL HitTest(float x, float y)
    {
        const float a = ellipse.radiusX;
        const float b = ellipse.radiusY;
        const float x1 = x - ellipse.point.x;
        const float y1 = y - ellipse.point.y;
        const float d = ((x1 * x1) / (a * a)) + ((y1 * y1) / (b * b));
        return d <= 1.0f;
    }
};


//D2D1::ColorF::Enum colors[] = { D2D1::ColorF::Yellow, D2D1::ColorF::Salmon, D2D1::ColorF::LimeGreen };



class MainWindow : public BaseWindow<MainWindow>
{

    enum Mode
    {
        InitialMode,
        MinkowskiDifferenceMode,
        MinkowskiSumMode,
        QuickhullMode,
        PointConvexHullMode,
        GJKMode
    };

    HCURSOR                 hCursor;

    ID2D1Factory*           pFactory;
    ID2D1HwndRenderTarget*  pRenderTargetM;
    ID2D1SolidColorBrush*   pBrushM;
    D2D1_POINT_2F           ptMouseM;

    ID2D1HwndRenderTarget*  pRenderTargetG;
    ID2D1SolidColorBrush*   pBrushG;
    D2D1_POINT_2F           ptMouseG;

    Mode                    mode;

    int                     GridNum;

    bool                    ScreenMove;

    float                    xoffset;
    float                    yoffset;

    //HWND                    gwnd = NULL;
    HWND                    hb1 = NULL;
    HWND                    hb2 = NULL;
    HWND                    hb3 = NULL;
    HWND                    hb4 = NULL;
    HWND                    hb5 = NULL;

    std::vector<D2D1_POINT_2F>           MDraw1;
    std::vector<D2D1_POINT_2F>           MDraw2;
    std::vector<D2D1_POINT_2F>           MDconvex1;
    std::vector<D2D1_POINT_2F>           MDconvex2;
    std::vector<D2D1_POINT_2F>           MDresult;

    std::vector<D2D1_POINT_2F>           MSraw1;
    std::vector<D2D1_POINT_2F>           MSraw2;
    std::vector<D2D1_POINT_2F>           MSconvex1;
    std::vector<D2D1_POINT_2F>           MSconvex2;
    std::vector<D2D1_POINT_2F>           MSresult;

    std::vector<D2D1_POINT_2F>           Qraw;
    std::vector<D2D1_POINT_2F>           Qresult;

    std::vector<D2D1_POINT_2F>           PCtarget;
    std::vector<D2D1_POINT_2F>           PCraw;
    std::vector<D2D1_POINT_2F>           PCconvex;

    std::vector<D2D1_POINT_2F>           GJKraw1;
    std::vector<D2D1_POINT_2F>           GJKraw2;
    std::vector<D2D1_POINT_2F>           GJKconvex1;
    std::vector<D2D1_POINT_2F>           GJKconvex2;
    std::vector<D2D1_POINT_2F>           GJKresult;
    
    std::vector<D2D1_POINT_2F>*           SelectPoly;
    std::vector<D2D1_POINT_2F>            Displacement;
    void ClearSelection3();
    void SetSelectPoly(std::vector<D2D1_POINT_2F>* vec, D2D1_POINT_2F pt);



    list<shared_ptr<MyEllipse>>             ellipses1;
    list<shared_ptr<MyEllipse>>::iterator   selection1;

    list<shared_ptr<MyEllipse>>             ellipses2;
    list<shared_ptr<MyEllipse>>::iterator   selection2;

    shared_ptr<MyEllipse> Selection1() {
        if (selection1 == ellipses1.end()) return nullptr;
        else return (*selection1);
    };

    shared_ptr<MyEllipse> Selection2() {
        if (selection2 == ellipses2.end()) return nullptr;
        else return (*selection2);
    };

    void    ClearSelection1() { selection1 = ellipses1.end(); }
    void    ClearSelection2() { selection2 = ellipses2.end(); }


    BOOL    HitTest1(float x, float y);
    BOOL    HitTest2(float x, float y);

    HRESULT InsertPoints(std::vector<D2D1_POINT_2F>* vec1, std::vector<D2D1_POINT_2F>* vec2);


    void    SetMode(Mode m);
    void    MoveSelection(float x, float y);

    //
    HRESULT CreateGraphicsResourcesM();
    //HRESULT CreateGraphicsResourcesG();
    void    DiscardGraphicsResourcesM();
    //void    DiscardGraphicsResourcesG();
    void    OnPaintM();
    //void    OnPaintG();
    //void    OnPaintB();
    void    ResizeM();
    //void    ResizeG();

    //
    void    OnLButtonDown(int pixelX, int pixelY, DWORD flags);
    void    OnLButtonUp();
    void    OnMouseMove(int pixelX, int pixelY, DWORD flags);
    void    OnKeyDown(UINT vkey);

    

    //void    OnLButtonDownG(int pixelX, int pixelY, DWORD flags);
    //void    OnLButtonUpG();
    //void    OnMouseMoveG(int pixelX, int pixelY, DWORD flags);
    //void    OnKeyDown(UINT vkey);

    //C
    void    DrawPoint(int code);
    void    DrawGraph(ID2D1HwndRenderTarget* pRT, ID2D1SolidColorBrush* pB);
    void    DrawPolygon(ID2D1HwndRenderTarget* pRT, ID2D1SolidColorBrush* pB, std::vector<D2D1_POINT_2F>* vec, D2D1_COLOR_F color);
    void    CreateLayout();
    void    GetGraphInfo(float p[]);
    float   ConvertXToDip(float x);
    float   ConvertYToDip(float y);
    float   ConvertDipToX(float x);
    float   ConvertDipToY(float y);
    //C

    void    UpdatePoint(std::vector<D2D1_POINT_2F>* vec, float x, float y, float newx, float newy);
    void    UpdatePoly(D2D1_POINT_2F pt);

    void    ShowQuickhull();
    void    ShowPointConvexHull();
    void    ShowMinkowskiSum();
    void    ShowMinkowskiDifference();
    void    ShowGJK();

    void    SelectedPointColor();

    void    ZoomIn();
    void    ZoomOut();

    void    RefreshPoints();



public:

    MainWindow() : pFactory(NULL),
        pRenderTargetM(NULL), pBrushM(NULL), ptMouseM(D2D1::Point2F()),
        pRenderTargetG(NULL), pBrushG(NULL), ptMouseG(D2D1::Point2F()), 
        SelectPoly(NULL), GridNum(40), ScreenMove(false), xoffset(0), yoffset(0),
        selection1(ellipses1.end()), selection2(ellipses2.end())
    {
    }

    PCWSTR  ClassName() const { return L"Convex Window Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

static float randx[] = { 1, 2, 0, 5, 6, -5 };
static float randy[] = { 2, 5, 8, -2, 0, -5 };

int RNG(int a, int b);
void GenerateInitialPoints(std::vector<D2D1_POINT_2F>* vec, int size, int lowrand, int upprand);
void SortPoints(std::vector<D2D1_POINT_2F>* vec);
















//#ifndef _GRAPHWIN_H
//#define _GRAPHWIN_H
//
//template <class DERIVED_TYPE>
//class GraphWindow
//{
//public:
//    static LRESULT CALLBACK GraphWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//    {
//        DERIVED_TYPE* pThis = NULL;
//
//        if (uMsg == WM_NCCREATE)
//        {
//            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
//            pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
//            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
//
//            pThis->m_hwnd = hwnd;
//        }
//        else
//        {
//            pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
//        }
//        if (pThis)
//        {
//            return pThis->HandleMessage(uMsg, wParam, lParam);
//        }
//        else
//        {
//            return DefWindowProc(hwnd, uMsg, wParam, lParam);
//        }
//    }
//
//    GraphWindow() : m_hwnd(NULL) { }
//
//    BOOL Create(
//        PCWSTR lpWindowName,
//        DWORD dwStyle,
//        DWORD dwExStyle = 0,
//        int x = 500,
//        int y = 100,
//        int nWidth = 1000,
//        int nHeight = 5000,
//        HWND hWndParent,
//        HMENU hMenu = 0
//    )
//    {
//        WNDCLASS wc = {};
//
//        wc.lpfnWndProc = DERIVED_TYPE::GraphWindowProc;
//        wc.hInstance = GetModuleHandle(NULL);
//        wc.lpszClassName = ClassName();
//
//        RegisterClass(&wc);
//
//        m_hwnd = CreateWindowEx(
//            dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
//            nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
//        );
//
//        return (m_hwnd ? TRUE : FALSE);
//    }
//
//    HWND Window() const { return m_hwnd; }
//
//protected:
//
//    virtual PCWSTR  ClassName() const = 0;
//    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
//
//    HWND m_hwnd;
//};
//
//#endif