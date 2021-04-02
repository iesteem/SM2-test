#include<stdlib.h>
#include "miracl.h"
#include "mirdef.h"
#include "operation.h"

/*
创建一个epoint
*/
epoint* NewPoint(big x, big y)
{
	epoint* result = (epoint*)malloc(sizeof(epoint));
	result = epoint_init();
	epoint_set(x, y, 0, result);
	return result;
}

/*
a(E) + b(E)
*/
epoint* AddEpoint(epoint* a, epoint* b)
{
	epoint* result = (epoint*)malloc(sizeof(epoint));
	result = epoint_init();
	epoint_copy(b, result);
	ecurve_add(a, result);
	return result;
}

/*
a(B) * b(E)
*/
epoint* MultiplyEpoint(big a, epoint* b)
{
	epoint* result = (epoint*)malloc(sizeof(epoint));
	result = epoint_init();
	ecurve_mult(a, b, result);
	return result;
}

/*
从epoint中获取x(B)
*/
big PointX(epoint* point)
{
	big x = mirvar(0);  //每个big类型被创建后必须赋初值
	big y = mirvar(0);
	epoint_get(point, x, y);
	mirkill(y);  //释放此大数占用的内存
	return x;
}

/*
从epoint中获取y(B)
*/
big PointY(epoint* point)
{
	big x = mirvar(0);
	big y = mirvar(0);
	epoint_get(point, x, y);
	mirkill(x);
	return y;
}

/*
x(B) 异或 y(B)
*/
big Xor2(big x, big y)
{
	big result = mirvar(0);
	unsigned char xstrData[1000];
	unsigned char ystrData[1000];
	int lengthX = big_to_bytes(0, x, xstrData, FALSE);  //将x(B)转换成字符串xstrData，从索引0开始，返回字符串的长度
	int lengthY = big_to_bytes(0, y, ystrData, FALSE);

	if (lengthX < lengthY)
	{
		for (int i = 0; i < lengthX; i++)
		{
			ystrData[lengthY - 1 - i] ^= xstrData[lengthX - 1 - i];  //从最低位开始异或运算
		}
		bytes_to_big(lengthY, ystrData, result);  //将字符串ystrData的前lengthY个字符 转换成大数result
	}
	else
	{
		for (int i = 0; i < lengthY; i++)
		{
			xstrData[lengthX - 1 - i] ^= ystrData[lengthY - 1 - i];
		}
		bytes_to_big(lengthX, xstrData, result);
	}

	return result;
}

/*
x(B) 与 y(B)
*/
big And2(big x, big y)
{
	big result = mirvar(0);
	unsigned char xstrData[1000];
	unsigned char ystrData[1000];
	int lengthX = big_to_bytes(0, x, xstrData, FALSE);  //将x(B)转换成字符串xstrData，从索引0开始，返回字符串的长度
	int lengthY = big_to_bytes(0, y, ystrData, FALSE);

	if (lengthX < lengthY)
	{
		for (int i = 0; i < lengthX; i++)
		{
			ystrData[lengthY - 1 - i] &= xstrData[lengthX - 1 - i];  //从最低位开始异或运算
		}
		bytes_to_big(lengthY, ystrData, result);  //将字符串ystrData的前lengthY个字符 转换成大数result
	}
	else
	{
		for (int i = 0; i < lengthY; i++)
		{
			xstrData[lengthX - 1 - i] &= ystrData[lengthY - 1 - i];
		}
		bytes_to_big(lengthX, xstrData, result);
	}

	return result;
}

/*
x(B) + y(B)
*/
big Add2(big x, big y)
{
	big result = mirvar(0);
	add(x, y, result); //令x(B) + y(B)，结果赋给result(B)
	return result;
}

/*
x(B) - y(B)
*/
big Sub2(big x, big y)
{
	big result = mirvar(0);
	subtract(x, y, result);  //令x(B) - y(B)，结果赋给result(B)
	return result;
}

