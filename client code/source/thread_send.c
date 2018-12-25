#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include<pthread.h>
#include <glib.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include "client.h"
#include "thread.h"
#define PORT 3769
typedef unsigned char INT8U;
typedef signed char INT8S;
typedef unsigned short int INT16U;
typedef signed short int INT16S;

/*发送线程*/
void* thread_send(void)
{
   char buf[200];
   int send_sockfd;
   struct sockaddr_in pin_addr;

   bzero(&pin_addr, sizeof(pin_addr));
   pin_addr.sin_family = AF_INET;
   pin_addr.sin_addr.s_addr = inet_addr("192.168.0.100");
 pin_addr.sin_port = htons(PORT);  
 if ((send_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
   {
      perror("socket");
      exit(1);
   }
   
   if (connect(send_sockfd, (void*) &pin_addr, sizeof(pin_addr)) == -1)
   {
      perror("connect");
      exit(1);
   }
   
//不断检查更新event的值，它不为0标志着事件发生
   if(event!=0)
{
   strcpy(buf,smsg);  //消息之中自带类型标识 
   printf("send message: %s\n",buf);
   if (send(send_sockfd, buf, strlen(buf), 0) == -1)
   {
      perror("send");
      exit(1);
   }
   event=0;
}
   
   close(send_sockfd);
   pthread_exit(NULL);
}

