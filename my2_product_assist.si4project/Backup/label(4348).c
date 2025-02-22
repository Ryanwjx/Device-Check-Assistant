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

void LabelInit(PLabel ptLabel,char * name,PRegion ptRegion,void (*LabelDefaultFunc)(void)
					,void (*LabelDisplayFunc)(void))
{
	ptLabel->Base.iRegion = *ptRegion;
	ptLabel->Base.Name = name;
	ptLabel->Base.type = LABEL_TYPE;
	
	ptLabel->color = MODULE_COLOR_RED;
	ptLabel->ptSignal0_default = CreateSignal0Wrapper();
	ptLabel->ptSignal0_display = CreateSignal0Wrapper();
	ConnectSignal0SlotWrapper(ptLabel->ptSignal0_default, LabelDefaultFunc);
	ConnectSignal0SlotWrapper(ptLabel->ptSignal0_display, LabelDisplayFunc);
}


