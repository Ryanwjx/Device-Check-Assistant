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
#include <ui.h>
#include "list.h"


static void DefaultOnPressed()
{
	printf("DefaultOnPressed on\n");
	
}

static void DefaultOnPressed2()
{
	printf("DefaultOnPressed2 on\n");
	
}


int main(int argc, char **argv)
{
	Button tButton;
	Region tRegion;

	Button tButton2;
	Region tRegion2;

	struct list_head uimodule_list;
	PBaseModule ptmodule;
	PButton ptbutton;
	
	INIT_LIST_HEAD(&uimodule_list);
	
	DisplaySystemRegister();

	SelectDefaultDisplay("fb");

	InitDefaultDisplay();

	
	tRegion.iLeftUpX = 0;
	tRegion.iLeftUpY = 0;
	tRegion.iWidth = 800;
	tRegion.iHeight = 480;
	
//	ButtonInit(&tButton,"test",&tRegion,DefaultOnDraw,DefaultOnPressed);
	ButtonInit(&tButton,"test",&tRegion,DefaultOnPressed);

	ButtonInit(&tButton2,"test2",&tRegion2,DefaultOnPressed2);

	list_add(&tButton.Base.module_list, &uimodule_list);
	list_add(&tButton2.Base.module_list, &uimodule_list);
	
	list_for_each_entry(ptmodule, &uimodule_list, module_list)
	{
		ptbutton = (PButton)ptmodule;
		printf("addr before = %d\r\n",(void *)&ptmodule);
		ButtonRun(ptbutton->ptSignal0);
		printf("name = %s\r\n",ptbutton->Base.Name);
		printf("addr after = %d\r\n",(void *)&ptmodule);
	}
		


	return 0;	
}


