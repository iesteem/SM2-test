 #define _CRT_SECURE_NO_WARNINGS
#include<sm2.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string.h>
#include"operation.h"
#include"miracl.h"
#include"mirdef.h"
#include"sm3.h"
#include"keyPair.h"

/*
定义给定常量，以十六进制串表示.  (大数以十六进制形式表示)
*/
char* p = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF";
char* a = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC";
char* b = "28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93";
char* n = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123";
char* Gx = "32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7";
char* Gy = "BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0";


/***********************************
*****************加密***************
注释：整个加密过程使用同一个随机数k
***********************************/
void Encryption()
{
Restart://重新开始生成参数
	InitRandomK();//初始化随机数

	//C1，C3, 十六进制串
	char* c1strData = CalculateC1();
	char* c3strData = CalculateC3();

	//C2，将字符串形式转换成十六进制串
	strData *C2 = CalculateC2();
	if (C2->size <= 0 && C2->data == NULL)	//t全零
	{
		goto Restart;
	}
	char* c2strData = ConvertstrDataAsHex(C2);
	free(C2->data);
	free(C2);


	//打印加密重要参数
	printf("***************椭圆曲线方程为：y^2=x^3+a*x+b\n");
	printf("p =%s\n", p);
	printf("a =%s\n", a);
	printf("b =%s\n", b);
	printf("n =%s\n", n);
	printf("Gx=%s\n", Gx);
	printf("Gy=%s\n", Gy);
	printf("k =%s\n\n", BigToHexChars2(k));
	/*
	printf("***************秘钥参数如下****************\n");
	printf("私钥 =%s\n", BigToHexChars2(myKey->d));
	printf("公钥x=%s\n", BigToHexChars2(myKey->Px));
	printf("公钥y=%s\n\n", BigToHexChars2(myKey->Py));
	*/
	printf("***************加密中间数据如下************\n");
	printf("C1=%s\n", c1strData);
	printf("C3=%s\n", c3strData);
	printf("C2=%s\n\n", c2strData);


	/*拼接字符串*/
	char *c = (char*)calloc(strlen(c1strData) + strlen(c2strData) + strlen(c3strData) + 1, sizeof(char));  //完整十六进制串分配内存
	strcat(c, c1strData);  //strcat拼接时，c1strData会覆盖c串的\0，保留c1strData串的\0
	strcat(c, c3strData);
	strcat(c, c2strData);
	free(c1strData);
	free(c3strData);
	free(c2strData);
	ccode = c;


	printf("明文: \n%s\n\n\n", inputData.data);  //明文数据
	//printf("密文：\n%s\n\n", ccode);              //密文数据

}


