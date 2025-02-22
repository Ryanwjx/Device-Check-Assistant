#include "page_manager.h"
#include "stdio.h"
#include "string.h"

static PPageOpr g_ptPageOpr;


void PageRegister(PPageOpr ptPageOpr)
{
	ptPageOpr->ptNext = g_ptPageOpr;
	g_ptPageOpr = ptPageOpr;
}

void PageSystemRegister(void)
{
	extern void MainPageRegister(void);
	MainPageRegister();
}

int PageSelectAndRun(char * PageName)
{
	PPageOpr ptTmp = g_ptPageOpr;

	while(ptTmp)
	{
		if (strcmp(ptTmp->name, PageName) == 0)
		{
			ptTmp->Run();
			return 0;
		}
		ptTmp = ptTmp->ptNext;
	}
	return -1;
}


/* 循环检测触碰输入事件 */
	while(1)
	{
		/* 获取输入信息 */
		if (!GetInputEvent(&tInputEvent))
		{	
			/* 根据输入获取button */
			ptButton  = GetButtonByInputEvent(&tInputEvent);
			/* 根据按键信息执行操作 */
			if (ptButton)
				ptButton->ButtonPressed(ptButton,&tInputEvent);
		}
	}


