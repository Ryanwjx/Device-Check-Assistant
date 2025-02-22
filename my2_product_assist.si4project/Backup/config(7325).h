#ifndef _CONFIG_H
#define _CONFIG_H

#include "typedef.h"

#define Config_File_Path "etc/product_assistant/page_info.cfg"
typedef struct ConfigItem{
	char name[100];
	int Index;
	int CanBeTouch;
	char CommandPath[100];
}ConfigItem, * PConfigItem;

int ParseConfigFile();
PConfigItem GetItemConfigByName(char * Name);
int GetItemConfigNum(void);
PConfigItem GetItemConfigByIndex(int Index);


#endif //_CONFIG_H
