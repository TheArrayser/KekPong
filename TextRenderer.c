
#include <windows.h>
#include <stdio.h>
#include "KekPong.h"

//#define FontSize 2

HBITMAP FontBMP = NULL;
SIZE FontBMPSize;
SIZE FontCharacterSize;

void PrintError(unsigned int line){
    printf("%i %i\n", line, GetLastError());
}

void FillRectWithText(HWND hWnd, char * MESSAGE, unsigned int msgLen, int FontSize){
    PAINTSTRUCT paintStruct;
    HDC hDC = BeginPaint(hWnd, &paintStruct);
    // Create a memory device compatible with the above DC variable
    
    HDC MemDC = CreateCompatibleDC(hDC);
    HDC AntiFontDC = CreateCompatibleDC(MemDC);
    HDC FontDC = CreateCompatibleDC(AntiFontDC);
    if(!(hDC && MemDC && AntiFontDC && FontDC)){
        printf("Error 1 %i\n", GetLastError());
    }
    int unusedbuffer;
    BITMAP btminfoforcrash;
    ZeroMemory( &btminfoforcrash,sizeof( BITMAP ) );

    unusedbuffer = GetObject(FontBMP,sizeof(BITMAP),&btminfoforcrash);
    //printf("%i %i %i %i %i %u %u %p\n", unusedbuffer,  btminfoforcrash.bmType, btminfoforcrash.bmWidth,
    //                btminfoforcrash.bmHeight,
    //                btminfoforcrash.bmWidthBytes,
    //                btminfoforcrash.bmPlanes,
    //                btminfoforcrash.bmBitsPixel,
    //                btminfoforcrash.bmBits);
    //printf("device: %i\n", GetDeviceCaps(FontDC, BITSPIXEL));

    HGDIOBJ selectObjectRet;
    // Select the new bitmap
    HBITMAP hOldBmp = SelectObject(FontDC, FontBMP);
    if(!hOldBmp){
        PrintError(__LINE__);
    }
    if(hOldBmp == HGDI_ERROR){
        PrintError(__LINE__);
        //printf("%i %i\n", __LINE__, GetLastError());
    }

    HBITMAP antibmp = CreateCompatibleBitmap(AntiFontDC, FontBMPSize.cx, FontBMPSize.cy);
    selectObjectRet = SelectObject(AntiFontDC, antibmp);
    if(!selectObjectRet || selectObjectRet == HGDI_ERROR){
        PrintError(__LINE__);
        //printf("%s", __LINE__);
    }

    unsigned int newLines = 1;
    for(int i = 0; MESSAGE[i] != '\0'; i++){
        if(MESSAGE[i] == '\n'){
            newLines++;
        }
    }

    HBITMAP membtm = CreateCompatibleBitmap(MemDC,FontCharacterSize.cx * msgLen/sizeof(MESSAGE[0]), FontCharacterSize.cy * newLines);
    selectObjectRet = SelectObject(MemDC, membtm);
    if(!selectObjectRet || selectObjectRet == HGDI_ERROR){
        PrintError(__LINE__);
        //printf("%s", __LINE__);
    }
    //SelectObject(MemDC, FontBMP);

    // Copy the bits from the memory DC into the current dc
    if(!BitBlt(AntiFontDC, 0, 0, FontBMPSize.cx, FontBMPSize.cy, FontDC, 0, 0, NOTSRCCOPY)){
        printf("e");
    }
    //BitBlt(MemDC, 0, 0, FontBMPSize.cx, FontBMPSize.cy, FontDC, 0, 0, SRCCOPY);

    unsigned int currentLine = 0;
    unsigned int currentCharPos = 0;
    for(int i = 0; MESSAGE[i] != '\0'; i++){
        if(MESSAGE[i] == '\n'){
            currentLine++;
            currentCharPos = 0;
            continue;
        }
        unsigned int charx = MESSAGE[i] % 16;
        unsigned int chary = MESSAGE[i] / 16;
        charx = charx * FontCharacterSize.cx;
        chary = chary * FontCharacterSize.cy;

        //printf("Next c: %u - charx: %u chary: %u - FontSize: x: %u y: %u\n", MESSAGE[i], charx, chary, FontCharacterSize.cx, FontCharacterSize.cy);
        if(!BitBlt(MemDC, currentCharPos * FontCharacterSize.cx, currentLine * FontCharacterSize.cy, FontCharacterSize.cx, FontCharacterSize.cy, AntiFontDC, charx, chary, SRCCOPY)){
            printf("u");
        }
        currentCharPos++;
    }
    
    //for(int y = 0; y < 128; y++){
    //    for(int x = 0; x < 128; x++){
    //        COLORREF curentcol = GetPixel(hDC,x,y);
    //        if(curentcol == CLR_INVALID){
    //            printf("inval");
    //        }else{
    //            printf("%lx", GetPixel(hDC,x,y));
    //        }
    //    }
    //    printf("\n");
    //}

    //if(!BitBlt(hDC, 0, 0, FontCharacterSize.cx * sizeof(MESSAGE)/sizeof(MESSAGE[0]), FontCharacterSize.cy, MemDC, 0, 0, SRCPAINT)){
    //    printf("q");
    //}
    //printf("4: %lu\n", GetPixel(hDC,0,0));
    //printf("5: %lu\n", GetPixel(hDC,40,33));
    //StretchBlt(hDC, 0, 0, 8 * 8, 8 * 8, MemDC, 0, 0, 4*8, 4*8, SRCCOPY);
    if(!StretchBlt(hDC, FontSize, FontSize, FontCharacterSize.cx * msgLen/sizeof(MESSAGE[0])  * FontSize, FontCharacterSize.cy * FontSize * newLines,  MemDC, 0, 0, FontCharacterSize.cx * msgLen/sizeof(MESSAGE[0]),  FontCharacterSize.cy * newLines, SRCCOPY)){
        printf("error stretching\n");
    }

    // Restore the old bitmap
    selectObjectRet = SelectObject(MemDC, hOldBmp);
    if(!selectObjectRet || selectObjectRet == HGDI_ERROR){
        PrintError(__LINE__);
        //printf("%s", __LINE__);
    }
    BOOL boolret = TRUE;
    boolret &= DeleteDC(FontDC);
    boolret &= DeleteDC(AntiFontDC);
    boolret &= DeleteObject(antibmp);
    boolret &= DeleteDC(MemDC);
    boolret &= DeleteObject(membtm);
    boolret &= EndPaint(hWnd, &paintStruct);
    if(!boolret){
        printf("An error occured here %s\n", __LINE__);
    }
    //printf("Last Error: %i\n", GetLastError());
}

void LoadFont(){
    FontBMP = LoadBitmap(hInstance, MAKEINTRESOURCE(99));
    //GetBitmapDimensionEx(FontBMP, &FontBMPSize);
    //printf("Size: x:%i y: %i\n", FontBMPSize.cx, FontBMPSize.cy);

    BITMAP  bitmapInfo;

    ZeroMemory( &bitmapInfo,sizeof( bitmapInfo ) );
    
    GetObject(FontBMP, sizeof(BITMAP), &bitmapInfo);
    FontBMPSize.cx = bitmapInfo.bmWidth;
    FontBMPSize.cy = bitmapInfo.bmHeight;
    FontCharacterSize.cx = bitmapInfo.bmWidth / 16;
    FontCharacterSize.cy = bitmapInfo.bmHeight / 16;

    printf("Size: x:%i y: %i\n", FontBMPSize.cx, FontBMPSize.cy);

    //std::cout << "Your bitmap is "  << bitmapInfo.bmWidth
    //            << "x"                << bitmapInfo.bmHeight
    //            << " at "             << bitmapInfo.bmBitsPixel
    //            << " bits per pixel." << std::endl;
}