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
用户登录
message：01 UID password
command：无
operation：更新在线用户列表
           获取buff文档信息
*/
void SignIn(char* msg)
{
	int UID;
//printf("-----------------------------");
	UID=(msg[2]-'0')*10+(msg[3]-'0');
	if(UID>=5)
	{
		g_print("Error:Out range UID\n");
		return;
	}

//printf("-----------------------------");
	/*if(CheckIDPassword(msg)==-1)
	{
		return;
	}*/
	UpdateOnlineList(UID);  
//	GetBufferData(UID); 
	return;
}

int CheckIDPassword(char* msg)
{
	char* password;
	char passwordtemp[50]; 
	int UID=(msg[2]-'0')*10+(msg[3]-'0');
	
	if(UID > 4)
	{
		Send_to_User("99Wrong Password\n",(msg[2]-'0')*10+(msg[3]-'0'),99);
		return -1;
	}
	int i;
	for(i=4;msg[i]!='\0';i++)
	{

		passwordtemp[i-4]=msg[i];
	}
	passwordtemp[i] = '\n';
	passwordtemp[i+1] = '\0';

	char temp[50];
	sprintf(temp,"%s0%d/password");
	char* psaaword = ReadLine(temp);

	if(strcmp(password,passwordtemp) == 0)
	{	
		return 0;
	}else 
	{
		Send_to_User("99Wrong Password\n",(msg[2]-'0')*10+(msg[3]-'0'),99);
		return -1;
	}

	
}


void GetBufferData(int UID)
{
	char temp[150];
	sprintf(temp,"%s0%d/buffer",path,UID);
	g_print("%s\n",temp);

	char buff[1024];
        FILE *fp = fopen(temp, "r");
	while(fgets(buff,1024,fp)!=NULL)
	{
		Send_to_User(buff, UID, 2);
		memset(buff,0,sizeof(buff));
	}
	fclose(fp);
	ClearFile(temp);
	memset(temp,0,sizeof(temp));

	return ;
}


/*
in:UID
out:07 UID
operation：更新在线状态
		   通知UID好友，UID上线
		   给UID发送好友在线状态
*/
void UpdateOnlineList(int UID)
{
	//更新在线状态
	user[UID].in_use = 1;

	gint i;
	gchar temp[50];
	gchar head[50];

	for(i=0;i<5;i++)
	{
		if(user[i].in_use==1)
		{
			//通知UID好友，UID上线
			sprintf(temp,"%d",UID);
			strcpy(head,"070");
			strcat(head,temp);
			strcat(head,"%");
			strcat(head,user[i].IP);
			strcat(head,"%");
			Send_to_User(head,i,7);
			memset(temp,0,sizeof(temp));
			memset(head,0,sizeof(head));

			//给UID发送好友在线状态
			sprintf(temp,"%d",i);
			strcpy(head,"070");
			strcat(head,temp);
			strcat(head,"%");
			strcat(head,user[i].IP);
			strcat(head,"%");
			Send_to_User(head,UID,7);
			memset(temp,0,sizeof(temp));
			memset(head,0,sizeof(head));
		}
		
	}

	return;
}


