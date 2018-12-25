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
查找朋友
Message:03 FromUID ToUID
Command:03 UID %用户名
		99 用户不存在
*/

void FindFriends(char* msg)

{

	int i;
	int FromUID = (msg[2]-'0')*10+(msg[3]-'0');
	int ToUID = (msg[4]-'0')*10+(msg[5]-'0');
	for(i = 0;i < MAX_USERS;i ++)
	{
		if(i == ToUID)
		{
			char temp[20];char head[20];
			strcpy(head,"03");
			sprintf(temp, "%d", ToUID);
			strcat(head, temp);
		//	strcat(head, user[ToUID].name);
			Send_to_User(head, ToUID,3);
			return;
		}

	}

	Send_to_User("99No Exist",ToUID, 99);

	return;

}


/*
添加朋友
Message:04 FromUID ToUID,来自FromUID
Command:04 ToUID FromUID,发给ToUID	
*/

void AddFriends(char* msg)

{

	int FromUID = (msg[2]-'0')*10+(msg[3]-'0');
	int ToUID = (msg[4]-'0')*10+(msg[5]-'0');

	char temp1[20];char temp2[20];char head[20];
	strcpy(head,"04");
	sprintf(temp1,"%d",ToUID);
	sprintf(temp1,"%d",FromUID);
	strcat(head,temp1);
	strcat(head,temp2);
	Send_to_User(head, ToUID, 4);
	return;

}
