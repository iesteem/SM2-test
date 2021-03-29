#pragma once
#include<stdlib.h>
#include"miracl.h"
#include"mirdef.h"


extern miracl *mip;
extern int Max;
/*带有长度size的unsigned char数组(字节数组)*/
typedef struct strData
{
	unsigned char* data;
	int size;
}strData;
/*密钥对存储格式*/
typedef struct keyPair
{
    big Px;
    big Py;
    big d;
}keyPair;


/*
椭圆曲线点运算
*/
epoint* NewPoint(big x, big y);
epoint* AddEpoint(epoint*, epoint*);
epoint* MultiplyEpoint(big, epoint*);
big PointX(epoint*);
big PointY(epoint*);

/*
大数运算,十六进制
*/
big Xor2(big x, big y);        // x ^ y（异或）
big And2(big x, big y);        // x & y（与）
big Add2(big x, big y);        // x + y 
big Sub2(big x, big y);        // x - y 
big Multiply2(big x, big y);   // x * y 
big Divide2(big x, big y);     // x / y 
big Mod2(big x, big y);        // x % y 
big Pow2(big x, int y);	       // x ^ y(幂次)

/*
形式转换
*/
char* ConvertstrDataAsHex(strData*);    //字符串 ----> 十六进制串
char* GetPartHexStr(char*, int, int);	//截取十六进制串
big HexCharsToBig(char*);               //十六进制串 ----> 大数
char* BigToHexChars2(big x);            //大数 ----> 十六进制串
strData * EpointToBytes(epoint *);      //坐标点 ----> 明文输入类型, 首字节为04h
//int big_to_bytes(int,big,char*,bool); //大数 ----> 字符串，返回字符串长度


