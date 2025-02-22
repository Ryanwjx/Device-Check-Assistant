#include "font_manager.h"
#include "stdio.h"
#include "string.h"

PFontOpr g_FontDevs;
PFontOpr g_FontDevSelect;

void RegisterFont(PFontOpr ptFontOpr)
{
	ptFontOpr->ptNext = g_FontDevs;
	g_FontDevs = ptFontOpr;
}


int FontSysRegister(void)
{
	extern void FreeTypeFontRegister(void);
	FreeTypeFontRegister();
	
	return 0;
}

int FontSelectAndInit(char * FontName, char * FontFile)
{
	PFontOpr tmp = g_FontDevs;

	while(tmp)
	{
		if (strcmp(FontName,tmp->name) == 0)
		{
			g_FontDevSelect = tmp;
			return tmp->FontInit(FontFile);
		}
		tmp = tmp->ptNext;
	}
	return -1;
}

int FontSetSize(FontSize tSize)
{
	int ret;
	ret = g_FontDevSelect->SetFontSize(tSize);
	if (ret != 0)
		return -1;
	return 0;
}

int GetFontBitMap(unsigned int dwCode, PFontBitMap ptFontBitMap)
{
	int ret;
	ret = g_FontDevSelect->GetFontBitMap(dwCode,ptFontBitMap);
	if (ret != 0)
		return -1;
	return 0;
}

int GetTextRegionCartesian(char * name, PRegion ptTextRegionCartesian)
{
	return g_FontDevSelect->GetTextRegionCartesian(name,ptTextRegionCartesian);
}




