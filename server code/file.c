/*
argc：命令行总的参数的个数,即argv中元素的格式。

* argv[ ]: 字符串数组,用来存放指向你的字符串参数的指针数组,每一个元素指向一个参数

argv[0]:指向程序的全路径名

argv[1]:指向在DOS命令行中执行程序名后的第一个字符串。

argv[2]:指向第二个字符串
*/
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#define MAX_LENTH 1024

#include "server.h"

int CreateFile(char *filename)
{
    int fd;
    fd = creat(filename,0755);
    if(fd < 0)
    {
        // printf("fd = %d \n", fd);
        printf("create file failure \n", fd);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("create file success!\n");
    }
    return fd;
}

void WriteFile(char* path[], char* buff)
{
    int fd;
    fd = open(path, O_APPEND|O_RDWR, 0755);
    if(fd < 0)
    {
        printf("open file.txt failed\n");
        return;
    }
    write(fd, buff,strlen(buff));
    close(fd);
    return;

}

char* ReadLine(char* path[])
{
    
    FILE *pf = fopen(path,"r");
    if(pf == NULL)
    {
        perror("file not open ");
    }   
    char* buff = (char*)malloc(sizeof(char)*200);

    int i = 1;char* temp;
    while((temp = fgets(buff,MAX_LENTH,pf))!=NULL)
    {
        if(i < LineNumber) 
        {
            i++;
            continue;
        }
        else break;
    }

    if(temp==NULL) 
        return NULL;
    else{
        fclose(pf);
        LineNumber++;
        return buff;
    }    
}



void ClearFile(char* path[])
{
    int fd = open(path, O_CREAT|O_RDWR, 0755);   
    /* 清空文件 */
    ftruncate(fd,0);
    /* 重新设置文件偏移量 */
    lseek(fd,0,SEEK_SET); 
    close(fd);
    return 0;
}



void test()
{
    char path[100];
    strcpy(path,"/home/new/server/test.txt");
    char buff[100];
    int fd;
    fd = CreateFile(path);


    //一行行写入文件
    strcpy(buff,"Lebron is\n");
    WriteFile(path,buff);
    strcpy(buff,"the best player\n");
    WriteFile(path,buff);
    strcpy(buff,"on the earth!\n");
    WriteFile(path,buff);

    
    //按行读取
    char *temp;
    while((temp = ReadLine(path))!=NULL){
        printf("%s",temp);
    }

    /* 清空文件 */
    ClearFile(path);

    return ;

}
/*
int main()
{
    char path[100];
    strcpy(path,"/home/new/server/test.txt");
    char buff[100];
    char *temp;

    int fd;
    fd = CreateFile(path);
    fd = open(path, O_CREAT|O_RDWR, 0755);
    if(fd < 0)
    {
        printf("open file.txt failed\n");
        return 0;
    }

    strcpy(buff,"Music is\n");
    write(fd, buff,strlen(buff));
    strcpy(buff,"Bravo!\n");
    write(fd, buff,strlen(buff));
    
    while((temp = ReadLine(path))!=NULL){
        printf("%s",temp);
    }

    ftruncate(fd,0);

    lseek(fd,0,SEEK_SET); 
    close(fd);

    return 0;

}

*/
