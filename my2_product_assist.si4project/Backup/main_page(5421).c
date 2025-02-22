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

void MainPageInit(void);

Page MainPage = {
	.PageName = "main",
	.PageInit = MainPageInit,
};

void *start_sh(void * data)
{	
	char * commandpath = (char*)data;
	system(commandpath);
	return NULL;
}

void MainPageButtonPressed(PButton ptButton)
{
	PConfigItem ptConfigItem;
	int err;
	pthread_t tid;
	char command[100];

	if (ptButton->Status == BUTTON_STATUS_UNPRESSED)
	{
		ptConfigItem = GetItemConfigByName(ptButton->Base.Name);
		sprintf(command,"%s %s",ptConfigItem->CommandPath,"1");
		err = pthread_create(&tid, NULL,start_sh,command);
		if (err < 0)
		{
			printf("%s test not start\n",ptConfigItem->name);
		}
		ptButton->Status = BUTTON_STATUS_PRESSING;
		ptButton->color = MODULE_COLOR_BLUE;
	}
	else if (ptButton->Status == BUTTON_STATUS_PRESSING)
	{
		ptConfigItem = GetItemConfigByName(ptButton->Base.Name);
		sprintf(command,"%s %s",ptConfigItem->CommandPath,"0");
		err = pthread_create(&tid, NULL,start_sh,command);
		if (err < 0)
		{
			printf("%s test not start\n",ptConfigItem->name);
		}
		ptButton->Status = BUTTON_STATUS_PRESSED;
		ptButton->color = MODULE_COLOR_GREEN;
	}
	else if (ptButton->Status == BUTTON_STATUS_PRESSED)
	{
		ptButton->Status = BUTTON_STATUS_UNPRESSED;
		ptButton->color = MODULE_COLOR_RED;
	}
}

void MainPageLabelDefaultFunc(PLabel ptLabel)
{	
	int err;
	pthread_t tid;
	PConfigItem ptConfigItem;
	
	ptConfigItem = GetItemConfigByName(ptLabel->Base.Name);
	err = pthread_create(&tid, NULL,start_sh,ptConfigItem->CommandPath);
	if (err < 0)
	{
		printf("%s test not start\n",ptConfigItem->name);
	}
}

void MainPageLabelChange(PLabel ptLabel)
{
	ptLabel->color = MODULE_COLOR_GREEN;
}
	
void GenerateUIModulesWithConfigItems(void)
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

	Region iRegion;
	
	/* 计算Button的位置分配 */
	item_num =  GetItemConfigNum();
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
			iRegion.iLeftUpX = startX;
			iRegion.iLeftUpY = startY;
			iRegion.iHeight = tButtonHeigh;
			iRegion.iWidth = tButtonWidth;
			if (ptConfigItem->CanBeTouch == 1)
			{
				/* 生成button */
				PButton ptButton = malloc(sizeof(Button));
				ButtonInit(ptButton,ptConfigItem->name,&iRegion,MainPageButtonPressed);
				/* 插入pages的button列表 */
				list_add(&ptButton->Base.module_list,&((&MainPage)->UI_Module_list));
			}
			else if (ptConfigItem->CanBeTouch == 0)
			{
				/* 生成label */
				PLabel ptLabel = malloc(sizeof(Label));
				LabelInit(ptLabel,ptConfigItem->name,&iRegion,MainPageLabelDefaultFunc,MainPageLabelChange);
				/* 插入pages的button列表 */
				list_add(&ptLabel->Base.module_list,&((&MainPage)->UI_Module_list));
			}
//			printf("MainPage UI_Module_list add: %s\r\n",ptButton->Base.Name);
			iItem++;
		}
	}
}





void MainPageInit(void)
{
	PBaseModule ptBaseModule;
	PLabel ptLabel;
	
	/* 初始化UI链表 */
	INIT_LIST_HEAD(&MainPage.UI_Module_list);
	/* 获取页面信息 */
	ParseConfigFile();
	/* 根据页面信息生成页面 */
	GenerateUIModulesWithConfigItems();
	/* 自动执行Label的程序 */
	list_for_each_entry(ptBaseModule, &MainPage.UI_Module_list, module_list)
	{
		if (ptBaseModule->type == LABEL_TYPE)
		{
			ptLabel = (PLabel)ptBaseModule;
			ptLabel->LabelDefaultFunc(ptLabel);
		}
	}
}


void MainPageRegister(void)
{
	PageRegister(&MainPage);
}



