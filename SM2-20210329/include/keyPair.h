#ifndef INCLUDE_KEYPAIR_H_
#define INCLUDE_KEYPAIR_H_

#include"miracl.h"
#include"mirdef.h"
#include"operation.h"
#include"sm2.h"

/*
定义外部导入
*/
extern miracl *mip;
extern int Max;
extern big k;//随机数k
extern keyPair* myKey;//我的密钥对
extern keyPair* otherKey;//对方的密钥对
extern epoint*  otherPK;//对方的公钥
extern char* p;						//椭圆曲线参数
extern char* a;						//
extern char* b;						//
extern char* n;						//
extern char* Gx;					//
extern char* Gy;					//
extern big ID;					    //用户ID
extern int lengthC1x;				//C1的x长度
extern int lengthC1y;				//C1的y长度
extern int lengthC1;                //C1的十六进制串长度
extern int lengthC3;				//C3的十六进制串长度
extern strData inputData;			//加密输入
extern char* ccode;					//加密得到的密文
extern char* outpudata;			    //解密输出
extern char* signature;             //签名信息
extern int lengthRS;                //签名中 r与s 的十六进制串长度

/*随机数*/
big GetBigRandom(big, big);		        //产生随机数 a<= result <=b
void InitRandomK();					    //产生随机参数K

/*密钥对*/
keyPair* CalculateKeys();               //产生公钥私钥对
int VerifyKeys(big, big);			    //验证公钥和私钥
epoint *CalculatePublicKey(big,big);    //公钥点(Px,Py)


#endif /* INCLUDE_KEYPAIR_H_ */
