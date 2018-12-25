#include <glib.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include "client.h"
#include "thread.h"
#define PORT 5559
typedef unsigned char INT8U;
typedef signed char INT8S;
typedef unsigned short int INT16U;
typedef signed short int INT16S;

/*线程函数*/
void accept_msg(gpointer socket)
{
	gint new_sockfd;
	new_sockfd = GPOINTER_TO_INT(socket);
	char buf[2];
        
        Link=-1; //初始化
      if (recv(new_sockfd, rmsg, 200, 0) == -1)
      {
         perror("recv");
         exit(1);
      }
       
      printf("received from client :%s\n", rmsg);  //在控制台打印
      buf[0]=rmsg[0];  buf[1]=rmsg[1];
      int type=(buf[0]-'0')*10+(buf[1]-'0');
      switch(type){
      case 0:  Link=0;  break;   //注册信息
      case 1:  Link=1;  break;   //登录信息
      case 2:  Link=2; on_receive_message();  break;
      case 3:  Link=3;  break;
      case 4:  Link=4;  break;
      case 5:  Link=5;  break;
      case 6:  Link=6;  break;
      case 7:  Link=7;  break;
      case 8:  Link=8;  break;
      case 9:  Link=9;  break;   //结束发送消息标志
      case 99: Link=99;   break;   //错误信息
      default: Link=-1; break;   //事件结束标记
                  }
      while(Link>=0) ;  //等待对应事件处理结束
      
      memset(rmsg,0,sizeof(rmsg));


   close(new_sockfd);  
}

/*接收函数*/
void* thread_listen(void)
{
   Link=-1;
   char buf[200];
   int count=0;
   int reuse = 0;
   int recv_sockfd,new_sockfd;
   struct sockaddr_in sin_addr, pin_addr;
   int len, pin_addr_size;
   
   pin_addr_size = sizeof(struct sockaddr);
   memset(&sin_addr, 0 , sizeof(sin_addr));
   sin_addr.sin_family = AF_INET;
   sin_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   sin_addr.sin_port = htons(PORT);
   
   if ((recv_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
   {
      perror("socket");
      exit(1);
   }

   if (bind(recv_sockfd, (struct sockaddr*) &sin_addr, sizeof(sin_addr)) < 0)
   {
      perror("bind");
      exit(1);
   }
   
   if (listen(recv_sockfd, 8) < 0)
   {
      perror("listen");
      exit(1);
   }
   printf("ready to listen...\n");
   while(1)
   {
	   if ((new_sockfd = accept(recv_sockfd, (struct sockaddr *) &pin_addr,&pin_addr_size)) < 0)
      {
	         perror("accept");
	         exit(1);
      }
	     
		//多线程处理监听来自他人的消息 
      g_thread_create((GThreadFunc)accept_msg,	(gpointer) new_sockfd	,TRUE,NULL);
      sleep(1);
   
   }
  pthread_exit(NULL);
}

