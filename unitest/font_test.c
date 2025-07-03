#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>


#include <stdlib.h>

#include <display_manager.h>
#include <font_manager.h>


int main(int argc, char **argv)
{

	PDispBuff ptBuffer;
	char * str = "wjx6666...55";
	FontBitMap tFontBitMap;
	int ret;
	int i=0;

	Region tRegion;

	if (argc != 5)
	{
		printf("Usage: %s <font_file> <lcd_x> <lcd_y> <font_size>\n", argv[0]);
		return -1;
	}

		
	DisplaySystemRegister();

	SelectDefaultDisplay("fb");

	InitDefaultDisplay();

	ret = FontSysRegister();
	if (ret<0)
	{
		printf("FontSeysRegister error\n");
		return -1;
	}
	
	ret = FontSelectAndInit("freetype", argv[1]);
	if (ret<0)
	{
		printf("FontSelectAndInit error\n");
		return -1;
	}
	ret = FontSetSize(strtol(argv[4], NULL, 0));
	if (ret<0)
	{
		printf("FontSetSize error\n");
		return -1;
	}

	tFontBitMap.iCurOriginX = strtol(argv[2], NULL, 0);
	tFontBitMap.iCurOriginY = strtol(argv[3], NULL, 0);
	while (str[i])
	{
		/* get bitmap */

		ret = GetFontBitMap(str[i], &tFontBitMap);
		printf("x,y,w,h = %d %d %d %d\n",tFontBitMap.iRegion.iLeftUpX,tFontBitMap.iRegion.iLeftUpY,
			tFontBitMap.iRegion.iWidth,tFontBitMap.iRegion.iHeight);
		if (ret)
		{
			printf("GetFontBitMap err\n");
			return -1;
		}

		/* draw on buffer */		
		DrawFontBitMap(&tFontBitMap, 0xff0000);

		/* flush to lcd/web */		
		ptBuffer = GetDisplayBuffer();

		tFontBitMap.iCurOriginX = tFontBitMap.iNextCurOriginX;
		tFontBitMap.iCurOriginY = tFontBitMap.iNextCurOriginY;	
		i++;
	}

	tRegion.iLeftUpX = 0;
	tRegion.iLeftUpY = 0;
	tRegion.iWidth = 800;
	tRegion.iHeight = 480;

	
	DrawTextInRegionCentral(str,&tRegion,0xff0000);
	
	return 0;	
}


