#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "sys/ioctl.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"
#include <poll.h>
#include <sys/select.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>


int main(int argc, char *argv[])
{	
    int status;

	FILE *fp;
		
	while (1) 
	{
		/* 执行Ping，并获取fd */
		fp = popen("dmesg", "r");
		if (!fp) 
		{
			fprintf(stderr, "popen error: %s\n", gai_strerror(status));
	        goto err; 
		}
		
		/* 根据fd进行信息读取与判断 */
		char result[1024];
		while (fgets(result, sizeof(result), fp) != NULL)
		{
			if(!strstr(result,"g_mass_storage gadget: g_mass_storage ready"))
				continue;
			printf("otg_device data get ok\r\n");
			system("./CoreLogic/net_tran_msg_test 127.0.0.1 otg_device");
			fclose(fp);
			return 0;
		}

		err: sleep(1);
    }
	return 0;
}

