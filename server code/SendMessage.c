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



/*
普通消息发送
Message:02 FromUID ToUID Time msg 	
Command:02 FromUID Time msg
*/
void SendMessage(char* msg)

{

	int FromUID = (msg[2]-'0')*10+(msg[3]-'0');
	int ToUID = (msg[4]-'0')*10+(msg[5]-'0');


	//判断send recv是否越界
	if(FromUID >= 5 || ToUID >= 5)

	{
		g_print("Error:Out range UID\n");
		return ;
	}

	char temp[1024];//temp 为需要发送的命令
	temp[0] = '0';temp[1] = '2';
	temp[2]=msg[2];temp[3]=msg[3];
	for(int i = 6;msg[i]!='\0';i++)
	{
		temp[i-2] = msg[i];
	}

	/*判断recv端是否在线
	  	在线，发送command: 02 FromUID time ：HH：MM：SS 消息内容。
	  	不在线，发送将信息发送到缓存区，完整消息。
	*/

	// char* ptr;
	// ptr = (char*)memcpy(temp,msg + 14,strlen(msg)-14 );//字符串段选复制
	if(user[ToUID].in_use == 1)

	{

		g_print("Sending to user\n");
		Send_to_User(temp,ToUID,2);

	}else{

		g_print("Sending to buffer\n");
		SendToBuffer(temp,ToUID);//将处理后的命令发送到缓存区，用户上线后，读取命令。
	}

	return;	

}



/*
A->B, B is offline,
将B本该收到的命令存到server 缓存文件， 等到B上线，然后读取命令
*/
void SendToBuffer(char* msg,int UID)

{
	int fd;
	char buff[1024];
	strcpy(buff,msg);
	char temp[100];
	sprintf(temp,"%s0%d/buffer",path,UID);
	g_print("%s\n",temp);
	fd = open(temp, O_APPEND|O_RDWR, 0755);
   	 if(fd < 0)
    	{
        	printf("open buffer failed\n");
        	return;
    	}
    	write(fd, buff,strlen(buff));
    	close(fd);
    	return;

}

