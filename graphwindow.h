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

//class GraphWindow {
//private:
//	HWND gwin = NULL;
//	void OnPaint();
//public:
//	static LRESULT CALLBACK GraphWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
//	GraphWindow(HWND hwnd);
//	HWND getGwin();
//};

//HWND CreateGraph(HWND hwnd);
//LRESULT CALLBACK GraphWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//void OnPaint(HWND hWnd);