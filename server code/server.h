#ifndef  _SERVER_H_
#define _SERVER_H_

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

#define OURPORT 3769
#define PORT 5559
#define MAX_USERS 5
#define MAX_SDPOOL 10000

typedef unsigned char INT8U;
typedef signed char INT8S;
typedef unsigned short int INT16U;
typedef signed short int INT16S;
typedef unsigned long int INT32U;
typedef signed long int INT32S;


char path[100];
struct _client 
{
//	gint sd;
//	gint send_sd;
	gboolean in_use;
//	gint UID;
	gchar IP[64];
	gchar name[64];
//	char buf[1024];
};
struct _sd_pool
{
	gint sd;
	gchar buf[1024];
};
typedef struct _client client;
typedef struct _sd_pool SD_Pool; 

client user[MAX_USERS];
SD_Pool pool[MAX_SDPOOL];
static int LineNumber = 1;
char path[100];


void Init();//Buff.txt, IDList, NewIDList
void Analyse_Info(char* message);////分析命令 ，处于线程绑定函数内部 

void SignUp(char* msg);//00
void SignIn(char* msg);//01
void SendMessage(char* msg);//02
void FindFriends(char* msg);//03
void AddFriends(char* msg);//04
void VerifyPassFriends(char* msg);//05
void VerifyDenyFriends(char* msg);//06
void OffLine(int FromUID);//08

//00
void UpdateUserList(char* name,char* password,int UID);
 

//01
int CheckIDPassword(char* msg);
void UpdateOnlineList(int UID);////遍历修改结构数组,并向用户发送上线用户 
void GetBufferData(int UID); //buff里面全是已经处理好的命令，只需要发送给UID

//02
void SendToBuffer(char* msg,int ToUID);
void SendToUser(char *message, int ToUID,int type);

//03

//04，AddFriends 直接发送命令

//05,用户接受请求,直接发送命令，客户端更新好友列表，然后发松07给双方好友，更新在线好友状态 

//06，用户拒绝请求 ，转发A的拒绝给B 

//07+ID，更新在线好友状态 
 
//08+ID，好友下线

#endif 
