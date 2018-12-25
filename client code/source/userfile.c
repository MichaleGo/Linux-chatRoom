#include <stdio.h>
#include <gtk/gtk.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include "client.h"
#include "GUI.h"
typedef unsigned char INT8U;
typedef signed char INT8S;
typedef unsigned short int INT16U;
typedef signed short int INT16S;

/*用户注册时创建文件夹*/
void create_user_file(Person_reg newuser)
{	
	char path[100]="/home/new/Desktop/neutalk/";
        int status1 = mkdir(path,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);  
	int status2 = mkdir(strcat(path,newuser.ID),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if(!status2){
		char path1[100],path2[100],path3[100];
		strcpy(path1,path);
		strcpy(path2,path);
		strcpy(path3,path);
		FILE *fp1=fopen(strcat(path1,"/self.txt"),"a+");
		FILE *fp2=fopen(strcat(path2,"/friend.txt"),"a+");
		status2 = mkdir(strcat(path3,"/history"),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); 
                fclose(fp2);
        	fprintf(fp1,"%s %s %s",newuser.ID, newuser.username, newuser.password);
		fclose(fp1);
	}
}

/*写入用户聊天记录*/
void write_chat_message(char *userID,char *friendID, char *chattime, char *chat_content, int type)
{
	char path[100]="/home/new/Desktop/neutalk/";
	strcat(path,userID);
	strcat(path,"/history/");
	strcat(path,friendID);
	strcat(path,".txt");
	FILE *fp=fopen(path,"a+");
	if(type){
		fprintf(fp,"%s      ID: %s\n",chattime,userID);
	}
	else
		fprintf(fp,"%s      ID: %s\n",chattime,friendID);	
	fprintf(fp,"%s\n",chat_content);
	fclose(fp);
}

/*添加新好友*/
void add_new_friend(char *userID, char *friendID)
{
	char path[100]="/home/new/Desktop/neutalk/";
	strcat(path,userID);
	strcat(path,"/history/");
	strcat(path,friendID);
	strcat(path,".txt");
	FILE *fp=fopen(path,"a+");
	fclose(fp);
}

