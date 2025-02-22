#ifndef _UI_H
#define _UI_H

#include "typedef.h"
#include "list.h"
#include "SignalSlotCreateWrapper.h"

#define BUTTON_TYPE 0
#define LABEL_TYPE 1

#define BUTTON_STATUS_PRESSED 0
#define BUTTON_STATUS_UNPRESSED 1
#define BUTTON_STATUS_PRESSING 2

#define MODULE_COLOR_RED 0xff0000
#define MODULE_COLOR_GREEN 0x00ff00
#define MODULE_COLOR_BLUE 0x0000ff
#define MODULE_COLOR_BLACK 0x000000



typedef struct BaseModule{
	char * Name;
	char type;
	Region iRegion;
	struct list_head module_list;
}BaseModule,* PBaseModule;

typedef struct Button
{	
	BaseModule Base;
	int Status;
	unsigned int color;
	void (*ButtonProcessInput)(struct Button * ptButton,int InputType);
}Button, *PButton;

void ButtonInit(PButton ptButton,char * name,PRegion ptRegion,void (*ButtonProcessInput)(PButton ptButton,int InputType));
void ButtonShow(PButton ptButton);

typedef struct Label
{	
	BaseModule Base;
	unsigned int color;
	void (*LabelDefaultFunc)(struct Label * ptLabel);
	void (*LabelProcessInput)(struct Label * ptLabel,int InputType);
}Label, *PLabel;
void LabelInit(PLabel ptLabel,char * name,PRegion ptRegion,void (*LabelDefaultFunc)(PLabel ptLabel)
					,void (*LabelProcessInput)(PLabel ptLabel,int InputType));
void LabelShow(PLabel ptLabel);


#endif //_UI_H
