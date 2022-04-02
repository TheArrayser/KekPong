#include <windows.h>
#include "KekPong.h"

HWND hhBat1;
HWND hhBat2;
WNDCLASS batWindowClass;

int mousex;
int mousey;
BOOL Lmoving = FALSE;
BOOL Rmoving = FALSE;

BOOL paintBatbyHWND(HWND coinHWND);

LRESULT __stdcall BatWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
//    printf("Foreground Window: %p,\n this window: %p", GetForegroundWindow(), hWindowHandle);

    switch (message) {
        case WM_DESTROY:
            //initDestructions();
            //PostQuitMessage(0);
            return 0;
        case WM_PAINT:
            paintBatbyHWND(hWnd);
            return 0;
        case WM_LBUTTONDOWN:
            SetCapture(hWnd);
            mousex = LOWORD(lParam);
            mousey = HIWORD(lParam);
            if(hWnd == hhBat1){
                Lmoving = TRUE;
            }else if (hWnd == hhBat2){
                Rmoving = TRUE;
            }
            
            return 0;
        case WM_LBUTTONUP:
            ReleaseCapture();
            Rmoving = FALSE;
            Lmoving = FALSE;
            return 0;
        case WM_MOUSEMOVE:
        {
            if (Rmoving || Lmoving) {
                int new_mousex = LOWORD(lParam);
                int new_mousey = HIWORD(lParam);
                RECT windowRect;
                GetWindowRect(hWnd, &windowRect);

                int deltaY = new_mousey - mousey;
                if(Lmoving){
                    if(BatLPosY + deltaY < BatBuffer || BatLPosY + BatHight + deltaY > screenhight - BatBuffer){
                        deltaY = 0;
                    }
                    BatLPosY += deltaY;
                    SetWindowPos(hWnd, NULL, BatLPosX, BatLPosY, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
                }else{
                    if(BatRPosY + deltaY < BatBuffer || BatRPosY + BatHight + deltaY > screenhight - BatBuffer){
                        deltaY = 0;
                    }
                    BatRPosY += deltaY;
                    SetWindowPos(hWnd, NULL, BatRPosX, BatRPosY, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
                }

                
                InvalidateRect(hWnd, NULL, FALSE);
            }
        }
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

BOOL paintBatbyHWND(HWND coinHWND){
    PAINTSTRUCT paintStruct;
    HDC hDC = BeginPaint(coinHWND, &paintStruct);
    HGDIOBJ original = NULL;
    original = SelectObject(hDC,GetStockObject(DC_PEN));
    FillRect(hDC, &paintStruct.rcPaint, (HBRUSH)GetStockObject(BLACK_BRUSH));
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    if(coinHWND == hhBat1){
        SetDCBrushColor(hDC, ColorOrenge);    
    }else{
        SetDCBrushColor(hDC,ColorBlew);
    }
    
    SelectObject(hDC, GetStockObject(DC_PEN));
    if(coinHWND == hhBat1){
        SetDCPenColor(hDC, ColorOrenge);
    }else{
        SetDCPenColor(hDC, ColorBlew);
    }
    Rectangle(hDC, BorderThiccness, BorderThiccness, BallRadius - BorderThiccness, BatHight - BorderThiccness);
    //Ellipse(hDC, BallSpawnX - BallRadius + BorderThiccness, BallSpawnY - BallRadius + BorderThiccness, BallSpawnX + BallRadius - BorderThiccness, BallSpawnY + BallRadius - BorderThiccness);
    SelectObject(hDC, original);
    EndPaint(coinHWND, &paintStruct);
}

//BOOL resetBall(){
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

BOOL initBat(){

    batWindowClass = (WNDCLASS){ CS_OWNDC, BatWindowProc, 0, 0, hInstance, NULL, NULL, NULL, NULL, "CringeBatWindowClass" };
    RegisterClass(&batWindowClass);

    BatLPosX = BatBuffer;
    BatLPosY = BallSpawnY - BatHight/2;
    BatRPosX = screenwidth - BatBuffer - BallRadius;
    BatRPosY = BallSpawnY - BatHight/2;

    hhBat1 = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW , "CringeBatWindowClass", "", WS_POPUP | WS_VISIBLE , BatBuffer, BallSpawnY - BatHight/2, BallRadius, BatHight, NULL, NULL, hInstance, NULL);
    hhBat2 = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW , "CringeBatWindowClass", "", WS_POPUP | WS_VISIBLE , screenwidth - BatBuffer - BallRadius, BallSpawnY - BatHight/2, BallRadius, BatHight, NULL, NULL, hInstance, NULL);
    
    BringWindowToTop(hhBat1);
    UpdateWindow(hhBat1);
    paintBatbyHWND(hhBat1);

    BringWindowToTop(hhBat2);
    UpdateWindow(hhBat2);
    paintBatbyHWND(hhBat2);

    SetCursor(LoadCursor(NULL, IDC_ARROW));
    
    BringWindowToTop(hhBat1);
    UpdateWindow(hhBat1);
    BringWindowToTop(hhBat2);
    UpdateWindow(hhBat2);

    paintBatbyHWND(hhBat1);
    paintBatbyHWND(hhBat2);
}

BOOL removeBats(){
    isBallAlive = FALSE;
    return DestroyWindow(hhBat1);
}