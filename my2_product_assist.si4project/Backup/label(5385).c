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

void ButtonInit(PButton ptButton,char * name,PRegion ptRegion,void (*ButtonPressed)(void))
{
	ptButton->Base.iRegion = *ptRegion;
	ptButton->Base.Name = name;
	ptButton->Base.type = BUTTON_TYPE;

	ptButton->Status = BUTTON_STATUS_UNPRESSED;
	ptButton->color = BUTTON_COLOR_RED;

	ptButton->ptSignal0 = CreateSignal0Wrapper();
	ConnectSignal0SlotWrapper(ptButton->ptSignal0, ButtonPressed);
}

void ButtonRun(void * ptSignal0)
{
	EmitSignal0Wrapper(ptSignal0);
}

void ButtonShow(PButton ptButton)
{
	DrawRegion(&ptButton->Base.iRegion, ptButton->color);
}


