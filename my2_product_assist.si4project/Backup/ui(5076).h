#ifndef _UI_H
#define _UI_H

#include "typedef.h"
#include "input_manager.h"

typedef struct Button
{
	char * Name;
	int Status;
	Region iRegion;
	void (*ButtonDraw)(struct Button * ptButton);
	void (*ButtonPressed)(struct Button * ptButton,PInputEvent ptInputEvent);
}Button, *PButton;

void ButtonInit(PButton ptButton, char * name, PRegion ptRegion,
					void (*ptButtonDraw)(PButton ptButton), void (*ptButtonPressed)(PButton ptButton,PInputEvent ptInputEvent));
#endif //_UI_H
