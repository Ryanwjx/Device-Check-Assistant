#ifndef _DISPLAY_MANAGER_H
#define _DISPLAY_MANAGER_H

#include "typedef.h"
#include "font_manager.h"

typedef struct DispBuff{
	int iXres;
	int iYres;
	int iBpp;
	char *Buff;
}DispBuff, *PDispBuff;

typedef struct DispOpr{
	char *name;
	int (*DeviceInit)(void);
	int (*DeviceExit)(void);
	int (*GetBuffer)(PDispBuff ptDispBuff);
	int (*FlushRegion)(PRegion ptRegion);
	struct DispOpr * ptNext;
}DispOpr, *PDispOpr;

void RegisterDisplay(PDispOpr ptDispOpr);

void DisplaySystemRegister(void);
int SelectDefaultDisplay(char * name);
int InitDefaultDisplay(void);
int PutPixel(int x, int y, unsigned int dwColor);
int DrawFontBitMap(PFontBitMap ptFontBitMap, unsigned int dwColor);
void DrawTextInRegionCentral(char *name, PRegion ptRegion, unsigned int dwColor);
void DrawRegion(PRegion ptRegion, unsigned int dwColor);

PDispBuff GetDisplayBuffer(void);
int FlushDisplayRegion(PRegion ptRegion);



#endif //_DISPLAY_MANAGER_H
