#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#define PORT 3769

char str[20];

int main(int argc, char **argv)
{
   char buf[200];
   char msg[256];
   int send_sockfd;
   struct sockaddr_in pin_addr;

   bzero(&pin_addr, sizeof(pin_addr));
   pin_addr.sin_family = AF_INET;
   pin_addr.sin_addr.s_addr = inet_addr("192.168.0.103");
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
  // printf("Sending msg %s to server...\n",str);
   while(1)
   {
      printf("please type msg:\n");
      scanf("%s",&str); 
//	char inttemp[1024];
//	sprintf(inttemp,"%d",send_sockfd);
//	strcat(inttemp,str);

   if (send(send_sockfd, str, strlen(str), 0) == -1)
   {
      perror("send");
      exit(1);
   }
   }
   close(send_sockfd);
   return 0;
}

