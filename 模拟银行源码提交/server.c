#include "server.h"      //服务器
int id_ask;
int id_reply;

struct PROCESS{
    char path[20];   //可执行文件的路径，相当于每个功能的
    pid_t pid;
};
struct PROCESS pro[]={{"./open"},{"./destroy"},{"./query"},{"./save"},{"./take"},{"./transfer"}};
int main()
{
    atexit(clearFun);       //注册清理函数
    signal(2,sigFun);       //清理对应的子进程
    id_ask = msgget(KEY_ASK,IPC_CREAT|0644);
    id_reply = msgget(KEY_REPLY,IPC_CREAT|0644);

    int i;
    for(i = 0;i < sizeof(pro)/sizeof(pro[0]);i++){
        pro[i].pid = fork();   
        if(pro[i].pid == 0){
            execl(pro[i].path,pro[i].path,NULL);    //开子进程
        }
    }

    printf("输入回车结束服务器\n");
    getchar();
    return 0;
}

void sigFun(int n)
{
    exit(0);
}
void clearFun(void)
{
    int i;
    for(i=0;i<sizeof(pro)/sizeof(pro[0]);i++){
        kill(pro[i].pid,9);
    }
    msgctl(id_ask,IPC_RMID,NULL);
    msgctl(id_reply,IPC_RMID,NULL);
}
