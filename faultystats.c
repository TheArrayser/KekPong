typedef int BOOL;
#define TRUE 1
#define FALSE 0
#define DECLARE_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name
DECLARE_HANDLE (HWND);
HWND hhStat1;

int paintStatsbyHWND(HWND hWnd);


BOOL paintStatsByHWND(HWND hWnd){
    return;
}

BOOL initStats(){
    paintStatsbyHWND(hhStat1);
}