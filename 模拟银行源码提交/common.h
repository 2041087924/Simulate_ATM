#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define KEY_ASK     123  //客户端向服务器发消息
#define KEY_REPLY   321  //服务器给客户端反馈

#define OPEN         1 //开户消息类型 
#define DESTROY      2 //销户消息类型
#define QUERY        3 //查询消息类型
#define SAVE         4 //存钱消息类型
#define TAKE         5 //取钱消息类型
#define TRANSFER     6 //转账消息类型

struct TAG{
    long type;
    char passwd[10];    //密码
    float money;        //金额
    int id;             //账号，由服务器生成
    int flag;           //销户标志
    int to_id;          //转入的账户名
    char err[50];       //服务器给客户端反馈错误信息 
};

#endif 