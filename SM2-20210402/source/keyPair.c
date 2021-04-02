#include "miracl.h"
#include "mirdef.h"
#include "operation.h"
#include "sm2.h"


/***********************************
产生随机数 a<= result <=b
***********************************/
big GetBigRandom(big a, big b)
{
	irand((unsigned)time(NULL));
	big result = mirvar(0);
	bigrand(Add2(Sub2(b, a), mirvar(1)), result);		// 0<= result <b-a+1
	return Add2(result, a);							// a<= xxx <=b
}

/***********************************
产生随机参数K
注释：所得随机数存入全局变量k
***********************************/
void InitRandomK()
{
    k = mirvar(0);
	copy(GetBigRandom(mirvar(1), Sub2(HexCharsToBig(n), mirvar(1))), k);	// 1<= k <= n-1
	//printf("k  =%s\n", BigToHexChars2(k));
}

/***********************************
产生公私钥
***********************************/
keyPair* CalculateKeys()
{
	keyPair *pdK = (keyPair*)malloc(sizeof(keyPair));
    big dm = mirvar(0);
    dm = GetBigRandom(mirvar(1), Sub2(HexCharsToBig(n), mirvar(1)));// 私钥	[1,n-1]
    epoint* pm = epoint_init();
    pm = MultiplyEpoint(dm, CalculateG());// 公钥
    pdK->Px = PointX(pm);
    pdK->Py = PointY(pm);
    pdK->d = dm;

/*
    FILE *fp = NULL;
    fp = fopen("/home/pi/yjx/SM2/xxx.txt", "w+");
    fputs(BigToHexChars2(pdK->Px), fp);
    fputs(BigToHexChars2(pdK->Py), fp);
    fputs("\n",fp);
    fputs(BigToHexChars2(pdK->d), fp);
    fclose(fp);
*/
    return pdK;

}

/************************************
验证公钥和私钥
注释：使用全局变量进行运算
************************************/
int VerifyKeys(big x,big y)
{
	if (!mr_compare(Mod2(Pow2(y, 2), HexCharsToBig(p)), Mod2(Add2(Pow2(x, 3), Add2(Multiply2(x, HexCharsToBig(a)), HexCharsToBig(b))), HexCharsToBig(p))))		// if((PBy^2 %p) != ((PBx^3 + a*PBx +b)%p))
	{
		printf("公钥验证无效，请重启程序\n");
		system("pause");
		exit(1);
	}
    printf("公私钥验证成功\n");
	return 1;
}

/***************************
公钥点(Px,Py)
***************************/
epoint *CalculatePublicKey(big Px,big Py)
{

	epoint *pK = (epoint*)malloc(sizeof(epoint));
	pK = epoint_init();
	epoint_set(Px, Py, 0, pK);
	return pK;
}
