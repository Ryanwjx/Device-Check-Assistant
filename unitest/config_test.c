#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>


#include <stdlib.h>

#include <config.h>

int main(int argc, char **argv)
{
	int ConfigItemNum = 0;
	int i = 0;
	PConfigItem ptConfigItem;
	
	if (argc < 2)
	{
		printf("usage %s <config file>\n",argv[0]);
		return -1;
	}
	
	ParseConfigFile(argv[1]);
	
	ConfigItemNum = GetItemConfigNum();

	for (i = 0; i < ConfigItemNum; i++)
	{
		ptConfigItem = GetItemConfigByIndex(i);
		printf("name = %s, canbetouch = %d, path = %s\n",ptConfigItem->name,ptConfigItem->CanBeTouch,ptConfigItem->CommandPath);
	}

	return 0;	
}


