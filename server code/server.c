/* ·þÎñÆ÷¶Ë server.c */
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

#include "server.h"
void Send_to_User(char *message, int ToUID,int type)
{
	gchar temp[1024];
	switch(type)
	{
		case 99:strcpy(temp,message);break;
		case 7:strcpy(temp,message);break;
		case 2:strcpy(temp,message);break;
		case 0:strcpy(temp,message);break;
		default:g_print("No such INfo Type\n");return ;
	}
	
	
	gint j;
	gint send_sockfd;
	struct sockaddr_in pin_addr;
   	bzero(&pin_addr, sizeof(pin_addr));
   	pin_addr.sin_family = AF_INET;
   	switch(ToUID)
   	{
   		case 0:pin_addr.sin_addr.s_addr = inet_addr("127.0.0.1");break;
   		case 1:pin_addr.sin_addr.s_addr = inet_addr("192.168.0.102");break;
   		case 2:pin_addr.sin_addr.s_addr = inet_addr("192.168.0.106");break;
   		case 3:pin_addr.sin_addr.s_addr = inet_addr("192.168.34.129");break;
   		case 4:pin_addr.sin_addr.s_addr = inet_addr("192.168.0.105");break;
   		default:pin_addr.sin_addr.s_addr = inet_addr("127.0.0.1");break;
	}
   
   	
 	pin_addr.sin_port = htons(PORT);  
 	if ((send_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
   	{
   		perror("socket");
      	return 0;
   	}
   	if (connect(send_sockfd, (void*) &pin_addr, sizeof(pin_addr)) == -1)
   	{
      	perror("connect");
      	return 0;
   	}
   	 if (send(send_sockfd, temp, strlen(temp), 0) == -1)
   	{
      	perror("send");
      	exit(1);
   	}
	close(send_sockfd);
	return;
}
void Init()
{
	strcpy(path,"/home/new/server/");
	strcpy(user[0].IP,"127.0.0.1");
	strcpy(user[1].IP,"192.168.0.102");
	strcpy(user[2].IP,"192.168.0.106");
	strcpy(user[3].IP,"192.168.34.129");
	strcpy(user[4].IP,"192.168.0.105");
}

void Analyse_Info(char* message)
{
	int Info_Type;
	Info_Type=(message[0]-'0')*10+(message[1]-'0');
	g_print("Info type:%d\n",Info_Type);
	switch(Info_Type)
	{
		case 0:SignUp(message);break;
		case 1:SignIn(message);break;
		case 2:SendMessage(message);break;
		case 3:FindFriends(message);break;
		case 4:AddFriends(message);break;
		case 5:VerifyPassFriends(message);break;
		case 6:VerifyDenyFriends(message);break;
		case 8:OffLine(message);break;
		default: SignIn(message);g_print("Error:no such Info type\n");return 0; 
	}
	return;
}



void  do_service (gpointer id)
{
	gint sd_id;
	sd_id=GPOINTER_TO_INT(id);

	while(read(pool[sd_id].sd,pool[sd_id].buf,1024)!=-1)
	{
		if(pool[sd_id].buf[0]=='\0')
		{
			continue;
		}
		g_print("%s\n",pool[sd_id].buf);
		Analyse_Info(pool[sd_id].buf);
		memset(pool[sd_id].buf ,0, sizeof(pool[sd_id].buf));
	}
}






int main(int argc, char* argv[])
{
	Init();
	gint sd, newsd;

	gint slen;
	gint count = 0;
	gint flags;
	gchar buf[1024];
	gchar tobuf[1024];
	gint length,i,j;
	struct sockaddr_in server_sin;
	if(g_thread_supported())
		g_thread_init(NULL);
	else
		g_print("thread not supported\n");
	sd = socket(AF_INET,SOCK_STREAM,0);
	if(sd == -1)
	{
		g_print("create socket error!\n");
		return -1;
	}
	server_sin.sin_family = AF_INET;
	server_sin.sin_port = htons(OURPORT);
	server_sin.sin_addr.s_addr = htonl(INADDR_ANY);
	slen = sizeof(struct sockaddr_in);
	if(bind(sd,(struct sockaddr *)&server_sin,slen)<0)
	{	
		g_print("bind error!\n");
		return -1;
	}
	if(listen(sd,8)<0)
	{
		g_print("listen error!\n");
		return -1;
	}
	for(i=0; i<MAX_USERS; i++)
		user[i].in_use = FALSE;
	flags = fcntl(sd,F_GETFL);
	fcntl(sd,F_SETFL,flags&~O_NDELAY);
	for(;;)
	{
		g_print("cccccc");
		newsd = accept(sd,&server_sin,&slen);
		g_print("llllll");
		if(newsd == -1)
		{
			g_print("accept error!\n");
			break;
		}
		else
		{
				pool[count].sd=newsd;
				g_print("count:%d\n",count);
//´´½¨ÎªÓÃ»§·þÎñµÄÏß³Ì
				g_thread_create((GThreadFunc)do_service,(gpointer)count,TRUE,NULL);
				count++;
		}
	}//for(;;)
	close(sd);
	g_free(&server_sin);
}
