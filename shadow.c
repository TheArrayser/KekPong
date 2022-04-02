#include "KekPong.h"
typedef int BOOL;
#define TRUE 1
#define FALSE 0

int BallSpawnX = 0;
int BallSpawnY = 0;
int BallRadius = 0;

float BallSpeedX = 30;
float BallSpeedY = 20;
int BallPosX = 0;
int BallPosY = 0;
BOOL isBallAlive = TRUE;

int BatLPosX = 0;
int BatLPosY = 0;

int BatRPosX = 0;
int BatRPosY = 0;

int BatHight = 0;


HANDLE physics;

void randomizeSpeed(){
    int factorX = 1;
    int factorY = 1;
    if(BallSpeedX < 0) factorX = -1;
    //if(BallSpeedY < 0) factorY = -1;
    factorY = random_number(0,1) ? 1 : -1;
    BallSpeedX = random_number(20, 40) * factorX;
    BallSpeedY = random_number(20, 40) * factorY;
}

DWORD WINAPI AdvancePhysics(LPVOID lpParameter){
    while(1){
        if(!isBallAlive) return;
        BallPosX += BallSpeedX;
        BallPosY += BallSpeedY;
        if(BallPosY > screenhight - (BallRadius + BatBuffer)){
            BallSpeedY = -BallSpeedY;
        }
        if(BallPosY < BallRadius + BatBuffer){
            BallSpeedY = -BallSpeedY;
        }
        if(BallPosX - BallRadius < BatBuffer + BallRadius){
            BallSpeedX = -BallSpeedX;
            //Beep( 750, 150 );
            //Beep(659, 150);
            
            if(BallPosY < BatLPosY || BallPosY > BatLPosY + BatHight){
                //Beep(784, 150);
                
                if(IncrementScoreAndUpdate(FALSE)){
                    prayFor(5);
                    MessageBox(NULL, "Team Blue won the match!", "Congratulations", MB_OK /*| MB_ICONWARNING*/);
                    TerminateProcess(GetCurrentProcess(), 0);
                }
                randomizeSpeed();
                prayFor(3);
                resetBall();
            }else{
                prayFor(1);
            }
        }
        if(BallPosX + BallRadius > screenwidth - (BatBuffer + BallRadius)){
            BallSpeedX = -BallSpeedX;
            //Beep( 494, 150 );
            
            if(BallPosY < BatRPosY || BallPosY > BatRPosY + BatHight){
                //Beep(415, 150);
                
                if(IncrementScoreAndUpdate(TRUE)){
                    prayFor(4);
                    MessageBox(NULL, "Team Orange won the match!", "Congratulations", MB_OK /*| MB_ICONWARNING*/);
                    TerminateProcess(GetCurrentProcess(), 0);
                }
                randomizeSpeed();
                prayFor(2);
                resetBall();
            }else{
                prayFor(0);
            }
        }
        setWindowPosBall(BallPosX, BallPosY);
        Sleep(100);
    }
}

void resetBall(){
    BallPosX = BallSpawnX;
    BallPosY = BallSpawnY;
}

BOOL startPhysics(){
    physics = CreateThread(NULL,0,&AdvancePhysics,0,0,NULL);
    if(physics == INVALID_HANDLE_VALUE || physics == NULL){
        printf("Error with Physics: %lu",GetLastError());
        return 1;
    }
}

void initShadow(){
    resetBall();
    startPhysics();
}

//int ScreenToRaster(int screen){
//    return screen / blockdim;
//}
//
//int RasterToScreen(int raster){
//    return blockdim * raster;
//}