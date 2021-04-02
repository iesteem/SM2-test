#pragma once
#include<stdlib.h>
#include"miracl.h"
#include"mirdef.h"
#include"keyPair.h"
#include"operation.h"

extern strData inputData;//加密输入
extern epoint*  otherPK; //对方的公钥
extern char* ccode;		 //密文(全局变量)
extern keyPair* otherKey;//对方的密钥对
extern char* outpudata;	 //解密输出(全局变量)

char* myEncryption(char*, char*);//加密
char* myDecryption(char*, char*);//解密



