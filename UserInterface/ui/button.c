#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include "ui.h"
#include "stdio.h"
#include "display_manager.h"

void ButtonInit(PButton ptButton,char * name,PRegion ptRegion,
					void (*ButtonProcessInput)(PButton ptButton,int InputType))
{
	ptButton->Base.iRegion = *ptRegion;
	ptButton->Base.Name = name;
	ptButton->Base.type = BUTTON_TYPE;
	ptButton->Base.dirty = 1;

	ptButton->Status = BUTTON_STATUS_UNPRESSED;
	ptButton->color = MODULE_COLOR_RED;

 	ptButton->ButtonProcessInput = ButtonProcessInput;
}

void ButtonShow(PButton ptButton)
{
	DrawRegion(&ptButton->Base.iRegion, ptButton->color);
	DrawTextInRegionCentral(ptButton->Base.Name, &ptButton->Base.iRegion, MODULE_COLOR_BLACK);
	ptButton->Base.dirty = 0;
}


