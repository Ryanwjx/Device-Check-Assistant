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

#include "display_manager.h"
#include "font_manager.h"
#include "input_manager.h"
#include "page_manager.h"

int main(int argc, char **argv)
{		
	int ret;
	PPage ptPage;
	
	if (argc != 2)
	{
		printf("Usage: %s <font_file>\n", argv[0]);
		return -1;
	}
	/* 关闭原ui程序，清除画面 */
//	sprintf(buf,"")
//	system(buf);
	
	/* 初始化显示系统 */
	DisplaySystemRegister();

	ret = SelectDefaultDisplay("fb");
	if (ret)
	{
		printf("SelectDefaultDisplay err\n");
		return -1;
	}
	ret = InitDefaultDisplay();
	if (ret)
	{
		printf("InitDefaultDisplay err\n");
		return -1;
	}
	/* 初始化文字系统 */
	FontSysRegister();

	FontSelectAndInit("freetype", argv[1]);

	FontSetSize(20);
	/* 初始化输入系统 */
	InputSystemRegister();

	InputDeviceInit();
	/* 初始化页面系统 */
	PageSystemRegister();

	ptPage = PageSelect("main");	/* 设置当前页面 */

	PageInit(ptPage);	/* 初始化页面结构 */

	PageShow(ptPage);	/* 页面显示 */
	
	/* 主线程进入页面更新模式 */
	printf("init ok\n");
	PageSystemRun();
	
	return 0;	
}


