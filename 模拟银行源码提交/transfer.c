#include "transfer.h"
int id_ask;
int id_reply;
struct TAG buf,rbuf,rbuf_to;

int main()
{
	printf("转账功能启动....\n");
    id_ask = msgget(KEY_ASK,IPC_CREAT|0644);
    id_reply = msgget(KEY_REPLY,IPC_CREAT|0644);

    while(1){               //转账功能相当于是在一直运行
        memset(&buf,0,sizeof(buf));
		memset(&rbuf,0,sizeof(rbuf));
        msgrcv(id_ask,&buf,sizeof(buf)-sizeof(long),TRANSFER,0);    //接收转账的消息
		if(buf.money == 0)
		{
			transfer_account();           //账户转账
		}
		else if(buf.money != 0)
		{
			transfer_account_query();
		}
        msgsnd(id_reply,&buf,sizeof(buf)-sizeof(long),0);
    }
	return 0;
}

int transfer_account()
{
	char file[30]={0};
	sprintf(file,"account/%d.txt",buf.id);
	int fd = open(file,O_RDONLY);
	if(fd == -1)
	{
		sprintf(buf.err,"转账失败，账户不存在!");
	}
	else
	{
		read(fd,&rbuf,sizeof(rbuf));
		if(strcmp(buf.passwd,rbuf.passwd) == 0){    //将输入的密码，和查询的密码相比较
			buf.money = rbuf.money;
		}
		else{
			sprintf(buf.err,"转账失败，账户密码错误!");
		}
		close(fd);
	}
	return 0;
}

int transfer_account_query()
{
	char file[30]={0};
	char file_to[30]={0};
	sprintf(file,"account/%d.txt",buf.id);
	sprintf(file_to,"account/%d.txt",buf.to_id);
	int fd = open(file,O_RDWR);
	int fd_to = open(file_to,O_RDWR);
	if(fd_to == -1)
	{
		sprintf(buf.err,"转账失败，对方账户不存在!");
	}
	else
	{
		read(fd,&rbuf,sizeof(rbuf));   //原本的账户信息
		read(fd_to,&rbuf_to,sizeof(rbuf_to));  //对方的账户信息
		if(buf.money <= rbuf.money)
		{
			buf.money = rbuf.money - buf.money;         //回复到客户端
			rbuf.money = buf.money;                     //更新自己金额到服务器
			rbuf_to.money = rbuf_to.money + buf.money;  //更新对方金额到服务器
			lseek(fd,0,SEEK_SET);
			write(fd,&rbuf,sizeof(rbuf));
			lseek(fd_to,0,SEEK_SET);
			write(fd_to,&rbuf_to,sizeof(rbuf_to));
			sprintf(buf.err,"转账成功！");
		}
		else
		{
			sprintf(buf.err,"取钱失败，账户金额不足!");
		}
		close(fd);
		close(fd_to);
	}
	return 0;
}