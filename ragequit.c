#include <windows.h>
#include "KekPong.h"

HWND hhRage;
WNDCLASS rageWindowClass;

BOOL paintRagebyHWND(HWND coinHWND);

#define sin45 0.7071067812

LRESULT __stdcall RageWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
//    printf("Foreground Window: %p,\n this window: %p", GetForegroundWindow(), hWindowHandle);

    switch (message) {
        case WM_DESTROY:
            //initDestructions();
            //PostQuitMessage(0);
            return 0;
        case WM_PAINT:
            paintRagebyHWND(hWnd);
            return 0;
        case WM_LBUTTONDOWN:
            return 0;
        case WM_LBUTTONUP:
            TerminateProcess(GetCurrentProcess(), 0);
            return 0;
        case WM_MOUSEMOVE:
            return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

BOOL paintRagebyHWND(HWND coinHWND){
   PAINTSTRUCT paintStruct;
    HDC hDC = BeginPaint(coinHWND, &paintStruct);
    HGDIOBJ original = NULL;
    original = SelectObject(hDC,GetStockObject(DC_PEN));
    FillRect(hDC, &paintStruct.rcPaint, (HBRUSH)GetStockObject(BLACK_BRUSH));
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hDC, ColorRad);
    SelectObject(hDC, GetStockObject(DC_PEN));
    SetDCPenColor(hDC, ColorRad);
    Ellipse(hDC, BorderThiccness, BorderThiccness, BallRadius - BorderThiccness, BallRadius - BorderThiccness);
    
    //SelectObject(hDC, GetStockObject(DC_BRUSH));
    //SetDCBrushColor(hDC, RGB(0xff,0xff,0xff));
    //SelectObject(hDC, GetStockObject(DC_PEN));
    //SetDCPenColor(hDC, RGB(0xff,0xff,0xff));
    HPEN newPen = CreatePen(PS_SOLID, BorderThiccness, RGB(0xff,0xff,0xff));
    SelectObject(hDC, newPen);
    double br2 = BallRadius/2.0;
    double outmost = sin45 * br2 - BorderThiccness;
    double smolf = outmost/5.0*2.0;
    int smol = smolf + 1;
    MoveToEx(hDC, br2 - outmost, br2 - outmost, NULL);
    LineTo(hDC, br2 + outmost, br2 + outmost);
    MoveToEx(hDC, br2 - outmost, br2 + outmost, NULL);
    LineTo(hDC, br2 + outmost, br2 - outmost);
    
    //Rectangle(hDC, br2 - outmost             , br2 - outmost             , br2 - outmost + smol      , br2 - outmost + smol);
    //Rectangle(hDC, br2 - outmost + smol      , br2 - outmost + smol * 1.0, br2 - outmost + smol * 2.0, br2 - outmost + smol * 2.0);
    //Rectangle(hDC, br2 - outmost + smol * 2.0, br2 - outmost + smol * 2.0, br2 - outmost + smol * 3.0, br2 - outmost + smol * 3.0);
    //Rectangle(hDC, br2 - outmost + smol * 3.0, br2 - outmost + smol * 3.0, br2 - outmost + smol * 4.0, br2 - outmost + smol * 4.0);
    //Rectangle(hDC, br2 - outmost + smol * 4.0, br2 - outmost + smol * 4.0, br2 - outmost + smol * 5.0, br2 - outmost + smol * 5.0);

    //Rectangle(hDC, br2 - outmost             , br2 - outmost             , br2 - outmost + smol      , br2 - outmost + smol);
    //Rectangle(hDC, br2 - outmost + smol      , br2 - outmost + smol * 1.0, br2 - outmost + smol * 2.0, br2 - outmost + smol * 2.0);
//
    //Rectangle(hDC, br2 - outmost + smol * 3.0, br2 - outmost + smol * 3.0, br2 - outmost + smol * 4.0, br2 - outmost + smol * 4.0);
    //Rectangle(hDC, br2 - outmost + smol * 4.0, br2 - outmost + smol * 4.0, br2 - outmost + smol * 5.0, br2 - outmost + smol * 5.0);

    DeleteObject(newPen);
    SelectObject(hDC, original);
    EndPaint(coinHWND, &paintStruct);
    
    //InvalidateRect(coinHWND, NULL, FALSE);
    //char * text = "x";
    //FillRectWithText(coinHWND, text, 3, 4);
    
}


BOOL initRage(){
    rageWindowClass = (WNDCLASS){ CS_OWNDC, RageWindowProc, 0, 0, hInstance, NULL, NULL, NULL, NULL, "CringeRageWindowClass" };
    RegisterClass(&rageWindowClass);

    hhRage = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW , "CringeRageWindowClass", "", WS_POPUP | WS_VISIBLE , BatRPosX, BatBuffer, BallRadius, BallRadius, NULL, NULL, hInstance, NULL);
    HRGN rgnCoin = CreateEllipticRgn(0, 0, BallRadius, BallRadius);
    if(!SetWindowRgn(hhRage, rgnCoin, TRUE)){
        printf("Could not set the window region: %lu\n", GetLastError());
    }
    BringWindowToTop(hhRage);
    UpdateWindow(hhRage);
    paintRagebyHWND(hhRage);

    SetCursor(LoadCursor(NULL, IDC_ARROW));
    
    BringWindowToTop(hhRage);
    UpdateWindow(hhRage);

    paintRagebyHWND(hhRage);
}