 #define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include"miracl.h"
#include"mirdef.h"
#include<sm2.h>
#include"operation.h"

int Max = 6000;
miracl *mip;
big k;//随机数
keyPair* myKey;//发送方的密钥对
keyPair* otherKey;//接收方的密钥对(私钥为0)--需要接收对方传输的结果
epoint*  otherPK;//接收方方的公钥

int lengthC1x;						//C1的x长度
int lengthC1y;						//C1的y长度
int lengthC1 = 130;                 //C1的十六进制串长度
int lengthC3 = 64;					//C3的十六进制串长度
strData inputData;					//明文(全局变量)
char* ccode;						//密文(全局变量)
char* outpudata;			        //解密输出(全局变量)

big ID;					            //用户ID
char* signature;                    //签名信息
int lengthRS = 64;                  //签名中 r与s 的十六进制串长度

