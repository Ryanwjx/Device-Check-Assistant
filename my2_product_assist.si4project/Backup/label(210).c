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

void LabelInit(PLabel ptLabel,char * name,PRegion ptRegion,void (*LabelDefaultFunc)(PLabel ptLabel)
					,void (*LabelProcessInput)(PLabel ptLabel,int InputType))
{
	ptLabel->Base.iRegion = *ptRegion;
	ptLabel->Base.Name = name;
	ptLabel->Base.type = LABEL_TYPE;
	
	ptLabel->color = MODULE_COLOR_RED;
	ptLabel->LabelDefaultFunc = LabelDefaultFunc;
	ptLabel->LabelProcessInput = LabelProcessInput;
}

void LabelShow(PLabel ptLabel)
{
	DrawRegion(&ptLabel->Base.iRegion, ptLabel->color);
	DrawTextInRegionCentral(ptLabel->Base.Name, &ptLabel->Base.iRegion, MODULE_COLOR_BLACK);
}


