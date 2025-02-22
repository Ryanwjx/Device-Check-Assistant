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

//void ButtonInit(PButton ptButton, char * name, PRegion ptRegion,
//				void (*ptButtonDraw)(PButton ptButton), 
//				void (*ptButtonPressed)(PButton ptButton,PInputEvent ptInputEvent))
//{
//	ptButton->Name = name;
//	ptButton->iRegion = *ptRegion;
//	ptButton->ButtonDraw = ptButtonDraw;
//	ptButton->ButtonPressed = ptButtonPressed;
//}

void ButtonInit(PButton ptButton,char * name,PRegion ptRegion,void (*ButtonPressed)(void))
{
	ptButton->Base.iRegion = *ptButton;
	ptButton->Base.Name = name;
	ptButton->Base.type = BUTTON_TYPE;

	ptButton->Status = BUTTON_STATUS_UNPRESSED;
	ptButton->color = BUTTON_COLOR_RED;

	ptButton->update0
}




