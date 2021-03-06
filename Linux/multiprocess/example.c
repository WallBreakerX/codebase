#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/ipc.h>
#include <sys/types.h>
#include<sys/msg.h>
#include<netinet/tcp.h> 

int glob=10;

#define SIZETEXT    12

typedef struct mymsg{
    long int mtype;
    char mtext[SIZETEXT];
}MSG;

static void my_exit1(void)  //进程退出时调用函数
{
    printf("=== first exit handler pid=%d\n",getpid());
}
 
static void my_exit2(void)
{
    printf("second exit handler === pid=%d\n",getpid());
}
 
int main()
{
    struct tcp_info info; 

    int local = 8;
    pid_t pid;
    // key_t key = ftok("/tmp",66);
    // if (key == -1){
    //     printf("Create key faile\n");
    //     exit(0);
    // }

    int msgid = msgget(1234, IPC_CREAT | 0666);
    if (msgid == -1){
        printf("Create msgid faile\n");
        exit(0);
    }
    printf(">>> msgid: %d\n", msgid);


    if(atexit(my_exit1)!=0)  //为进程注册的退出时调用函数也会被子进程共享，先注册的后调用
    {
        perror("atexit");
    }
 
    if(atexit(my_exit2)!=0)
    {
        perror("atexit");
    }
 
    pid = fork();
    if(pid == 0)
    {
        for (int i=0; i<10; i++){
            MSG msgtext = {0};
            msgtext.mtype = 1;
            sprintf(msgtext.mtext, "I killed %d man\n", i);
            
            if (msgsnd(msgid, (void*)&msgtext, sizeof(msgtext.mtext), IPC_NOWAIT) != 0)
                printf(">>>> msg is FULL!!!Send faile\n");
            else{
                printf(">> Son Send: %s\n", msgtext.mtext);
            }
        }
        MSG msgtext = {0};
        msgtext.mtype = 1;
        sprintf(msgtext.mtext, "Over");
        printf(">> Son Send: %s\n", msgtext.mtext);
        msgsnd(msgid, (void*)&msgtext, 64, 0);
    }
    else if(pid > 0)
    {
        sleep(10);
        while(1){
            MSG msgtext = {0};
            memset(&msgtext, 0 ,sizeof(msgtext));
            msgrcv(msgid, (void*)&msgtext, sizeof(msgtext.mtext), 0, 0);
            printf(">> Father Recv: %s\n", msgtext.mtext);

            if (strcmp(msgtext.mtext, "Over") == 0){
                printf("< Father exit, destroy msg\n");
                msgctl(msgid, IPC_RMID, NULL);
                break;
            }
            sleep(1);
        }    
    }
    else
    {
        perror("fork");
    }
    printf(">>> OVER: pid=%d,glob=%d,localar=%d\n",getpid(),glob,local);//这段代码父子进程共享
    exit(0);//也可以使用exit(0)
}