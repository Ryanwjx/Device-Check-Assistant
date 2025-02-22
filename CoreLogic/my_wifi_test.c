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
	struct addrinfo hints, *res;
    int status;

	FILE *fp;
	char command[100];

	memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;  // IPv4
    hints.ai_socktype = SOCK_STREAM;
		
	while (1) 
	{
		/* 解析域名 */
		status = getaddrinfo("www.baidu.com", NULL, &hints, &res);
	    if (status != 0) {
	        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
	        goto err;  // 无法解析域名
	    }

		/* 执行Ping，并获取fd */
		sprintf(command,"ping %s",inet_ntoa(((struct sockaddr_in*)(res->ai_addr))->sin_addr));
		//printf("command = %s \r\n",command);
		fp = popen(command, "r");
		if (!fp) 
		{
			fprintf(stderr, "popen error: %s\n", gai_strerror(status));
	        goto err;  // 无法解析域名
		}
		
		/* 根据fd进行信息读取与判断 */
		char result[1024];
		while (fgets(result, sizeof(result), fp) != NULL)
		{
			if(!strstr(result,"bytes from"))
				continue;
			printf("wifi data get ok\r\n");
			system("./CoreLogic/net_tran_msg_test 127.0.0.1 wifi");
			fclose(fp);
			return 0;
		}

		err: sleep(1);
    }
	return 0;
}