/*********************************
**************解密****************
注释：不涉及随机数k
*********************************/
 void Decryption()
 {
     /*验证十六进制串C1是否满足椭圆曲线方程*/
     char* x1strData = GetPartHexStr(ccode, 2, lengthC1x);  //字符串中4在十六进制串中表示为 “04”
     char* y1strData = GetPartHexStr(ccode, 2 + lengthC1x, lengthC1y);
     big C1x = mirvar(0);
     big C1y = mirvar(0);
     C1x = HexCharsToBig(x1strData);
     C1y = HexCharsToBig(y1strData);
     if (!mr_compare(Mod2(Pow2(C1y, 2), HexCharsToBig(p)), Mod2(Add2(Pow2(C1x, 3), Add2(Multiply2(C1x, HexCharsToBig(a)), HexCharsToBig(b))), HexCharsToBig(p))))		// if((PBy^2 %p) != ((PBx^3 + a*PBx +b)%p))
     {
         printf("C1验证无效，请重启程序\n");
         system("pause");
         exit(1);
     }
     /*椭圆曲线点C1，C2*/
     epoint* C1 = NewPoint(HexCharsToBig(x1strData), HexCharsToBig(y1strData));
     epoint* C2 = MultiplyEpoint(otherKey->d, C1);	//求解 [DB]C1=(x2, y2)
     epoint_free(C1);
     /*拆分密文*/
     char* C1strData = GetPartHexStr(ccode, 0, lengthC1);
     char* C3strData = GetPartHexStr(ccode, lengthC1, lengthC3);
     char* C2strData = GetPartHexStr(ccode, lengthC1 + lengthC3, strlen(ccode)-lengthC1 - lengthC3);
     /*大数t判零*/
     int klen = strlen(C2strData) / 2;        //明文数据的字符长度
     big t = KDF(C2, klen);					//求解 t = KDF(x2∥y2,klen)
     if (mr_compare(t, mirvar(0)) == 0)
     {
         printf("解密时t全0，错误\n");
         system("pause");
         exit(3);
     }
     /*求得M'*/
     big C2Number = mirvar(0);
     C2Number = HexCharsToBig(C2strData);
     char* mcode = BigToHexChars2(Xor2(C2Number, t));
     mirkill(C2Number);
     /*拼接十六进制串：xmy = x2  ||  M'  ||  Y2*/
     char* xmy = (char*)calloc(strlen(BigToHexChars2(PointX(C2))) + strlen(BigToHexChars2(PointY(C2))) + strlen(mcode) + 1, sizeof(char));
     strcat(xmy, BigToHexChars2(PointX(C2)));
     strcat(xmy, mcode);
     strcat(xmy, BigToHexChars2(PointY(C2)));
     /*比较u与C3*/
     if (strcmp(SM3ByHexStr(xmy), C3strData) != 0)
     {
         printf("破译失败\n");
         return;
     }
     free(xmy);
     /*将大数以字符串形式输出*/
     big mNumber = mirvar(0);
     mNumber = HexCharsToBig(mcode);
     char mstrData[1000];
     int mstrDataLength = big_to_bytes(0, mNumber, mstrData, FALSE);  //大数转换成字符串
     mstrData[mstrDataLength] = '\0';  //补\0
     outpudata = mstrData;
     //printf("翻译成明文为:\n%s\n\n", mstrData);
 }


 /***********************
*******制作签名**********
***********************/
 char* myMakeSign(big z, char* c, keyPair* key)
 {
     char *zc = (char*)calloc(strlen(BigToHexChars2(z)) + strlen(c) + 1, sizeof(char));  //完整十六进制串分配内存
     strcat(zc, BigToHexChars2(z));  //strcat拼接时，c1strData会覆盖c串的\0，保留c1strData串的\0
     strcat(zc, c);
     big E = mirvar(0);
     E = HexCharsToBig(SM3ByHexStr(zc));
     Restart:
     /*3*/
     InitRandomK();	    //初始化随机数
     /*4*/
     epoint *point1 = CalculatePoint1();
     /*5*/
     big x1 = mirvar(0);
     x1 = PointX(point1);
     //epoint_free(point1);
     big r = mirvar(0);
     r = Mod2(Add2(E, x1), HexCharsToBig(n));
     if ((mr_compare(r, mirvar(0)) == 0) || (mr_compare(Add2(r, k), HexCharsToBig(n)) == 0))
     {
         printf("r is bad!\n");
         goto Restart;
     }
     /*6*/
     big s1 = Add2(mirvar(1), key->d);
     xgcd(s1, HexCharsToBig(n), s1, s1, s1);
     big s2 = mirvar(0);
     s2 = Mod2(Sub2(k, Multiply2(r, key->d)), HexCharsToBig(n));
     big s = mirvar(0);
     s = Mod2(Multiply2(s1, s2), HexCharsToBig(n));
     if ((mr_compare(s, mirvar(0)) == 0))
     {
         printf("s is bad!\n");
         goto Restart;
     }
     /*拼接字符串*/
     char *ccc = (char*)calloc(strlen(BigToHexChars2(r)) + strlen(BigToHexChars2(s)) + 1, sizeof(char)); //完整十六进制串分配内存
     strcat(ccc, BigToHexChars2(r));
     strcat(ccc, BigToHexChars2(s));
     signature = ccc;
     printf("签名:\n%s\n\n", signature);  //签名
     return ccc;
 }

