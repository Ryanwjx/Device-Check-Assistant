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

#define SERVER_PORT 8888


int main(int argc, char *argv[])
{
	int fd;
	unsigned short databuf[100];
	char command[100];
	int ret = 0;

	if (argc != 4)
	{
		printf("Usage:%s <%s> <%s> <%s>\r\n", argv[0],"dev","option","key");
		return -1;
	}
		
	while (1) {
		fd = open(argv[1], O_RDWR);
		if(fd < 0) {
			printf("can't open file %s\r\n", argv[1]);
			goto err;
		}

		while (1)
		{
			ret = read(fd, databuf, sizeof(databuf));
			if(ret == atoi(argv[2])) { 			/* 数据读取成功 */
				printf("%s data get ok\r\n",argv[1]);
				sprintf(command,"./CoreLogic/net_tran_msg_test 127.0.0.1 %s",argv[3]);
				system(command);
				close(fd);	/* 关闭文件 */	
				return 0;
			}

			sleep(1);
		}
		err: sleep(1);
	}
	
	return 0;
}

