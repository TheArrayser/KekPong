#include <windows.h>
#include "KekPong.h"


HANDLE speaker;

char previousProgram = -1;
char currentProgram = -1;
unsigned char currentIndex;

BOOL oldPlayer = FALSE;

struct programInfo{
    struct nprogramInfo * soundI;
    unsigned int ElementsInProgram;
};

struct nprogramInfo{
    DWORD frequency;
    DWORD duration;
};
struct nprogramInfo leftTouch[] = {{494, 150}};
struct nprogramInfo rightTouch[] = {{659, 150}};
struct nprogramInfo leftScore[] = {{494, 150},{415, 150}};
struct nprogramInfo rightScore[] = {{659, 150},{784, 150}};
struct nprogramInfo leftWin[] = {{659, 150},{494, 150},{415, 150}};
struct nprogramInfo rightWin[] = {{494, 150},{659, 150},{784, 150}};

struct programInfo program[] = {
    {leftTouch, sizeof(leftTouch)/sizeof(leftTouch[0])},
    {rightTouch, sizeof(rightTouch)/sizeof(rightTouch[0])},
    {leftScore, sizeof(leftScore)/sizeof(leftScore[0])},
    {rightScore, sizeof(rightScore)/sizeof(rightScore[0])},
    {leftWin, sizeof(leftWin)/sizeof(leftWin[0])},
    {rightWin, sizeof(rightWin)/sizeof(rightWin[0])}
};

DWORD WINAPI SoundThread(LPVOID lpParameter){
    if(oldPlayer){
        while(1){
            while(currentProgram>=0){
                if(currentProgram!=previousProgram){
                    previousProgram = currentProgram;
                    currentIndex = 0;
                }
                if(currentIndex >= program[currentProgram].ElementsInProgram) break;
                if(!Beep(program[currentProgram].soundI[currentIndex].frequency, program[currentProgram].soundI[currentIndex].duration)){
                    printf("Error while playin! %i", GetLastError());
                }
                currentIndex++;
                //Beep( 750, 150 );
            }
            previousProgram = -1;
            currentProgram = -1;
            currentIndex = 0;
            Sleep(100);
        }
    }else{
        while(1){
            if(currentProgram>=0){
                PlaySound(NULL, NULL, 0);
                unsigned int soundtype = 0;
                if(currentProgram == 0 || currentProgram == 1) soundtype = 10 + random_number(0,11);
                if(currentProgram == 2 || currentProgram == 3) soundtype = 30 + random_number(0,4);
                if(currentProgram == 4 || currentProgram == 5) soundtype = 40 + random_number(0,1);
                //printf("Random was: %u\n", soundtype);
                if(!PlaySound(MAKEINTRESOURCE(soundtype), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC)){
                    printf("COuld not play sound: %i\n", GetLastError());
                }else{
                    printf("Could play sound\n");
                }
                currentProgram = -1;
            }
            Sleep(100);
        }
    }
}

void initSound(){
    oldPlayer = random_number(0,1);

    speaker = CreateThread(NULL,0,&SoundThread,0,0,NULL);
    if(speaker == INVALID_HANDLE_VALUE || speaker == NULL){
        printf("Error with Physics: %lu",GetLastError());
        return 1;
    }
}

void prayFor(char ProgramI){
    currentProgram = ProgramI;
}