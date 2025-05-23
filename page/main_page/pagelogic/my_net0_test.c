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

#define SERVER_PORT 8888


int main(int argc, char *argv[])
{	
	FILE *fp;
	char result[1024];
	
	while (1) 
	{
		fp = popen("ping 192.168.0.198", "r");
	    if (!fp)
	    {
			printf("popen err\r\n");
			goto err;
		}
		
		while (fgets(result, sizeof(result), fp) != NULL)
		{
			if(strstr(result,"bytes from"))
			{
				printf("wifi data get ok\r\n");
	 			system("./page/main_page/pagelogic/net_tran_msg_test 127.0.0.1 net0");
				fclose(fp);
				return 0;
			}
		}

		err: sleep(1);
	}
	
	return 0;
}

