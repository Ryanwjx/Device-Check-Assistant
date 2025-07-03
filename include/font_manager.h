#ifndef _FONT_MANAGER_H
#define _FONT_MANAGER_H

#include "typedef.h"

typedef int FontSize;

typedef struct FontBitMap{
	int iCurOriginX;	//笛卡尔空间原点，设置的baseline
	int iCurOriginY;
	Region iRegion;		//lcd的区域，需要根据笛卡尔空间原点转换
	unsigned char * iBuffer;
	int iNextCurOriginX;//如果继续采用上述笛卡尔空间 原点，下一字符的原点
	int iNextCurOriginY;
}FontBitMap, *PFontBitMap;

typedef struct FontOpr{
	char * name;
	int (*FontInit)(char * CharSet);
	int (*FontDeinit)(void);
	int (*SetFontSize)(FontSize tSize);
	int (*GetFontBitMap)(unsigned int dwCode,PFontBitMap ptFontBitMap);
	int (*GetTextRegionCartesian)(char * name, PRegion tTextRegionCartesian);
	struct FontOpr * ptNext;
}FontOpr,* PFontOpr;

void RegisterFont(PFontOpr ptFontOpr);

int FontSysRegister(void);
int FontSelectAndInit(char * FontName, char * FontFile);
int FontSetSize(FontSize tSize);
int GetFontBitMap(unsigned int dwCode, PFontBitMap ptFontBitMap);
int GetTextRegionCartesian(char * name, PRegion ptTextRegionCartesian);


#endif //_FONT_MANAGER_H
