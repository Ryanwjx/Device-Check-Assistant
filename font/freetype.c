#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include "font_manager.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H


static FT_Face g_face;
static int g_iDefaultFontSize = 12;

int FreeTypeFontInit(char * CharSet)
{	
	FT_Library library;
	int error;
    
    error = FT_Init_FreeType( &library );  
	if (error)
    {
        printf("FT_Init_FreeType error\n");
        return -1;
    }
		
    error = FT_New_Face( library, CharSet, 0, &g_face ); 	//打开字体文件argv[1]，写入face
	if (error)
    {
        printf("FT_New_Face error\n");
        return -1;
    }
		
    error = FT_Set_Pixel_Sizes(g_face, g_iDefaultFontSize, 0);

 	if (error)
    {
        printf("FT_Set_Pixel_Sizes error\n");
        return -1;
    }
		   
	return 0;
}
int FreeTypeFontDeinit(void)
{
	return 0;
}

int FreeTypeSetFontSize(FontSize tSize)
{
	int error;
	error = FT_Set_Pixel_Sizes(g_face, tSize, 0);
	if (error)
    {
        printf("FT_Set_Pixel_Sizes error\n");
        return -1;
    }
	return 0;
}

int FreeTypeGetFontBitMap(unsigned int dwCode,PFontBitMap ptFontBitMap)
{
	int error;
	FT_Vector pen;
	FT_GlyphSlot slot = g_face->glyph;

	pen.x = ptFontBitMap->iCurOriginX * 64; /* 单位: 1/64像素 */
    pen.y = ptFontBitMap->iCurOriginY * 64; /* 单位: 1/64像素 */

	FT_Set_Transform(g_face, 0, &pen);/* 转换：transformation */
		
	error = FT_Load_Char(g_face, dwCode, FT_LOAD_RENDER);
	if (error)
    {
        printf("FT_Load_Char error\n");
        return -1;
    }
	
	ptFontBitMap->iBuffer = slot->bitmap.buffer;
	ptFontBitMap->iRegion.iLeftUpX = slot->bitmap_left;
	ptFontBitMap->iRegion.iLeftUpY = ptFontBitMap->iCurOriginY - (slot->bitmap_top - ptFontBitMap->iCurOriginY);
	ptFontBitMap->iRegion.iWidth = slot->bitmap.width;
	ptFontBitMap->iRegion.iHeight = slot->bitmap.rows;

	ptFontBitMap->iNextCurOriginY = ptFontBitMap->iCurOriginY;
	ptFontBitMap->iNextCurOriginX = ptFontBitMap->iCurOriginX + slot->advance.x / 64;
	return 0;
}

/* freetype的bitmap变换为LCD下 */
//ptFontBitMap中设置iCurOriginX，iCurOriginY：作为笛卡尔坐标系下的原点，作为lcd下的基准线
//返回ptFontBitMap中根据lcd下的基准线设置tRegion的左上角点坐标
	//DrawBitMap可直接使用该函数返回值
	//DrawTextInRegionCentral需要知道lcd的基准线设置，如以lcd下str的左上角点获取bitmap整体会偏上；需要提前向下移动字符顶到基线距离
//static int FreeTypeGetFontBitMap___(unsigned int dwCode, PFontBitMap ptFontBitMap)
//{
//	int error;
//    FT_Vector pen;
//    FT_GlyphSlot slot = g_tFace->glyph;
	
//	//输入的ptFontBitMap设置了笛卡尔空间原点，也就确定了freetype基线
//    pen.x = ptFontBitMap->iCurOriginX * 64; /* 单位: 1/64像素 */
//    pen.y = ptFontBitMap->iCurOriginY * 64; /* 单位: 1/64像素 */
	
//	FT_Set_Transform(g_tFace, 0, &pen); 

//	//获取位图g_tFace
//	//会得到slot = g_tFace->glyph；slot->bitmap_left，slot->bitmap_top分别存储，生成位图左上角在笛卡尔空间的xy
//	error = FT_Load_Char(g_tFace, dwCode, FT_LOAD_RENDER);
//	if (error)
//	{
//		printf("FT_Load_Char error\n");
//		return -1;
//	}

