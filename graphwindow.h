#include <windows.h>
#include <Windowsx.h>
#include <d2d1.h>

#include <list>
#include <memory>
using namespace std;

#pragma comment(lib, "d2d1")

#include "basewin.h"
#include "resource.h"
#include "covexhullwindow.h"


class GraphWindow : public BaseWindow<GraphWindow>
{
    void    OnPaint();
public:
    PCWSTR  ClassName() const { return L"Graph Window Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};