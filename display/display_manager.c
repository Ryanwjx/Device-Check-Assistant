#include "display_manager.h"
#include "stdio.h"
#include "string.h"

static PDispOpr g_DispDevs; //作为环形链表头部
static PDispOpr g_DispDevDefault;
static DispBuff g_DispBuffer;



void RegisterDisplay(PDispOpr ptDispOpr)
{
	ptDispOpr->ptNext = g_DispDevs; //首次插入时，g_DisDevs==NULL
	g_DispDevs = ptDispOpr;	
}

void DisplaySystemRegister(void)
{
	extern PDispOpr FrameBufferRegister(void);
	RegisterDisplay(FrameBufferRegister());
}

int SelectDefaultDisplay(char * name)
{
	PDispOpr tmp = g_DispDevs;
	while(tmp)
	{
		if (strcmp(name,tmp->name) == 0)
		{
			g_DispDevDefault = tmp;
			return 0;
		}
		tmp = tmp->ptNext;
	}
	return -1;
}

int InitDefaultDisplay(void)
{
	int ret;
	
	ret = g_DispDevDefault->DeviceInit();
	if (ret < 0)
	{
		printf("default display init error\n");
		return -1;
	}

	ret = g_DispDevDefault->GetBuffer(&g_DispBuffer);
	if (ret < 0)
	{
		printf("get default buffer error\n");
		return -1;
	}
	return 0;
}

int PutPixel(int x, int y, unsigned int dwColor)
{
	//计算xy初始地址
	unsigned char *pen_8 = (unsigned char *)(g_DispBuffer.Buff + y*g_DispBuffer.iXres*g_DispBuffer.iBpp/8 + x*g_DispBuffer.iBpp/8);
	unsigned short *pen_16;	
	unsigned int *pen_32;	

	unsigned int red, green, blue;	

	pen_16 = (unsigned short *)pen_8;
	pen_32 = (unsigned int *)pen_8;

	switch (g_DispBuffer.iBpp)
	{
		case 8:
		{
			*pen_8 = dwColor;
			break;
		}
		case 16:
		{
			/* 565 */
			red   = (dwColor >> 16) & 0xff;
			green = (dwColor >> 8) & 0xff;
			blue  = (dwColor >> 0) & 0xff;
			dwColor = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
			*pen_16 = dwColor;
			break;
		}
		case 32:
		{
			*pen_32 = dwColor;
			break;
		}
		default:
		{
			printf("can't surport %dbpp\n", g_DispBuffer.iBpp);
			return -1;
			break;
		}
	}

	return 0;
}

int DrawFontBitMap(PFontBitMap ptFontBitMap, unsigned int dwColor)
{
	int x,y,ix,iy;
	int iLeftUpX = ptFontBitMap->iRegion.iLeftUpX;
	int iLeftUpY = ptFontBitMap->iRegion.iLeftUpY;
	int iRightDownX = iLeftUpX + ptFontBitMap->iRegion.iWidth;
	int iRightDowny = iLeftUpY + ptFontBitMap->iRegion.iHeight;
	int iWidth = ptFontBitMap->iRegion.iWidth;
	unsigned char * iFontBitMapBuff = ptFontBitMap->iBuffer;
	int ret = 0;
	
	for (x = iLeftUpX, ix = 0 ; x < iRightDownX; x++, ix++)
	{
		for (y = iLeftUpY, iy = 0 ; y < iRightDowny; y++, iy++)
		{
			if (x<0 || y<0 || x>g_DispBuffer.iXres || y>g_DispBuffer.iYres)
				continue;
		
			if(iFontBitMapBuff[iy * iWidth + ix] != 0)
			{
				ret = PutPixel(x,y, dwColor);
			}

			if (ret<0)
				return -1;
		}
	}
	return 0;
}


//void DrawTextInRegionCentral____(char *name, PRegion ptRegion, unsigned int dwColor)
//{
//	int n = strlen(name);
//	FontBitMap tFontBitMap;
//	RegionCartesian tRegionCar;

//	int iOriginX, iOriginY;
//	int i = 0;
//	int error;

//	/* 计算字符串的外框 */
//	GetStringRegionCar(name, &tRegionCar);

//	/* 算出第一个字符的origin */
//	iOriginX = ptRegion->iLeftUpX + (ptRegion->iWidth - tRegionCar.iWidth)/2 - tRegionCar.iLeftUpX; //tRegionCar.iLeftUpX 一般为 0 
//	iOriginY = ptRegion->iLeftUpY + (ptRegion->iHeigh - tRegionCar.iHeigh)/2 + tRegionCar.iLeftUpY; //tRegionCar.iLeftUpY 一般为 字体顶到baseline的差距最大
//																									//后续GetFontBitMap会根据输入的笛卡尔空间的原点，返回在lcd上也以该点作为轴线的bitmap
//																									//原x,y是想在lcd左上角开始显示，现要用GetFontBitMap返回的bitmap也在原x,y处开始显示
//																									//需要将原x,y转化为笛卡尔空间的原点，所以要向下 字体顶到baseline的差距

//	/* 逐个绘制 */
//	while (name[i])
//	{
//		/* get bitmap */
//		tFontBitMap.iCurOriginX = iOriginX;
//		tFontBitMap.iCurOriginY = iOriginY;
//		error = GetFontBitMap(name[i], &tFontBitMap);
//		if (error)
//		{
//			printf("SelectAndInitFont err\n");
//			return;
//		}

//		/* draw on buffer */		
//		DrawFontBitMap(&tFontBitMap, dwColor);		

//		iOriginX = tFontBitMap.iNextOriginX;
//		iOriginY = tFontBitMap.iNextOriginY;	
//		i++;
//	}
	
//}

void DrawTextInRegionCentral(char *name, PRegion ptRegion, unsigned int dwColor)
{
	FontBitMap tFontBitMap;
	int i=0;
	Region tTextRegionCartesian;
	int ret;
		
	ret = GetTextRegionCartesian(name, &tTextRegionCartesian);
	if (ret < 0)
	{
		printf("GetTextRegionCartesian error\n");
		return;
	}
	
	tFontBitMap.iCurOriginX = ptRegion->iLeftUpX + (ptRegion->iWidth - tTextRegionCartesian.iWidth)/2;
	tFontBitMap.iCurOriginY = ptRegion->iLeftUpY + (ptRegion->iHeight - tTextRegionCartesian.iHeight )/2 + tTextRegionCartesian.iLeftUpY;

	while (name[i])
	{
		ret = GetFontBitMap(name[i], &tFontBitMap);
		if (ret < 0)
		{
			printf("GetFontBitMap error\n");
			return;
		}
		DrawFontBitMap(&tFontBitMap, dwColor);
		if (ret < 0)
		{
			printf("DrawFontBitMap error\n");
			return;
		}
				
		tFontBitMap.iCurOriginX = tFontBitMap.iNextCurOriginX;
		tFontBitMap.iCurOriginY = tFontBitMap.iNextCurOriginY;

		i++;
	}
}

void DrawRegion(PRegion ptRegion, unsigned int dwColor)
{
	int x = ptRegion->iLeftUpX;
	int y = ptRegion->iLeftUpY;
	int width = ptRegion->iWidth;
	int heigh = ptRegion->iHeight;

	int i,j;
	for (j = y; j < y + heigh; j++)
	{
		for (i = x; i < x + width; i++)
			PutPixel(i, j, dwColor);
	}
}

int FlushDisplayRegion(PRegion ptRegion)
{
	return g_DispDevDefault->FlushRegion(ptRegion);
}

PDispBuff GetDisplayBuffer(void)
{
	return &g_DispBuffer;
}