/*********************
	   验证签名
*********************/
void myVerifySign(big z, char* ciphertext, char* signatures, keyPair* key)
{
    /*拆分签名*/
    char* RstrData = GetPartHexStr(signatures, 0, lengthRS);
    char* SstrData = GetPartHexStr(signatures, strlen(signatures) - lengthRS, lengthRS);
    //1.
    int r1 = mr_compare(HexCharsToBig(RstrData), HexCharsToBig(n));
    if (r1 != (-1))
    {
        printf("r1 is bad!\n");
        system("pause");
        exit(1);
    }
    int r2 = mr_compare(HexCharsToBig(RstrData), mirvar(0));
    if (r2 != (+1))
    {
        printf("r2 is bad!\n");
        system("pause");
        exit(1);
    }
    //2.
    int s1 = mr_compare(HexCharsToBig(SstrData), HexCharsToBig(n));
    if (s1 != (-1))
    {
        printf("s1 is bad!\n");
        system("pause");
        exit(1);
    }
    int s2 = mr_compare(HexCharsToBig(SstrData), mirvar(0));
    if (s2 != (+1))
    {
        printf("s2 is bad!\n");
        system("pause");
        exit(1);
    }
    //3.+4.
    char *zc = (char*)calloc(strlen(BigToHexChars2(z)) + strlen(ciphertext) + 1, sizeof(char));  //完整十六进制串分配内存
    strcat(zc, BigToHexChars2(z));  //strcat拼接时，c1strData会覆盖c串的\0，保留c1strData串的\0
    strcat(zc, ciphertext);
    big e = mirvar(0);
    e = HexCharsToBig(SM3ByHexStr(zc));
    //5.
    big t = mirvar(0);
    t = Mod2(Add2(HexCharsToBig(RstrData), HexCharsToBig(SstrData)), HexCharsToBig(n));
    //printf("t = %s\n", BigToHexChars2(t));
    if (mr_compare(t, mirvar(0)) == 0)
    {
        printf("t==0，signature is bad!\n");
        system("pause");
        exit(1);
    }
    /*保证缓存*/
	printf("Gx=%s\n\n", Gx);
	printf("Gy=%s\n\n", Gy);
	epoint *GG = (epoint*)malloc(sizeof(epoint));
	GG = epoint_init();
	BOOL flag = 1;
	VerifyKeys(HexCharsToBig(Gx), HexCharsToBig(Gy));	//验证公钥和私钥
	flag = epoint_set(HexCharsToBig(Gx), HexCharsToBig(Gy), 0, GG);
	//printf("%d\n", flag);
    //6.
    epoint* PA = CalculatePublicKey(key->Px,key->Py);
    epoint* G = CalculateG();
    epoint* point1 = AddEpoint(MultiplyEpoint(HexCharsToBig(SstrData), G), MultiplyEpoint(t, PA));
    //7.
    big x1 = mirvar(0);
    x1 = PointX(point1);
    big R = mirvar(0);
    R = Mod2(Add2(e, x1), HexCharsToBig(n));;
    if (mr_compare(R, HexCharsToBig(RstrData)) != 0)
    {
        printf("R =! r，signature is bad!\n");
        system("pause");
        exit(1);
    }
    printf("signature is good!\n\n");
}




/***********************
参数G
***********************/
epoint * CalculateG()
{
	epoint *G = (epoint*)malloc(sizeof(epoint));
	G = epoint_init();
	epoint_set(HexCharsToBig(Gx), HexCharsToBig(Gy), 0, G);
	return G;
}

/*************************************
计算(x1,y1)
注释：使用使用全局变量k进行运算
*************************************/
epoint *CalculatePoint1()
{
	return MultiplyEpoint(k, CalculateG());
}

/**********************************
计算(x2,y2)
注释：使用全局变量k进行运算
**********************************/
epoint *CalculatePoint2()
{
	return MultiplyEpoint(k, otherPK);
}