//	ptFontBitMap->pucBuffer = slot->bitmap.buffer;
	
//	//lcd上也要在某线上对齐，y方向需要移动到同一baseline，以字符顶部为lcd_baseline，各bitmap只需向上移动：笛卡尔空间下bitmap上方到其baseline的距离
//	ptFontBitMap->tRegion.iLeftUpX = slot->bitmap_left;
//	ptFontBitMap->tRegion.iLeftUpY = ptFontBitMap->iCurOriginY - (slot->bitmap_top - ptFontBitMap->iCurOriginY);
//	//tRegion.iLeftUpX，tRegion.iLeftUpY是各bitmap在lcd下的左上xy，tRegion.iLeftUpX=笛卡尔原点x，tRegion.iLeftUpY=笛卡尔原点y-字符高度
//	//所以lcd上展示的原点值，与设置的笛卡尔坐标原点一致
//	//单个bitmap画出来需要考虑ptFontBitMap->iCurOriginY > 字符高度，否则bitmap起点为负值，显示不完整
	
//	ptFontBitMap->tRegion.iWidth   = slot->bitmap.width;
//	ptFontBitMap->tRegion.iHeigh   = slot->bitmap.rows;
	
//	ptFontBitMap->iNextOriginX = ptFontBitMap->iCurOriginX + slot->advance.x / 64;
//	ptFontBitMap->iNextOriginY = ptFontBitMap->iCurOriginY;

//	return 0;
//}

int FreeTypeGetTextRegionCartesian(char * name, PRegion ptTextRegionCartesian)
{
	int i=0;
	
	FT_Vector pen;
	FT_GlyphSlot slot = g_face->glyph;
	FT_Glyph  glyph;
	FT_BBox glyph_bbox;
	
	int xMin = 30000;
	int yMin = 30000;
	int xMax = -30000;
	int yMax = -30000;

	int error;

	pen.x = 0; /* 单位: 1/64像素 */
    pen.y = 0; /* 单位: 1/64像素 */
	
	while(name[i])
	{
		/* 转换：transformation */
		FT_Set_Transform(g_face, 0, &pen);

		/* 加载位图 */
		error = FT_Load_Char(g_face, name[i], FT_LOAD_RENDER);
        if (error)
        {
            printf("FT_Load_Char error\n");
            return -1;
        }

        /* 取出glyph */
        error = FT_Get_Glyph(g_face->glyph, &glyph);
        if (error)
        {
            printf("FT_Get_Glyph error!\n");
            return -1;
        }
        
        /* 从glyph得到外框: bbox */
        FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_TRUNCATE, &glyph_bbox);

		/* 更新方框 */
		if (glyph_bbox.xMin < xMin)
			xMin = glyph_bbox.xMin;
		if (glyph_bbox.yMin < yMin)
			yMin = glyph_bbox.yMin;
		if (glyph_bbox.xMax > xMax)
			xMax = glyph_bbox.xMax;
		if (glyph_bbox.yMax > yMax)
			yMax = glyph_bbox.yMax;

		/* 重设原点 */
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;

		i++;
	}
	
	ptTextRegionCartesian->iLeftUpX = xMin;
	ptTextRegionCartesian->iLeftUpY = yMax;
	ptTextRegionCartesian->iWidth 	= xMax - xMin + 1;
	ptTextRegionCartesian->iHeight 	= yMax - yMin + 1;

	//printf("%d %d %d %d",ptTextRegionCartesian->iLeftUpX,ptTextRegionCartesian->iLeftUpY,ptTextRegionCartesian->iWidth,ptTextRegionCartesian->iHeight);
	return 0;
}



static FontOpr FreeTypeFontOpr = {
	.name = "freetype",
	.FontInit = FreeTypeFontInit,
	.FontDeinit = FreeTypeFontDeinit,
	.SetFontSize = FreeTypeSetFontSize,
	.GetFontBitMap = FreeTypeGetFontBitMap,		
	.GetTextRegionCartesian = FreeTypeGetTextRegionCartesian,
};

void FreeTypeFontRegister(void)
{
	RegisterFont(&FreeTypeFontOpr);
}

