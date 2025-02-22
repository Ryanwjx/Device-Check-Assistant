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
	PBaseModule ptBaseModule;
	
	ptPage->PageInit();
	
	list_for_each_entry(ptBaseModule, &ptPage->UI_Module_list, module_list)
	{	
		printf("Module type: %d , Module Name: %s\r\n",ptBaseModule->type,ptBaseModule->Name);
	}
}

void PageShow(PPage ptPage)
{	
	PBaseModule ptBaseModule;
	PButton ptButton;
	PLabel ptLabel;
	
	list_for_each_entry(ptBaseModule, &ptPage->UI_Module_list, module_list)
	{
		if (ptBaseModule->type == BUTTON_TYPE)
		{
			ptButton = (PButton)ptBaseModule;
			ButtonShow(ptButton);
		}
		else if (ptBaseModule->type == LABEL_TYPE)
		{
			ptLabel = (PLabel)ptBaseModule;
			LabelShow(ptLabel);
		}
	}
}

int IfInputInRegion(int x, int y,PRegion ptRegion)
{
	int xul = ptRegion->iLeftUpX;
	int yul = ptRegion->iLeftUpY;
	int xdr = ptRegion->iLeftUpX + ptRegion->iWidth;
	int ydr = ptRegion->iLeftUpY + ptRegion->iHeight;

	if (x >= xul && x<=xdr && y >= yul && y<=ydr)
		return 1;
	return 0;
}

PBaseModule GetBaseModuleAccordInput(PInputEvent ptInputEvent)
{
	PBaseModule ptBaseModule;
	int InRegionFlag;
	
	if (ptInputEvent->iType == INPUT_TYPE_TOUCH)
	{
		list_for_each_entry(ptBaseModule, &g_CurrentPage->UI_Module_list, module_list)		
		{
			InRegionFlag = IfInputInRegion(ptInputEvent->iX, ptInputEvent->iY,&ptBaseModule->iRegion);
			if (InRegionFlag)
				return ptBaseModule;
		}
	}
	else if (ptInputEvent->iType == INPUT_TYPE_NET)
	{
		list_for_each_entry(ptBaseModule, &g_CurrentPage->UI_Module_list, module_list)		
		{
			if (strcmp(ptBaseModule->Name, ptInputEvent->str) == 0)
				return ptBaseModule;
		}
	}
	return NULL;
}


void PageSystemRun(void)
{
	InputEvent tInputEvent;
	PBaseModule ptBaseModule;
	PButton ptButton;
	PLabel ptLabel;

	while(1)
	{
		/* 获取输入信息 */
		if (!GetInputEvent(&tInputEvent))
		{	
			/* 根据输入获取UI MODULE */
			ptBaseModule = GetBaseModuleAccordInput(&tInputEvent);
			if (ptBaseModule == NULL)
				continue;
			
			/* 转换为对应模块并执行函数，修改数据 */
			if (ptBaseModule->type == BUTTON_TYPE)
			{
				ptButton = (PButton)ptBaseModule;
				ptButton->ButtonProcessInput(ptButton,tInputEvent.iType);
			}
			else if (ptBaseModule->type == LABEL_TYPE)
			{
				ptLabel = (PLabel)ptBaseModule;
				ptLabel->LabelProcessInput(ptLabel,tInputEvent.iType);
			}	
			/* 进行页面刷新 */
			PageShow(g_CurrentPage);
		}
	}
}
	


