#include "client.h"   //客户端
int id_ask;
int id_reply;
struct TAG buf,rbuf;
int main()
{
    int ret;
    id_ask = msgget(KEY_ASK,IPC_CREAT|0644);     //用于向服务器发送的消息队列
    id_reply = msgget(KEY_REPLY,IPC_CREAT|0644);  //用于接受信息的消息队列

    while(ret = menu()){
        if(ret > 6 || ret < 0){
            printf("输入有误\n");
            continue;
        }
        if(ret == 1){    
            cli_open();      //开户
        }
        if(ret == 2){    
            cli_destroy();   //销户
        }
        if(ret == 3){    
            cli_save();      //存钱
        }
        if(ret == 4){    
            cli_take();      //取钱
        }
        if(ret == 5){    
            cli_transfer();  //转账
        }
        if(ret == 6){    
            cli_query();     //查询
        }
        if(ret == 0){        
            return 0;        //退卡
        }
    }
    return 0;
}

void cli_open(void)    //开户
{
    memset(&buf,0,sizeof(buf));
    buf.type = OPEN;
    printf("请输入账号密码及起始金额\n");
    scanf("%s%f",buf.passwd,&buf.money);
    
    msgsnd(id_ask,&buf,sizeof(buf)-sizeof(long),0);   //客户端发送开户请求
    memset(&rbuf,0,sizeof(rbuf));
    
    msgrcv(id_reply,&rbuf,sizeof(rbuf)-sizeof(long),OPEN,0);   //服务器反馈的开户结果，此时会阻塞等待
    if(strlen(rbuf.err) != 0){
        printf("open err:%s\n",rbuf.err);
    }  
    else{
        printf("新开的账户：%d  密码：%s   金额：%.2f\n",rbuf.id,rbuf.passwd,rbuf.money);     
    } 
}

void cli_destroy()    //销户
{
    memset(&buf,0,sizeof(buf));
    buf.type = DESTROY;
    printf("请输入要销户的账号，密码\n");
    scanf("%d %s",&buf.id,buf.passwd);

    msgsnd(id_ask,&buf,sizeof(buf)-sizeof(long),0);   //客户端发送销户请求

    memset(&rbuf,0,sizeof(rbuf));
    msgrcv(id_reply,&rbuf,sizeof(rbuf)-sizeof(long),DESTROY,0);   //服务器反馈的销户结果，此时会阻塞等待
    if(strlen(rbuf.err) != 0){
        printf("open err:%s\n",rbuf.err);
    }  
    else{
        printf("您要销户的账户：%d ，当前金额：%.2f\n",rbuf.id,rbuf.money);   
        printf("是否选择取出全部金额后，进行销户操作\n");  
        printf("是-----1         否(退出)------0 \n");
        printf("请输入选择：");
        scanf("%d",&buf.flag);
        msgsnd(id_ask,&buf,sizeof(buf)-sizeof(long),0);   //客户端发送销户请求
        memset(&rbuf,0,sizeof(rbuf));
        msgrcv(id_reply,&rbuf,sizeof(rbuf)-sizeof(long),DESTROY,0);   //服务器反馈的销户结果，此时会阻塞等待  
        printf("%s\n",rbuf.err);
    }
}

void cli_save(void)    //存钱
{
    memset(&buf,0,sizeof(buf));
    buf.type = SAVE;
    printf("请输入账号,密码,存钱金额\n");
    scanf("%d %s %f",&buf.id,buf.passwd,&buf.money);
    
    msgsnd(id_ask,&buf,sizeof(buf)-sizeof(long),0);   //客户端发送存钱请求
    memset(&rbuf,0,sizeof(rbuf));
    
    msgrcv(id_reply,&rbuf,sizeof(rbuf)-sizeof(long),SAVE,0);   //服务器反馈的存钱结果，此时会阻塞等待
    if(strlen(rbuf.err) != 0){
        printf("open err:%s\n",rbuf.err);
    }  
    else{
        printf("账户：%d  当前金额：%.2f\n",rbuf.id,rbuf.money);     
    }
}

void cli_take(void)    //取钱
{
    memset(&buf,0,sizeof(buf));
    buf.type = TAKE;
    printf("请输入账号,密码,取钱金额\n");
    scanf("%d %s %f",&buf.id,buf.passwd,&buf.money);
    
    msgsnd(id_ask,&buf,sizeof(buf)-sizeof(long),0);   //客户端发送取钱请求
    memset(&rbuf,0,sizeof(rbuf));
    
    msgrcv(id_reply,&rbuf,sizeof(rbuf)-sizeof(long),TAKE,0);   //服务器反馈的取钱结果，此时会阻塞等待
    if(strlen(rbuf.err) != 0){
        printf("open err:%s\n",rbuf.err);
    }  
    else{
        printf("账户：%d  当前金额：%.2f\n",rbuf.id,rbuf.money);     
    }
}

void cli_transfer(void)    //转账
{
    memset(&buf,0,sizeof(buf));
    buf.type = TRANSFER;
    printf("请输入账号,密码\n");
    scanf("%d %s",&buf.id,buf.passwd);
    
    msgsnd(id_ask,&buf,sizeof(buf)-sizeof(long),0);   //客户端发送转账请求

    memset(&rbuf,0,sizeof(rbuf));
    msgrcv(id_reply,&rbuf,sizeof(rbuf)-sizeof(long),TRANSFER,0);   //服务器反馈的转账结果，此时会阻塞等待

    if(strlen(rbuf.err) != 0){
        printf("open err:%s\n",rbuf.err);
    }  
    else{
        printf("账户：%d  当前金额：%.2f\n",rbuf.id,rbuf.money);
        printf("请输入转账金额，以及转账对方的账户\n");
        scanf("%f %d",&buf.money,&buf.to_id);
        msgsnd(id_ask,&buf,sizeof(buf)-sizeof(long),0);   //客户端发送转账请求

        memset(&rbuf,0,sizeof(rbuf));
        msgrcv(id_reply,&rbuf,sizeof(rbuf)-sizeof(long),TRANSFER,0);   //服务器反馈的转账结果，此时会阻塞等待
        printf("账户：%d  当前金额：%.2f\n",rbuf.id,rbuf.money);
        printf("%s\n",rbuf.err);
    }

}

void cli_query()    //查询
{ 
    memset(&buf,0,sizeof(buf));
    buf.type = QUERY;
    printf("请输入要查找的账号，密码\n");
    scanf("%d %s",&buf.id,buf.passwd);

    msgsnd(id_ask,&buf,sizeof(buf)-sizeof(long),0);   //客户端发送查询请求

    memset(&rbuf,0,sizeof(rbuf));
    msgrcv(id_reply,&rbuf,sizeof(rbuf)-sizeof(long),QUERY,0);   //服务器反馈的查询结果，此时会阻塞等待
    if(strlen(rbuf.err) != 0){
        printf("open err:%s\n",rbuf.err);
    }  
    else{
        printf("账户：%d  当前金额：%.2f\n",rbuf.id,rbuf.money);       
    }
}
      
int menu(void)   //菜单
{
    printf("1.\t开户\n");
    printf("2.\t销户\n");
    printf("3.\t存钱\n");
    printf("4.\t取钱\n");
    printf("5.\t转账\n");
    printf("6.\t查询\n");
    printf("0.\t退卡\n");
    printf("请输入进行的操作\n");
    int choose;
    if(scanf("%d", &choose) != 1){  // 输入不是一个整数
        printf("输入有误，请输入一个数字！\n");
        while(getchar() != '\n');  // 清空输入缓冲区中的非数字字符
    }
    return choose;
}