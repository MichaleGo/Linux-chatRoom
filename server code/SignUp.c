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
注册账号，服务器已经准备好五个IDIP对
Message:00 00 00 name % passwd % ip 	
Command:00 ToUID
*/
void SignUp(char* message)
{
	int Type=(message[2]-'0')*10+(message[3]-'0');
	if(Type!=0)
	{
		return;
	}
	char tempip[20];
	char messagetemp[100];
	char UIDtemp[5];
	char tempname[100];
	char temppassword[100];
	memset(UIDtemp,0,sizeof(UIDtemp));
	memset(tempip,0,sizeof(tempip));
	int flag=0;
	int pip=0;
	int i=0;
	int UIDtempint=0;

	//解析 name passwd IP
	for( i=5;message[i]!='\0';i++)
	{
		if(message[i]=='%')
		{
			flag++;
		}
		if(flag==1)
		{
			if(message[i]=='%')
			{
				i++;
			}
			tempip[pip]=message[i];
			pip++;
		}
		
	}
	g_print("register info  ip:%s \n",tempip);

	//遍历所有用户，寻找IPID对，发送命令 00 ToUID
	for( i=4;i>=0;i--)
	{
	//	g_print("userip:%s\n",user[i].IP);
	//	g_print("tempip:%s\n",tempip); 
		if(strcmp(user[i].IP,tempip)==0)
		{
			sprintf(UIDtemp,"%d",i);
			UIDtempint=i;
			messagetemp[0]='0';messagetemp[1]='0';messagetemp[2]='0';
			strcat(messagetemp,UIDtemp);
			Send_to_User(messagetemp,i,0);
			memset(messagetemp,0,sizeof(messagetemp));
			memset(UIDtemp,0,sizeof(UIDtemp));
		}
	}

	//更新所有用户列表
	UpdateUserList(tempname,temppassword, UIDtempint);
}


/*
将用户的name password UID 写入用户文件
*/
void UpdateUserList(char* name,char* password,int UID)
{
	g_print("updating user list\n"); 
	char temp[50];
	int fd;

	//name 写到 name文件
	sprintf(temp,"%s0%d/name",path,UID);
	fd = open(temp, O_APPEND|O_RDWR, 0755);
    if(fd < 0)
    {
        perror("open name file failure! \n");
        exit(1);
    }
    g_print("%d\n",UID);
    write(fd,name,strlen(name));
    close(fd);

    //password 写到 password 文件夹
    memset(temp,0,sizeof(temp));
    sprintf(temp,"%s0%d/password",path,UID);
    fd = open(temp, O_APPEND|O_RDWR, 0755);
    if(fd < 0)
    {
        perror("open password file failure! \n");
        exit(1);
    }
    write(fd,password,strlen(password));
    close(fd);

    return ;
    
}
