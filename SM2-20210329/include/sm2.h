#pragma once
#include <operation.h>
#include <sm3.h>
#include"miracl.h"
#include"mirdef.h"

/*
定义外部导入
*/
extern miracl *mip;
extern int Max;

extern big k;//随机数k
extern big ID;					    //用户ID

extern keyPair* myKey;//我的密钥对
extern keyPair* otherKey;//对方的密钥对
extern epoint*  otherPK;//对方的公钥

extern char* p;						//椭圆曲线参数
extern char* a;						//
extern char* b;						//
extern char* n;						//
extern char* Gx;					//
extern char* Gy;					//

extern int lengthC1x;				//C1的x长度
extern int lengthC1y;				//C1的y长度
extern int lengthC1;                //C1的十六进制串长度
extern int lengthC3;				//C3的十六进制串长度
extern int lengthRS;                //签名中 r与s 的十六进制串长度

extern strData inputData;			//加密输入
extern char* ccode;					//加密得到的密文
extern char* outpudata;			    //解密输出
extern char* signature;             //签名信息


epoint *CalculateG();				    //自定义参数G
epoint *CalculatePoint1();			    //计算(x1,y1)
epoint *CalculatePoint2();			    //计算(x2,y2)
char *CalculateC1();				    //计算C1
strData *CalculateC2();				    //计算C2
char* CalculateC3();				    //计算C3
big KDF(epoint*, int);				    //计算t
void Encryption();                      //加密
void Decryption();                      //解密

/*签名函数*/
big CalculateZ(big, big, big);
char* myMakeSign(big, char*, keyPair*);
void myVerifySign(big, char*, char*, keyPair*);


