#include <windows.h>
#include "KekPong.h"

HWND hhBall;
WNDCLASS ballWindowClass;

BOOL paintBallbyHWND(HWND coinHWND);

LRESULT __stdcall BallWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
//    printf("Foreground Window: %p,\n this window: %p", GetForegroundWindow(), hWindowHandle);

    switch (message) {
        case WM_DESTROY:
            //initDestructions();
            //PostQuitMessage(0);
            return 0;
        case WM_PAINT:
            paintBallbyHWND(hWnd);
            return 0;
        case WM_LBUTTONDOWN:
            return 0;
        case WM_LBUTTONUP:
            return 0;
        case WM_MOUSEMOVE:
            return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

BOOL paintBallbyHWND(HWND coinHWND){
   PAINTSTRUCT paintStruct;
    HDC hDC = BeginPaint(coinHWND, &paintStruct);
    HGDIOBJ original = NULL;
    original = SelectObject(hDC,GetStockObject(DC_PEN));
    FillRect(hDC, &paintStruct.rcPaint, (HBRUSH)GetStockObject(BLACK_BRUSH));
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hDC,RGB(0xff,0xff,0x00));
    SelectObject(hDC, GetStockObject(DC_PEN));
    SetDCPenColor(hDC, RGB(0xff,0xff,0x00));
    Ellipse(hDC, BorderThiccness, BorderThiccness, 2 * BallRadius - BorderThiccness, 2 * BallRadius - BorderThiccness);
    SelectObject(hDC, original);
    EndPaint(coinHWND, &paintStruct);
}

//BOOL resetBallWindow(){
//    for(unsigned int i = 0; i < NumberOfCoins; i++){
//        daCoins[i].hCoin = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW , "CringeCoinWindowClass", "", WS_POPUP | WS_VISIBLE , 0, 0, screenwidth, screenhight, NULL, NULL, //hInstance, NULL);
//        HRGN rgnCoin = CreateEllipticRgn(daCoins[i].posX - coinR, daCoins[i].posY - coinR, daCoins[i].posX + coinR, daCoins[i].posY + coinR);
//        if(!SetWindowRgn(daCoins[i].hCoin, rgnCoin, TRUE)){
//            printf("Could not set the window region: %lu\n", GetLastError());
//        }
//        BringWindowToTop(daCoins[i].hCoin);
//        UpdateWindow(daCoins[i].hCoin);
//        paintCoin(i);
//    }
//}

BOOL initBall(){

    ballWindowClass = (WNDCLASS){ CS_OWNDC, BallWindowProc, 0, 0, hInstance, NULL, NULL, NULL, NULL, "CringeBallWindowClass" };
    RegisterClass(&ballWindowClass);

    DebugError(__FUNCTION__, __LINE__);
    hhBall = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW , "CringeBallWindowClass", "", WS_POPUP | WS_VISIBLE , BallSpawnX - BallRadius, BallSpawnY - BallRadius, BallRadius * 2, BallRadius * 2, NULL, NULL, hInstance, NULL);
    DebugError(__FUNCTION__, __LINE__);
    HRGN rgnCoin = CreateEllipticRgn(0, 0, 2 * BallRadius, 2 * BallRadius);
    
    if(!SetWindowRgn(hhBall, rgnCoin, TRUE)){
        printf("Could not set the window region: %lu\n", GetLastError());
    }
    BringWindowToTop(hhBall);
    UpdateWindow(hhBall);
    paintBallbyHWND(hhBall);

    SetCursor(LoadCursor(NULL, IDC_ARROW));
    
    BringWindowToTop(hhBall);
    UpdateWindow(hhBall);

    paintBallbyHWND(hhBall);
}

BOOL removeBall(){
    isBallAlive = FALSE;
    return DestroyWindow(hhBall);
}

BOOL setWindowPosBall(int windowPosX, int windowPosY){
    BOOL ret = SetWindowPos(hhBall, NULL, windowPosX - BallRadius, windowPosY - BallRadius, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);           
    ret |= InvalidateRect(hhBall, NULL, FALSE);
    return ret;
}