#ifndef CLIENT_H
#define CLIENT_H

/******本地变量*********/
#define Maxsize 8  //人数限制
typedef struct friend{
    int onoff;
    char* UID;
    char* name;
    char buffer[200];  //缓存
    FILE *fid;  //日志文件指针 指向一个文本文件
    char* filename; //文件名
    int counter; //消息计数器
    struct friend *next;
}Friend;    //好友基本数据

typedef struct client{
    int online;
    Friend* head;
    char* UID;
    char* name;
    char* password;
}Client,*CL;    //用户数据
Client suser[Maxsize];

char smsg[1024];  //发送的信息,一次最多接受200个字符
char rmsg[1024];  //接受的消息 
int event;  //标志事件
int count; //在线人数
int Link;  //事件响应链接

#endif/*CLIENT_H*/



