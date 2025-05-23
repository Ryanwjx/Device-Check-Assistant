#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include "stdio.h"
#include "config.h"

ConfigItem g_ConfigItems[20];
int g_ConfigItemNum = 0;

int ParseConfigFile()
{	
	FILE * fp;
	char buf[100];
	char *p = buf;

	fp = fopen(Config_File_Path, "r");
	if (fp == NULL)
	{
		printf("can not open cfg file %s\n", Config_File_Path);
		return -1;
	}
	//printf("opened cfg file %s\n", Config_File_Path);
	while(fgets(buf, 100, fp))//读取一行字符串到buf,最多读取100。如果一行多余100，自动加'\0'；否则无，但包含'\n'
	{	
		//printf("get buf %s\n", buf);
		buf[99] = '\0';	//对于少于100的需要主动加0，成为str
	
		p = buf;

		while(*p == ' ' || *p == '\t')
			p++;

		if (*p == '#')
			continue;

		g_ConfigItems[g_ConfigItemNum].CommandPath[0] = '\0';
		g_ConfigItems[g_ConfigItemNum].Index = g_ConfigItemNum;
		sscanf(p,"%s %d %[^\n]",g_ConfigItems[g_ConfigItemNum].name,&g_ConfigItems[g_ConfigItemNum].CanBeTouch,g_ConfigItems[g_ConfigItemNum].CommandPath);

		//printf("get command Index: %d\n",g_ConfigItems[g_ConfigItemNum].Index);
		//printf("get command name: %s\n",g_ConfigItems[g_ConfigItemNum].name);
		//printf("get command CanBeTouch: %d\n",g_ConfigItems[g_ConfigItemNum].CanBeTouch);
		//printf("get command path: %s\n",g_ConfigItems[g_ConfigItemNum].CommandPath);

		g_ConfigItemNum++;
	}
	
	return 0;
}
PConfigItem GetItemConfigByName(char * Name)
{
	int i;

	for (i = 0; i < g_ConfigItemNum ; i++)
	{
		if (strcmp(Name,g_ConfigItems[i].name) == 0)
			return &g_ConfigItems[i];
	}

	return NULL;
}

int GetItemConfigNum(void)
{
	return g_ConfigItemNum;
}

PConfigItem GetItemConfigByIndex(int Index)
{
	int i;

	for (i = 0; i < g_ConfigItemNum ; i++)
	{
		if (Index == g_ConfigItems[i].Index)
			return &g_ConfigItems[i];
	}

	return NULL;
}