/*******************************************
计算C1 = [k]G，以十六进制串表示，占65字节
注释：使用了全局变量随机数k
*******************************************/
char * CalculateC1()
{
    epoint *point1 = CalculatePoint1();
	unsigned char *x1 = (unsigned char*)malloc(sizeof(unsigned char)*Max);
	unsigned char *y1 = (unsigned char*)malloc(sizeof(unsigned char)*Max);
	lengthC1x = big_to_bytes(0, PointX(point1), x1, FALSE) * 2;
	lengthC1y = big_to_bytes(0, PointY(point1), y1, FALSE) * 2;  //字符个数 * 2 = 十六进制个数

	/*
	椭圆曲线点-->字符串-->十六进制串
	*/
	strData *result = EpointToBytes(point1);  //64字节的point1转65字节的result
	char* C1 = ConvertstrDataAsHex(result);
	epoint_free(point1);  //暂存变量被释放
	return C1;
}

/************************
计算C2，以字符串表示
注释：使用了全局变量随机数k
*******************************/
strData * CalculateC2()
{
	strData *result = (strData*)malloc(sizeof(strData));
	epoint *point2 = CalculatePoint2();
	big t = KDF(point2, inputData.size);  //KDF函数(使用全局变量)
	if (mr_compare(t, mirvar(0)) == 0)
	{
		result->data = NULL;
		result->size = -1;
		return result;
	}//if--判零
	epoint_free(point2);  //暂存变量被释放
	/*明文数据M不定长，可能超出大数定义范围，不能使用大数运算，故使用字符串求异或运算*/
	unsigned char *tstrData = (unsigned char*)malloc(sizeof(unsigned char)*Max);
	int lengthT = big_to_bytes(0, t, tstrData, FALSE);		//将t转换成字符串,返回字符个数
	if (lengthT != inputData.size)
	{
		for (int i = lengthT - 1; i >= 0; i--)
		{
			tstrData[inputData.size - lengthT + i] = tstrData[i];
		}

		for (int i = 0; i < inputData.size - lengthT; i++)
		{
			tstrData[i] = 0;
		}
		lengthT = inputData.size;  //tstrData(t)的字符长度和inputData.size(M)的长度已经一致
	}

	result->data = (unsigned char*)malloc(sizeof(unsigned char)*lengthT);
	for (int i = 0; i < lengthT; i++)
	{
		result->data[i] = tstrData[i] ^ inputData.data[i];  //C2 = t ^ M ，以字符串形式进行运算
	}
	result->size = lengthT;  //将t的字符长度赋给C2的字符个数
    /*释放暂存变量*/
	free(tstrData);
	mirkill(t);
	return result;
}

/***********************
KDF函数，返回大数
***********************/
big KDF(epoint* point2, int klen)
{
	unsigned char* xStr = (unsigned char*)malloc(sizeof(unsigned char)*Max);
	unsigned char* yStr = (unsigned char*)malloc(sizeof(unsigned char)*Max);
	int lengthX = big_to_bytes(0, PointX(point2), xStr, FALSE);  //字符个数
	int lengthY = big_to_bytes(0, PointY(point2), yStr, FALSE);  //字符个数
	free(xStr);
	free(yStr);

	big x = PointX(point2);  //大数x表示point2的横坐标
	big y = PointY(point2);  //大数y表示point2的纵坐标

	big t  = mirvar(0);
	big ct = mirvar(1);	 //计数器至少32位，初值为1
	big V0 = mirvar(0);
	big Ha = mirvar(0);
	big Z  = mirvar(0);
	int v  = 32;	 //sm3的hash值长度为32字节(256bit)，基本单位为字节(字符)

	/*
	求解Z
	*/
	copy(x, Z);
	sftbit(Z, lengthY * 8, Z);	//左移y字符串所占的比特数，lengthY表示字符个数
	add(Z, y, Z);  //Z比特拼接y 最终Z= x||y

	/*
	klen表示明文中的字符个数
	*/
	if (klen / v >= 1)
	{
		for (int i = 1; i <= klen / v; i++)
		{
			sftbit(Z, 32, t);					//Z左移32位数
			add(t, ct, t);						//t比特拼接ct

			big sm3Value = SM3(t);  //求t的hash值 t= Z||ct
			copy(sm3Value, V0);					
			mirkill(sm3Value);
			add(Ha, V0, Ha);					//Ha比特拼接V0

			sftbit(Ha, 256, Ha);				//Ha左移V0位数，为下一次拼接做准备 Ha= Ha||x||y||ct
			add(ct, mirvar(1), ct);				//计数器加一
		}
		sftbit(Ha, -256, Ha);					//去掉多出的一次左移
	}

	if (klen%v == 0)	//klen/v为整数
	{
		//同上算法
		sftbit(Z, 32, t);						//左移32位
		add(t, ct, t);							//结合后的数

		big sm3Value = SM3(t);
		copy(sm3Value, V0);						//求t的hash值 t= Z||ct
		mirkill(sm3Value);

		sftbit(Ha, 256, Ha);
		add(V0, Ha, Ha);						//哈希后的数和之前的数相加

	}
	else
	{
		sftbit(Z, 32, t);						//左移32位
		add(t, ct, t);							//结合后的数

        //printf("V0-1=%s\n", BigToHexChars2(V0));
        //printf("SM3(t)=%s\n", BigToHexChars2(SM3(t)));
		copy(SM3(t), V0);						//哈希后的数，保存到V0中,256位
        //printf("SM3(t)=%s\n", BigToHexChars2(SM3(t)));
        //printf("V0-2=%s\n", BigToHexChars2(V0));
		sftbit(V0, -(256 - (klen * 8 - (klen / v)*v * 8)), V0);
        //printf("V0-3=%s\n", BigToHexChars2(V0));
		sftbit(Ha, klen * 8 - (klen / v)*v * 8, Ha);
		add(Ha, V0, Ha);							//哈希后的数和之前的数相加
	}
	mirkill(V0);
	mirkill(x);
	mirkill(y);
	mirkill(t);
	mirkill(ct);
	mirkill(Z);
	return Ha;
}

