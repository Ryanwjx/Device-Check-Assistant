#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include "input_manager.h"
#include <tslib.h>
#include "stdio.h"

static struct tsdev * g_ts;

int TSDeviceInit(void)
{
	g_ts = ts_setup(NULL,0);
	if (!g_ts)
	{
		printf("TSDeviceInit error\n");
		return -1;
	}
	
	return 0;
}

int TSDeviceExit(void)
{
	ts_close(g_ts);
	return 0;
}

int TSGetInputEvent(PInputEvent ptInputEvent)
{
	int ret;
	struct ts_sample samp;
	
	ret = ts_read(g_ts,&samp, 1);

	if (ret != 1)
	{
		return -1;
	}
	
	ptInputEvent->iPressure = samp.pressure;
	ptInputEvent->iX		= samp.x;
	ptInputEvent->iY		= samp.y;
	ptInputEvent->iType		= INPUT_TYPE_TOUCH;
	ptInputEvent->iTime		= samp.tv;

	printf("TS.pressure = %d\n",ptInputEvent->iPressure);
	return 0;
}


static InputDevice g_TouchScreen = {
	.name 			= "touchscreen",
	.GetInputEvent 	= TSGetInputEvent,
	.DeviceInit		= TSDeviceInit,
	.DeviceExit		= TSDeviceExit,
};

void TSRegisterInputDevice(void)
{
	RegisterInputDevice(&g_TouchScreen);
}