/*
x(B) * y(B)
*/
big Multiply2(big x, big y)
{
	big result = mirvar(0);
	multiply(x, y, result);  //令x(B) * y(B)，结果赋给result(B)
	return result;
}

/*
x(B) / y(B)
*/
big Divide2(big x, big y)
{
	big x1 = mirvar(0);
	big y1 = mirvar(0);
	big z1 = mirvar(0);
	copy(x, x1);
	copy(y, y1);
	divide(x1, y1, z1);  //令x(B) / y(B)，结果赋给result(B)
	mirkill(x1);
	mirkill(y1);
	return z1;
}

/*
x(B) % y(B)
*/
big Mod2(big x, big y)
{
	big x1 = mirvar(0);
	big y1 = mirvar(0);
	big z1 = mirvar(0);
	copy(x, x1);
	copy(y, y1);
	powmod(x1, mirvar(1), y1, z1);  //令 x1(B)的1(B)次方 mod y1(B), 结果赋给result(B)
	mirkill(x1);
	mirkill(y1);
	return z1;
}

/*
x(B)的y次方
*/
big Pow2(big x, int y)
{
	big x1 = mirvar(0);
	big y1 = mirvar(0);
	copy(x, x1);  //将x(B)赋给x1(B)
	for (int i = 0; i < y; i++)
	{
		y1 = Multiply2(x1, x1);
	}
	mirkill(x1);
	return y1;
}


/*
strData ----> 十六进制串
*/
char* ConvertstrDataAsHex(strData* s)
{
	char* result = (char*)malloc(sizeof(char)*(s->size * 2 + 1));  //字符串结构转十六进制串(一个字符占一个字节，一个十六进制数占半个字节)
	for (int i = 0; i < s->size; i++)  //遍历字符串结构
	{
		sprintf(&result[i * 2], "%02x", s->data[i]);  //数组s的内容以十六进制形式转存到数组result
	}
	result[s->size * 2] = '\0';  //数组result的最后一个元素标识字符串结束
	return result;
}

/*
截取选定的十六进制串
*/
char* GetPartHexStr(char* sstr, int startIndex, int length)
{
	char* str = (char*)malloc(sizeof(char)*(length + 1));

	int i = 0;
	for (int j = 0; j < length; j++)
	{
		str[i++] = sstr[startIndex + j];
	}
	str[i] = '\0';
	return str;
}

/*
大数 ----> 十六进制串
*/
char* BigToHexChars2(big x)
{
	mip->IOBASE = 16;  //字符串设定为十六进制
	char *str = (char*)malloc(sizeof(char)*Max);
    str[0] = 0;
	cotstr(x, str);  //将大数转换成十六进制串
	return str;
}

/*
十六进制串 ----> 大数
*/
big HexCharsToBig(char* str)
{
	mip->IOBASE = 16;
	big result = mirvar(0);
	cinstr(result, str);
	return result;
}


/*
坐标点 ----> 明文输入类型, 首字节为04h
*/
strData* EpointToBytes(epoint *point)
{
	unsigned char *x = (unsigned char*)malloc(sizeof(unsigned char)*Max);
	unsigned char *y = (unsigned char*)malloc(sizeof(unsigned char)*Max);
	int lengthX = big_to_bytes(0, PointX(point), x, FALSE);  //字节长度
	int lengthY = big_to_bytes(0, PointY(point), y, FALSE);  //字节长度

	strData *result = (strData*)malloc(sizeof(strData));
	result->size = lengthX + lengthY + 1;  //strData长度
	result->data = (unsigned char*)malloc(sizeof(unsigned char)*(result->size));

	int i = 0;
	result->data[i++] = 4;  //result字符数组以 4 开头

	for (int j = 0; j < lengthX; j++)
	{
		result->data[i++] = x[j];
	}  //接着存入x

	for (int j = 0; j < lengthY; j++)
	{
		result->data[i++] = y[j];
	}  //再存入y

	return result;		// 4 || point->X || point->Y(字符串)
}

