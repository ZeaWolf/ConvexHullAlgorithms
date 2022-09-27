#define ID_MD_BUTTON		380
#define ID_MS_BUTTON		381
#define ID_Q_BUTTON			382
#define ID_PCH_BUTTON		383
#define ID_GJK_BUTTON		384
#define ID_GRAPH			385


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