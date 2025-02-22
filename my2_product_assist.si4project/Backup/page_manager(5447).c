#include "page_manager.h"
#include "stdio.h"
#include "string.h"

static PPage g_ptPage;


void PageRegister(PPage ptPage)
{

}

void PageSystemRegister(void)
{

}

void PageSelect(char * PageName,PPage ptTargetPage)
{

}

void PageSystemRun(void)
{
	InputEvent tInputEvent;
	
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
	


