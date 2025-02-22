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

			/* 生成button */
			PButton ptButton = malloc(sizeof(Button));
			ptButton->Base.Name = ptConfigItem->name;
			ptButton->Base.type = BUTTON_TYPE;
			ptButton->Base.iRegion.iLeftUpX = startX;
			ptButton->Base.iRegion.iLeftUpY = startY;
			ptButton->Base.iRegion.iHeight = tButtonHeigh;
			ptButton->Base.iRegion.iWidth = tButtonWidth;
			/* 插入pages的button列表 */
			list_add(&ptButton->Base.module_list,&((&MainPage)->UI_Module_list));
//			printf("MainPage UI_Module_list add: %s\r\n",ptButton->Base.Name);
			iItem++;
		}
	}
}


void *start_sh(void * data)
{	
	char * commandpath = (char*)data;
	system(commandpath);
	return NULL;
}


void MainPageInit(void)
{
	PButton ptButton;
	int item_num;
	int i;
	PConfigItem ptConfigItem;
	int err;
	pthread_t tid;

	/* 初始化UI链表 */
	INIT_LIST_HEAD(&MainPage.UI_Module_list);
	/* 获取页面信息 */
	ParseConfigFile();
	/* 根据页面信息生成页面 */
	GenerateUIModulesWithConfigItems();
	
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


void MainPageRegister(void)
{
	PageRegister(&MainPage);
}



