#include <windows.h>
#include "KekPong.h"

unsigned char LeftScore = 0;
unsigned char RightScore = 0;

HWND hhStat1;
HWND hhStat2;
WNDCLASS statsWindowClass;

BOOL paintStatsbyHWND(HWND hWnd);

LRESULT __stdcall StatsWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
//    printf("Foreground Window: %p,\n this window: %p", GetForegroundWindow(), hWindowHandle);

    switch (message) {
        case WM_DESTROY:
            //initDestructions();
            //PostQuitMessage(0);
            return 0;
        case WM_PAINT:
            paintStatsbyHWND(hWnd);
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

//returns true if game should end
BOOL IncrementScoreAndUpdate(BOOL isLeft){
    if(isLeft){ //Left
        if(LeftScore == 9){
            //Should End
            //MessageBox(NULL, "Team Orange won the match!", "Congratulations", MB_OK /*| MB_ICONWARNING*/);
            return TRUE;
        }
        LeftScore++;
        updateStatsWindow(isLeft);
        return FALSE;
    }else{
        if(RightScore == 9){
            //Should End
            //MessageBox(NULL, "Team Blue won the match!", "Congratulations", MB_OK /*| MB_ICONWARNING*/);
            return TRUE;
        }
        RightScore++;
        updateStatsWindow(isLeft);
        return FALSE;
    }
}

BOOL paintStatsbyHWND(HWND hWnd){
    PAINTSTRUCT paintStruct;
    HDC hDC = BeginPaint(hWnd, &paintStruct);
    HGDIOBJ original = NULL;
    original = SelectObject(hDC,GetStockObject(DC_PEN));
    FillRect(hDC, &paintStruct.rcPaint, (HBRUSH)GetStockObject(BLACK_BRUSH));
    //SelectObject(hDC, GetStockObject(DC_BRUSH));
    //SetDCBrushColor(hDC,RGB(0xff,0xff,0x00));
    //SelectObject(hDC, GetStockObject(DC_PEN));
    //SetDCPenColor(hDC, RGB(0xff,0xff,0x00));
    //Rectangle(hDC, BorderThiccness, BorderThiccness, BallRadius - BorderThiccness, BatHight - BorderThiccness);
    
    //Ellipse(hDC, BallSpawnX - BallRadius + BorderThiccness, BallSpawnY - BallRadius + BorderThiccness, BallSpawnX + BallRadius - BorderThiccness, BallSpawnY + BallRadius - BorderThiccness);
    SelectObject(hDC, original);
    EndPaint(hWnd, &paintStruct);
    InvalidateRect(hWnd, NULL, FALSE);
    unsigned char text[] = "0";
    if(hWnd == hhStat1){
        *text = LeftScore + '0';
        FillRectWithText(hWnd, &text, 1, BatHight/16);
    }else if(hWnd == hhStat2){
        *text = RightScore + '0';
        FillRectWithText(hWnd, &text[0], 1, BatHight/16);
    }
    
}

BOOL updateStatsWindow(BOOL isLeft){
    //drawStatsByHWND();
    if(isLeft){
        paintStatsbyHWND(hhStat1);
    }else{
        paintStatsbyHWND(hhStat2);
    }
}

BOOL initStats(){
    LoadFont();
    statsWindowClass = (WNDCLASS){ CS_OWNDC, StatsWindowProc, 0, 0, hInstance, NULL, NULL, NULL, NULL, "CringeStatWindowClass" };
    RegisterClass(&statsWindowClass);

    hhStat1 = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW , "CringeStatWindowClass", "", WS_POPUP | WS_VISIBLE , BallSpawnX - StatsInnerBuffer - BatHight/2, BatBuffer, BatHight/2, BatHight/2, NULL, NULL, hInstance, NULL);
    hhStat2 = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW , "CringeStatWindowClass", "", WS_POPUP | WS_VISIBLE , BallSpawnX + StatsInnerBuffer            , BatBuffer, BatHight/2, BatHight/2, NULL, NULL, hInstance, NULL);

    if(!SetWindowLong(hhStat1, GWL_EXSTYLE, GetWindowLong(hhStat1, GWL_EXSTYLE) | WS_EX_LAYERED)){
        printf("Error: %i\n", GetLastError());
    }

    if(!SetWindowLong(hhStat2, GWL_EXSTYLE, GetWindowLong(hhStat1, GWL_EXSTYLE) | WS_EX_LAYERED)){
        printf("Error: %i\n", GetLastError());
    }
    // Make red pixels transparent:
    SetLayeredWindowAttributes(hhStat1, RGB(0,0,0), 0, LWA_COLORKEY);
    SetLayeredWindowAttributes(hhStat2, RGB(0,0,0), 0, LWA_COLORKEY);

    if(hhStat1 == NULL || hhStat2 == NULL){
        printf("Error: %i\n", GetLastError());
    }
    printf("CreatedWindow\n");
    
    BringWindowToTop(hhStat1);
    UpdateWindow(hhStat1);
    paintStatsbyHWND(hhStat1);

    BringWindowToTop(hhStat2);
    UpdateWindow(hhStat2);
    paintStatsbyHWND(hhStat2);

    SetCursor(LoadCursor(NULL, IDC_ARROW));
    
    BringWindowToTop(hhStat1);
    UpdateWindow(hhStat1);
    BringWindowToTop(hhStat2);
    UpdateWindow(hhStat2);

    paintStatsbyHWND(hhStat1);
    paintStatsbyHWND(hhStat2);
    //printf("Error: %i\n", GetLastError());
}

BOOL destroyStatsWindow(){
    BOOL ret = DestroyWindow(hhStat1);
    ret |= DestroyWindow(hhStat2);
    return ret;
}