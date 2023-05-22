#include "save.h"
int id_ask;
int id_reply;
struct TAG buf,rbuf;

int main()
{
	printf("存钱功能启动....\n");
    id_ask = msgget(KEY_ASK,IPC_CREAT|0644);
    id_reply = msgget(KEY_REPLY,IPC_CREAT|0644);

    while(1){               	//存钱功能相当于是在一直运行
        memset(&buf,0,sizeof(buf));
		memset(&rbuf,0,sizeof(rbuf));
        msgrcv(id_ask,&buf,sizeof(buf)-sizeof(long),SAVE,0);    //接收存钱的消息

		save_account();            //账户存钱
        msgsnd(id_reply,&buf,sizeof(buf)-sizeof(long),0);
    }
    return 0;
}

int save_account()
{
	char file[30]={0};
	sprintf(file,"account/%d.txt",buf.id);
	int fd = open(file,O_RDWR);
	if(fd == -1)
	{
		sprintf(buf.err,"存钱失败，账户不存在!");
	}
	else
	{
		read(fd,&rbuf,sizeof(rbuf));
		if(strcmp(buf.passwd,rbuf.passwd) == 0){    //将输入的密码，和查询的密码相比较
			buf.money = rbuf.money + buf.money;     //回复到客户端
			rbuf.money = buf.money;   				//更新金额到服务器
			lseek(fd,0,SEEK_SET);
			write(fd,&rbuf,sizeof(rbuf));
		}
		else
		{
			sprintf(buf.err,"存钱失败，账户密码错误!");
		}
		close(fd);
	}
	return 0;
}
