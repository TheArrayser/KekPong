#include <windows.h>

#define BorderThiccness 5

//#define BatHeight 4
//#define BatWidth 1

//#define BallHeight 1
//#define BallWidth 1

#define BatBuffer BallRadius

#define StatsInnerBuffer (BallRadius * 2)

//Color One 0x000000
//Color Two 0xFFFFFF

#define ColorOrenge RGB(0xff,0x5d,0)
#define ColorBlew RGB(0x0,0x65,0xFF)
#define ColorRad RGB(0XFF, 0, 0)

extern HINSTANCE hInstance;
extern int screenwidth;
extern int screenhight;
extern int blockdim;

//Ball
extern int BallRadius;
BOOL initBall();
BOOL removeBall();
//BOOL resetBallWindow();
//BOOL setBallPos();
BOOL setWindowPosBall(int windowPosX, int windowPosY);

//Shadow
void initShadow();
extern int BallSpawnX;
extern int BallSpawnY;
extern int BallRadius;

extern float BallSpeedX;
extern float BallSpeedY;
extern int RasterBallPosX;
extern int RasterBallPosY;
extern BOOL isBallAlive;

extern int BatLPosX;
extern int BatLPosY;

extern int BatRPosX;
extern int BatRPosY;

extern int BatHight;

int ScreenToRaster(int screen);
int RasterToScreen(int raster);
void resetBall();

//Stats
//returns true if game should end
BOOL IncrementScoreAndUpdate(BOOL isLeft);
BOOL initStats();

//Bat
BOOL initBat();

//Text Renderer
void FillRectWithText(HWND hWnd, char * MESSAGE, unsigned int msgLen, int fontSize);
//void FillRectWithText(HWND hWnd);
void LoadFont();

//Ragequit
BOOL initRage();

//SoundPrayer
void initSound();
void prayFor(char ProgramI);

//main
void DebugError(char * func, unsigned int line);

//util
void setRandom();
int random_number(int min_num, int max_num);