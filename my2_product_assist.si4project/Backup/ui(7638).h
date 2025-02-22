#ifndef _UI_H
#define _UI_H

#include "typedef.h"
#include "list.h"
#include "Delegate.h"
#include "Signal.h"

#define BUTTON_TYPE 0

#define BUTTON_STATUS_PRESSED 0
#define BUTTON_STATUS_UNPRESSED 1

#define BUTTON_COLOR_RED 0
#define BUTTON_COLOR_GREEN 1


typedef struct BaseModule{
	char * Name;
	char * type;
	Region iRegion;
	struct list_head;
}BaseModule,* PBaseModule;

typedef struct Button
{	
	BaseModule Base;
	int Status;
	unsigned int color;
	Signal0< void > update0;
	void (*ButtonPressed)(void);
}Button, *PButton;

void ButtonInit(PButton ptButton,char * name,PRegion ptRegion,void (*ButtonPressed)(void));

#endif //_UI_H
