#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "thread.h"
typedef unsigned char INT8U;
typedef signed char INT8S;
typedef unsigned short int INT16U;
typedef signed short int INT16S;

/*主函数*/
int main(int argc, char **argv)
{
    int res;
    pthread_t a_thread;
    pthread_t b_thread;
    pthread_t c_thread;
    void *thread_result;
    char ch;
    res = pthread_create(&a_thread, NULL, thread_listen,NULL);
    if (res !=0)
    {
       perror("Listen Thread creation failed");
       exit(EXIT_FAILURE);
    }
    
    
    res=pthread_create(&c_thread,NULL,thread_GUI,NULL);
    if(res!=0)
    {
      perror("Send Thread creation failed");
       exit(EXIT_FAILURE);
    }
    printf("waiting for thread GUI finished....\n");
    res = pthread_join(c_thread, &thread_result);   //等待GUI线程的结束
    if (res !=0)
    {
       perror("Thread join failed");
       exit(EXIT_FAILURE);
    }
    printf("GUI  finished!\n");
    
    exit(EXIT_FAILURE);

}


