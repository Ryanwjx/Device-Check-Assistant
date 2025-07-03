#include "page_manager.h"
#include "input_manager.h"
#include "stdio.h"
#include "string.h"
#include "ui.h"

static struct list_head g_PagesList;
static PPage g_CurrentPage;

void PageRegister(PPage ptPage)
{
	list_add(&ptPage->Page_list, &g_PagesList);
	printf("add page named : %s\r\n",ptPage->PageName);
	list_for_each_entry(ptPage, &g_PagesList, Page_list)
	{
		printf("now all name=%s\r\n",ptPage->PageName);
	}
}

void PageSystemRegister(void)
{
	INIT_LIST_HEAD(&g_PagesList);

	extern void MainPageRegister(void);
	MainPageRegister();
}

PPage PageSelect(char * PageName)
{	
	PPage ptPage;

	list_for_each_entry(ptPage, &g_PagesList, Page_list)
	{
		printf("want name=%s,get name=%s\r\n",PageName,ptPage->PageName);
		if (strcmp(PageName, ptPage->PageName) == 0)
		{
			g_CurrentPage = ptPage;
		}
	}

	return g_CurrentPage;
}

void PageInit(PPage ptPage)
{	
	// PBaseModule ptBaseModule;
	
	ptPage->PageInit();
	
	// list_for_each_entry(ptBaseModule, &ptPage->UI_Module_list, module_list)
	// {	
	// 	printf("Module type: %d , Module Name: %s\r\n",ptBaseModule->type,ptBaseModule->Name);
	// }
}

void PageShow(PPage ptPage)
{	
	ptPage->PageShow();
}

void PageSystemRun(void)
{
	InputEvent tInputEvent;
	while(1)
	{
		/* 获取输入信息 */
		if (!GetInputEvent(&tInputEvent))
		{	
			printf("get input event type = %d\r\n",tInputEvent.iType);
			/* 事件处理 */
			g_CurrentPage->PageEventProcess(&tInputEvent);
			/* 进行页面刷新 */
			g_CurrentPage->PageShow();
		}
	}
}
	


