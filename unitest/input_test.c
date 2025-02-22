#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include <input_manager.h>

int main(int argc, char **argv)
{
	InputEvent Event;

	InputSystemRegister();
	printf("InputSystemRegister ok\n");
	InputDeviceInit();
	printf("InputDeviceInit ok\n");
	while(1)
	{
		if (!GetInputEvent(&Event))
		{
			printf("Time=%d.%d s\n"	,(int)(Event.iTime.tv_sec),(int)(Event.iTime.tv_usec/1000000));	
			printf("Type=%d\n"		,Event.iType);	
			printf("x=%d\n"			,Event.iX);	
			printf("y=%d\n"			,Event.iY);	
			printf("Pressure=%d\n"	,Event.iPressure);	
			printf("---------------\n");	
		}
		
	}
	
	return 0;	
}


