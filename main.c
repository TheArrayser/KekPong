#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <stdio.h>

#include "KekPong.h"

int blockdim = 0;

//int KekmanR = 0;
//int KekmanPosx = 0;
//int KekmanPosy = 0;
//int coinR = 0;

int screenwidth;
int screenhight;
HINSTANCE hInstance;

void DebugError(char * func, unsigned int line){
    DWORD e = GetLastError();
    if(e){
        printf("Error @ %s %u %i", func, line, e);
        printf(" Test: %i", GetLastError());
    }
}

void ErrorMessageBox(int pos, int error){
    //TODO
}

void NameToBlockDim(){
    LPSTR fileName = malloc(MAX_PATH);
    DWORD fileSize = GetModuleFileNameA((HMODULE)hInstance, fileName, MAX_PATH);

    if(fileSize == 0){
        //Todo Error
        GetLastError();
    }
    if(fileSize > MAX_PATH){
        //TODO Error
    }
    if(fileSize < (4)){ //w0000.exe
        //Todo Error
    }
    if(*(fileName + fileSize - 4) != '.'){
        printf("Unknown file Type");
        //Todo Error
    }
    if(fileSize < (5+4)){
        blockdim = screenhight/16;
    }else{
        if(      *(fileName + fileSize - 4 - 5) == 'w'){
            char s[] = {*(fileName + fileSize - 4 - 4),
                        *(fileName + fileSize - 4 - 3),
                        *(fileName + fileSize - 4 - 2),
                        *(fileName + fileSize - 4 - 1)};
            int div = 0;
            int exp = 1000;
            for(int i = 0; i < 4; i++){
                if(s[i] < '0' || s[i] > '9'){
                    blockdim = screenhight/16;
                    goto end;
                }
                div += (s[i] - '0') * exp;
                exp /= 10;
            }

            blockdim = screenwidth/div;
            goto end;
        }else if(*(fileName + fileSize - 4 - 5) == 'h'){
            char s[] = {*(fileName + fileSize - 4 - 4),
                        *(fileName + fileSize - 4 - 3),
                        *(fileName + fileSize - 4 - 2),
                        *(fileName + fileSize - 4 - 1)};
            int div = 0;
            int exp = 1000;
            for(int i = 0; i < 4; i++){
                if(s[i] < '0' || s[i] > '9'){
                    blockdim = screenhight/16;
                    goto end;
                }
                div += (s[i] - '0') * exp;
                exp /= 10;
            }

            blockdim = screenhight/div;
            goto end;
            
        }else{
            blockdim = screenhight/16;
        }
    }

  end:    
    printf("%s -> %i\n", fileName, blockdim);
    free(fileName);
}

void initPositions(){
    BallSpawnX = screenwidth/2;
    BallSpawnY = screenhight/2;

    enum WidthOrHight {Width, Hight};
    enum WidthOrHight used = (screenwidth < screenhight ? Width : Hight);
    int thirdable = (used == Width ? screenwidth : screenhight);

    BatHight = thirdable/3;
    BallRadius = BatHight/4/2;
}

int __stdcall WinMain(HINSTANCE hInstanceoo, HINSTANCE hPrevious, LPSTR cmdLine, int cmdShow) {
    setRandom();
    //PlaySound(MAKEINTRESOURCE(10), NULL, SND_RESOURCE);
    //if(!MessageBox(NULL, "Team Orange won the match!", "Congratulations", MB_OK | 0x20)){
    //    printf("E: %i\n", GetLastError());
    //}
    screenwidth = GetSystemMetrics(SM_CXSCREEN);
    screenhight = GetSystemMetrics(SM_CYSCREEN);
    hInstance = hInstanceoo;
    
    NameToBlockDim();
    
    initPositions();
    

    initBall();
    
    initBat();
    
    initStats();
    
    initRage();
    
    initSound();
    
    initShadow();
    

    //initCoins();
    //initKekman();

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)){
        DispatchMessage(&msg);
    };
    //initMainWindow();

    return 0;
}