/********************************
计算C3，以十六进制串表示，占32字节--保留
注释：使用了全局变量随机数k
********************************/
char* CalculateC3()
{
	epoint *point2 = CalculatePoint2();
	char* x2 = (char*)malloc(sizeof(char)*Max);
	char* y2 = (char*)malloc(sizeof(char)*Max);
	int lengthX = big_to_bytes(0, PointX(point2), x2, FALSE);  //字节个数
	int lengthY = big_to_bytes(0, PointY(point2), y2, FALSE);  //字节个数
	epoint_free(point2);//暂存变量被释放
	/*拼接十六进制串*/
	char* xmy = (char*)malloc(sizeof(char)*(lengthX*2 + lengthY*2 + inputData.size * 2 + 1));  //十六进制个数
	int i = 0;
	for (int j = 0; j < lengthX; j++)
	{
		sprintf(&xmy[i], "%02x", (unsigned char)(x2[j]));  //以16进制的格式输出unsigned char类型的数值,输出域宽为2,右对齐,不足的用字符0替代
		i += 2;
	}
	for (int j = 0; j < inputData.size; j++)
	{
		sprintf(&xmy[i], "%02x", (unsigned char)(inputData.data[j]));
		i += 2;
	}
	for (int j = 0; j < lengthY; j++)
	{
		sprintf(&xmy[i], "%02x", (unsigned char)(y2[j]));
		i += 2;
	}
	xmy[i] = '\0';   //直接赋值，赋值时不包含\0
	free(x2);
	free(y2);
	xmy = SM3ByHexStr(xmy);  //SM3(x2||data||y2)，以十六进制串表示
	return xmy;
}

/*
计算Z
*/
 big CalculateZ(big id, big px, big py)
 {
     /*拼接十六进制串*/
     char *c = (char*)calloc(strlen(BigToHexChars2(id)) + strlen(a) + strlen(b) + strlen(Gx) + strlen(Gy) + strlen(BigToHexChars2(px)) + strlen(BigToHexChars2(py)) +16 + 1, sizeof(char));  //完整十六进制串分配内存
     strcat(c, "0x0100");
     strcat(c, BigToHexChars2(id));
     strcat(c, a);
     strcat(c, b);
     strcat(c, Gx);
     strcat(c, Gy);
     strcat(c, BigToHexChars2(px));
     strcat(c, BigToHexChars2(py));
     big Z = mirvar(0);
     Z = HexCharsToBig(SM3ByHexStr(c));
     //printf("Z = %s\n", BigToHexChars2(Z));
     return Z;
 }



