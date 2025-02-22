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

void ButtonInit(PButton ptButton,char * name,PRegion ptRegion,void (*ButtonPressed)(PButton ptButton))
{
	ptButton->Base.iRegion = *ptRegion;
	ptButton->Base.Name = name;
	ptButton->Base.type = BUTTON_TYPE;

	ptButton->Status = BUTTON_STATUS_UNPRESSED;
	ptButton->color = MODULE_COLOR_RED;

 	ptButton->ButtonPressed = ButtonPressed;
}

void ButtonShow(PButton ptButton)
{
	DrawRegion(&ptButton->Base.iRegion, ptButton->color);
}


