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

void PageSelect(char * PageName)
{	PPage ptPage;

	list_for_each_entry(ptPage, &g_PagesList, Page_list)
	{
		printf("want name=%s,get name=%s\r\n",PageName,ptPage->PageName);
		if (strcmp(PageName, ptPage->PageName) == 0)
		{
			g_CurrentPage = ptPage;
		}
	}
}

void PageInit(PPage ptPage)
{
	ptPage->PageInit();
	list_for_each_entry(ptBaseModule, &ptPage->UI_Module_list, module_list)
	{	
//		printf("Page UI_Module_list: %s\r\n",ptBaseModule->Name);
		if (ptBaseModule->type == BUTTON_TYPE)
		{
			ptButton = (PButton)ptBaseModule;
			printf("Module type: %d , Module Name: %s\r\n",ptButton->Base.type,ptButton->Base.Name);
		}
	}
}
void PageShow(PPage ptPage)
{	
	PBaseModule ptBaseModule;
	list_for_each_entry(ptBaseModule, &ptPage->UI_Module_list, module_list)
	{
		if (ptBaseModule->type == BUTTON_TYPE)
		{
			ptButton = (PButton)ptBaseModule;
			
		}
	}
}


void PageSystemRun(void)
{
	InputEvent tInputEvent;
	PBaseModule ptBaseModule;
	PButton ptButton;



	
	while(1)
	{
		/* 获取输入信息 */
		if (!GetInputEvent(&tInputEvent))
		{	
			/* 根据输入获取button */
			
			/* 根据按键信息执行操作 */
			
		}
	}
}
	


