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
验证通过好友
message：05 FromUID ToUID
command：05 FromUID 01
operation：给ToUID发送信息验证通过05 FromUID 01
		   客户端需要更新好友列表以及显示新好友在线状态
*/
void VerifyPassFriends(char* msg)
{
	int ToUID = (msg[4]-'0')*10 + msg[5]-'0';
	int FromUID = (msg[2]-'0')*10 + msg[3]-'0';
	char temp[20];
	sprintf(temp,"050%d01",FromUID);
	Send_to_User(temp,ToUID,0);
	return;
}

/*
验证拒绝好友
message：06 FromUID ToUID
command：06 FromUID 00
operation：给ToUID发送信息验证通过05 FromUID 01
*/
void VerifyDenyFriends(char* msg)
{
	int ToUID = (msg[4]-'0')*10 + msg[5]-'0';
	int FromUID = (msg[2]-'0')*10 + msg[3]-'0';
	char temp[20];
	sprintf(temp,"050%d00",FromUID);
	Send_to_User(temp,ToUID,0);
	return;
}
/*
用户下线
message：08 FromUID
command：08 FromUID
operation：告诉所有好友FromUID下线
*/
void OffLine(int FromUID)
{
	//更新在线状态
	user[FromUID].in_use = 0;

	gint i;
	gchar temp[3];
	gchar head[5];

	//更新在线状态	提醒已经在线的好友，UID上线了
	for(i=0;i<5;i++)
	{
		if(user[i].in_use==1)
		{
			sprintf(temp,"%d",FromUID);
			strcpy(head,"080");
			strcat(head,temp);
			Send_to_User(head,i,8);
			memset(temp,0,sizeof(temp));
			memset(head,0,sizeof(head));
		}	
	}

	return;
}
