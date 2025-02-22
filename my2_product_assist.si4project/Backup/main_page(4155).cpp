#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <math.h>


#include "page_manager.h"
#include "stdio.h"
#include "ui.h"
#include "config.h"
#include "display_manager.h"
#include <stdlib.h>
#include <pthread.h>


static Button g_Buttons[20];
static int g_ButtonsNum=0;

void *callsys(void * data)
{
	char command[100];
	char *input;
	input = (char *)data; 
	memcpy(command, input, strlen(input));
	printf("%s \r\n",command);
	system(command);
	return NULL;
}


void MainPageButtonPressed(PButton ptButton, PInputEvent ptInputEvent)
{
//	printf("%s is pressed!\n",ptButton->Name);
	PConfigItem ptConfigItem = GetItemConfigByName(ptButton->Name);
	char iCommand[100];
	int err;
	pthread_t tid;
	
	/* 触摸屏事件 */
	if (ptInputEvent->iType == INPUT_TYPE_TOUCH && ptInputEvent->iPressure)
	{
		/* 可被触摸 */
		if (!ptConfigItem->CanBeTouch)
			return;
		/* 更新画面 */
		if (ptButton->Status == 0)
		{
			DrawRegion( &ptButton->iRegion, 0x0000ff);
			ptButton->Status = 1;
		}
		else if (ptButton->Status == 1)
		{
			DrawRegion( &ptButton->iRegion, 0x00ff00);
			ptButton->Status = 2;
		}
		else if (ptButton->Status == 2)
		{
			DrawRegion( &ptButton->iRegion, 0xff0000);
			ptButton->Status = 0;
		}
		DrawTextInRegionCentral(ptButton->Name,  &ptButton->iRegion, 0x000000);
		FlushDisplayRegion(&ptButton->iRegion);
		
	}
	/* 网络事件 */
	else if (ptInputEvent->iType == INPUT_TYPE_NET)
	{
		DrawRegion( &ptButton->iRegion, 0x00ff00);
		ptButton->Status = 2;
		DrawTextInRegionCentral(ptButton->Name,  &ptButton->iRegion, 0x000000);
		FlushDisplayRegion(&ptButton->iRegion);
	}

	/* 执行程序 */
	if (ptConfigItem->CanBeTouch && ptConfigItem->CommandPath != '\0')
	{	
		if (ptButton->Status == 1)
			sprintf(iCommand,"%s %d\n",ptConfigItem->CommandPath,1);
		else if (ptButton->Status == 2)
			sprintf(iCommand,"%s %d\n",ptConfigItem->CommandPath,0);
		

		err = pthread_create(&tid, NULL,callsys,iCommand);
		if (err < 0)
		{
			printf("%s test not start\n",ptConfigItem->name);
		}
	}
	
}
	
void MainPageButtonDraw(PButton ptButton)
{
	DrawRegion(&ptButton->iRegion, 0xff0000);
	DrawTextInRegionCentral(ptButton->Name,&ptButton->iRegion, 0x000000);
	FlushDisplayRegion(&ptButton->iRegion);
}

