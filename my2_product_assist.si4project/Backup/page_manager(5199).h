#ifndef _PAGE_MANAGER_H
#define _PAGE_MANAGER_H

#include "list.h"

typedef struct{
	char * PageName;
	void (*PageInit)(void);
	struct list_head UI_Module_list;
	struct list_head Page_list;
} Page, * PPage;

void PageRegister(PPage ptPage);

void PageSystemRegister(void);

void PageSelect(char * PageName);
void PageInit(PPage ptPage);
void PageShow(PPage ptPage);
void PageSystemRun(void);


#endif //_PAGE_MANAGER_H
