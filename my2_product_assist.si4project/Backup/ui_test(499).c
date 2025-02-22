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


static void DefaultOnDraw(void)
{
	printf("DefaultOnDraw on\n");
}

static void DefaultOnPressed(PInputEvent ptInputEvent)
{
	printf("DefaultOnPressed on\n");
}


int main(int argc, char **argv)
{

	PDispBuff ptBuffer;
	Button tButton;
	Region tRegion;
	PInputEvent ptInputEvent;
		
	DisplaySystemRegister();

	SelectDefaultDisplay("fb");

	InitDefaultDisplay();

	tRegion.iLeftUpX = 0;
	tRegion.iLeftUpY = 0;
	tRegion.iWidth = 800;
	tRegion.iHeight = 480;
	
	ButtonInit(&tButton,"test",&tRegion,DefaultOnDraw,DefaultOnPressed);

	tButton.ButtonDraw();
	tButton.ButtonPressed(ptInputEvent);
	
	


	return 0;	
}


