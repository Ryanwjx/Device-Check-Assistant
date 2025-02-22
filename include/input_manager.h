#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#include <sys/time.h>
#include "list.h"

#ifndef NULL
	#define NULL (void *)0
#endif 

#define INPUT_TYPE_TOUCH 1
#define INPUT_TYPE_NET 2

#define TOUCH_START 11
#define TOUCH_END 12

#define NET_INFO 21

typedef struct InputEventBase{
	struct timeval iTime;
	int InputType;
}InputEventBase, * ptInputEventBase;

typedef struct InputEvent{
	InputEventBase iInputEventBase;
	int iType;
	int iX;
	int iY;
	char str[1024];
}InputEvent, * PInputEvent;

typedef struct InputDevice{
	char * name;
	int (*GetInputEvent)(PInputEvent ptInputEvent);
	int (*DeviceInit)(void);
	int (*DeviceExit)(void);
	struct list_head InputDeviceList;
}InputDevice, *PInputDevice;

void RegisterInputDevice(PInputDevice       ptInputDev);
int InputSystemRegister(void);
int InputDeviceInit(void);
int InputDeviceExit(void);
int GetInputEvent(PInputEvent ptInputEvent);

#endif //_INPUT_MANAGER_H
