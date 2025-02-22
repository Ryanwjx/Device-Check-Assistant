#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#include <sys/time.h>

#ifndef NULL
	#define NULL (void *)0
#endif 

#define INPUT_TYPE_TOUCH 1
#define INPUT_TYPE_NET 2

#define TOUCH_BEGIN 11
#define TOUCH_END 12

typedef struct InputEventBase{
	struct timeval iTime;
	int InputType;
}InputEventBase, * PInputEventBase;

typedef struct InputEventTouch{
	InputEventBase iInputEventBase;
	int iX;
	int iY;
	int iTouchType;
}InputEventTouch, * PInputEventTouch;

typedef struct InputEventNet{
	char str[1024];
}InputEventNet, * PInputEventNet;

typedef struct InputDevice{
	char * name;
	int (*GetInputEvent)(PInputEventBase ptInputEventBase);
	int (*DeviceInit)(void);
	int (*DeviceExit)(void);
	struct list_head InputDeviceList;
}InputDevice, *PInputDevice;

void RegisterInputDevice(PInputDevice           ptInputDev);
int InputSystemRegister(void);
int InputDeviceInit(void);
int InputDeviceExit(void);
int GetInputEvent(PInputEventBase ptInputEventBase);

#endif //_INPUT_MANAGER_H
