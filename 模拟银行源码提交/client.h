#ifndef _CLIENT_H
#define _CLIENT_H

#include "common.h"
int menu(void);
void cli_open(void);        //开户
void cli_destroy();         //销户
void cli_query();           //查询
void cli_save(void);        //存钱
void cli_take(void);        //取钱
void cli_transfer(void);    //转账

#endif