void GenerateButtonsWithConfigItems(void)
{
	int item_num;
	PDispBuff ptDispBuff;
	int tDispWidth;
	int tDispHeigh;
	int tButtonWidth;
	int tButtonHeigh;
	int rows;
	int cols;

	int irow,icol;
	int iItem = 0;
	int startX=0;
	int startY=0;
	int xgap;
	int ygap;
	PConfigItem ptConfigItem;

	/* 计算Button的位置分配 */
	item_num =  GetItemConfigNum();
	g_ButtonsNum = item_num;
	ptDispBuff = GetDisplayBuffer();
	tDispWidth = ptDispBuff->iXres;
	tDispHeigh = ptDispBuff->iYres;
	tButtonWidth = sqrt(1.0/0.6 *tDispWidth * tDispHeigh / item_num);//屏幕为800*480，长宽比例0.6，平分面积
																	 //但如果不是n*n个，单个面积过大，宽度过长，行增多
	cols = tDispWidth/tButtonWidth + 1;
	tButtonWidth = tDispWidth / cols; 	//防止行多，多增加一列，重新计算宽度
	tButtonWidth -= 10;	//此时行上无空隙，加入固定空隙
	
	tButtonHeigh = tButtonWidth * 0.6;
	if (item_num % cols > 0)
		rows = item_num/cols + 1;
	else
		rows = item_num/cols;
	
	xgap = (tDispWidth - tButtonWidth * cols)/(cols+1);
	ygap = (tDispHeigh - tButtonHeigh * rows)/(rows+1);

	/* 生成各个button */
	for (irow =0 ; irow < rows && iItem < item_num; irow++)
	{
		startY = irow * (tButtonHeigh + ygap ) + ygap;
		for (icol = 0 ; icol < cols && iItem < item_num; icol++)
		{	
			startX = (icol) * (tButtonWidth + xgap) + xgap;
			ptConfigItem = GetItemConfigByIndex(iItem);
			
			g_Buttons[iItem].Name = ptConfigItem->name;
			g_Buttons[iItem].Status = 0;
			g_Buttons[iItem].iRegion.iLeftUpX = startX;
			g_Buttons[iItem].iRegion.iLeftUpY = startY;
			g_Buttons[iItem].iRegion.iWidth = tButtonWidth;
			g_Buttons[iItem].iRegion.iHeight = tButtonHeigh;
			g_Buttons[iItem].ButtonDraw = MainPageButtonDraw;
			g_Buttons[iItem].ButtonPressed = MainPageButtonPressed;

			iItem++;
		}
	}

	for (iItem = 0 ; iItem < g_ButtonsNum ; iItem++)
	{
		g_Buttons[iItem].ButtonDraw(&g_Buttons[iItem]);
	}
}

PButton GetButtonByInputEvent(PInputEvent ptInputEvent)
{
	int i;
	int xmin;
	int xmax;
	int ymin;
	int ymax;
	
	int inputx;
	int inputy;

	if (ptInputEvent->iType == INPUT_TYPE_TOUCH)
	{
		inputx = ptInputEvent->iX;
		inputy = ptInputEvent->iY;
		for (i = 0; i < g_ButtonsNum; i++)
		{
			xmin = g_Buttons[i].iRegion.iLeftUpX;
			ymin = g_Buttons[i].iRegion.iLeftUpY;
			xmax = xmin + g_Buttons[i].iRegion.iWidth;
			ymax = ymin + g_Buttons[i].iRegion.iHeight;

//			printf("xmin = %d,inputx = %d,xmax = %d\n",xmin,inputx,xmax);
//			printf("ymin = %d,inputy = %d,ymax = %d\n",ymin,inputy,ymax);
			if (inputx < xmax && inputx > xmin && inputy < ymax && inputy > ymin)
			{
				return &g_Buttons[i];
			}
		}

	}
	else if(ptInputEvent->iType == INPUT_TYPE_NET)
	{
		for (i = 0; i < g_ButtonsNum; i++)
		{
			if (!strcmp(ptInputEvent->str,g_Buttons[i].Name))
			{
				return &g_Buttons[i];
			}
		}
	}
	return NULL;
}

void *start_sh(void * data)
{	
	char * commandpath = (char*)data;
	system(commandpath);
	return NULL;
}


void MainPageInit(void)
{
	InputEvent tInputEvent;
	PButton ptButton;
	int item_num;
	int i;
	PConfigItem ptConfigItem;
	int err;
	pthread_t tid;
	
	/* 获取页面信息 */
	ParseConfigFile();
	/* 根据页面信息生成并显示页面按钮 */
	GenerateButtonsWithConfigItems();
	
	/* 自动执行不可触碰程序，创建线程 */
	item_num =  GetItemConfigNum();
	for (i = 0; i < item_num; i++)
	{
		/* 选出不可触碰且包含程序的器件 */
		ptConfigItem = GetItemConfigByIndex(i);
		if (ptConfigItem->CanBeTouch != 0 || ptConfigItem->CommandPath[0]=='\0')
			continue;
		/* 执行对应程序 */
		//printf("path = %s\n",ptConfigItem->CommandPath);
		err = pthread_create(&tid, NULL,start_sh,ptConfigItem->CommandPath);
		if (err < 0)
		{
			printf("%s test not start\n",ptConfigItem->name);
		}
	}
}

Page MainPage = {
	.PageName = "main",
	.PageInit = MainPageInit;
};

void MainPageRegister(void)
{
	PageRegister(&MainPage);
}



