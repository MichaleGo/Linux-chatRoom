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
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 5559

void accept_msg(gpointer socket)
{
	//printf("new_sofd = %d\n",count);
	gint new_sockfd;
	new_sockfd = GPOINTER_TO_INT(socket);
	char buf[200];
   while (1)
   {
   	
      if (recv(new_sockfd, buf, 200, 0) == -1)
      {
         perror("recv");
         exit(1);
      }
      if(buf[0]=='\0')
      {
      	continue;
	  }
      printf("received from client :%s\n", buf);
      memset(buf,'\0',sizeof(buf));
   }
   close(new_sockfd);  
}


int main(int argc, char **argv)
{
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
/*
   if(setsockopt(recv_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
   {
                perror("setsockopet error\n");
                return -1;
   }
*/ 
   if (bind(recv_sockfd, (struct sockaddr*) &sin_addr, sizeof(sin_addr)) < 0)
   {
      perror("bind");
      exit(1);
   }
   
   if (listen(recv_sockfd, 5) < 0)
   {
      perror("listen");
      exit(1);
   }

while(1)
{
   if ((new_sockfd = accept(recv_sockfd, (struct sockaddr *) &pin_addr,&pin_addr_size)) < 0)
      {
         perror("accept");
         exit(1);
      }
      
   g_thread_create((GThreadFunc)accept_msg,	(gpointer) new_sockfd			,TRUE,NULL);
 //  sleep(1);
   
}
  return 0;
}


