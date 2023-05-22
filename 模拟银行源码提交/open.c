#include "open.h"   //服务器开户功能
int id_ask;
int id_reply;
int main()
{
    printf("开户功能启动....\n");
    id_ask = msgget(KEY_ASK,IPC_CREAT|0644);
    id_reply = msgget(KEY_REPLY,IPC_CREAT|0644);
    struct TAG buf;
    while(1){           //开户功能相当于是在一直运行
        memset(&buf,0,sizeof(buf));
        msgrcv(id_ask,&buf,sizeof(buf)-sizeof(long),OPEN,0);    //接收开户的消息
        buf.id = open_account();           //服务器生成账号
        if(save(buf)==-1){
            sprintf(buf.err,"开户失败!");
        }
        msgsnd(id_reply,&buf,sizeof(buf)-sizeof(long),0);
    }


    return 0;
}

//存放账户信息的文件名: id.txt
//保存信息成功返回0 失败返回-1
int save(struct TAG buf)
{
    char file[30]={0};
    sprintf(file,"account/%d.txt",buf.id);
    int fd = open(file,O_WRONLY|O_CREAT,0644);
    if(fd == -1){
        return -1;
    }
    write(fd,&buf,sizeof(buf));
    return 0;
}

//函数返回新开的账号 
//id.txt 保存的是最新的使用到的账号数值 
int open_account(void)
{
    int id = 1000;
    int fd = open("id.txt",O_RDWR|O_CREAT,0644);
    read(fd,&id,4);
    id++;
    lseek(fd,0,SEEK_SET);
    write(fd,&id,4);
    return id;
}

