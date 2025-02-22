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



int main(int argc, char *argv[])
{
	int fd;
	char key_flag=0;
	int ret = 0;
			
	while (1) {
		fd = open("/dev/my_key0", O_RDWR);
		if(fd < 0) {
			printf("can't open file %s\r\n", "/dev/my_key0");
			goto err;
		}

		while (1)
		{
			ret = read(fd, &key_flag, sizeof(key_flag));
			if (ret < 0)
			{
				perror("read key err\r\n");
				goto errin;
			}
			
			if( key_flag == 1 ) { 			/* 数据读取成功 */
				printf("key data get ok\r\n");
				system("./CoreLogic/net_tran_msg_test 127.0.0.1 key");
				close(fd);	/* 关闭文件 */	
				return 0;
			}
			errin: sleep(1);
		}
		err: sleep(1);
	}
	
	return 0;
}

