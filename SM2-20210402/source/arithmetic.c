#include <stdlib.h>
#include "miracl.h"
#include "mirdef.h"
#include "keyPair.h"
#include "operation.h"
#include "arithmetic.h"

char* myEncryption(char* caption, char* publicKey)
{
	char* data = (char*)malloc(sizeof(char) * Max);  //分配字符串内存
	int dataSize = 0;
	memset(data, '\0', sizeof(data));
	strcpy(data,caption);
	dataSize = strlen(caption);
	inputData.data = data;      //以字符串形式存储明文数据
	inputData.size = dataSize;  //明文中字符的个数

	epoint* pxy = (epoint*)malloc(sizeof(epoint));
	big publicKeyX = mirvar(0);
	big publicKeyY = mirvar(0);
	char *x = GetPartHexStr(publicKey, 0, 64);
	char *y = GetPartHexStr(publicKey, 64, 64);
	publicKeyX = HexCharsToBig(x);
	publicKeyY = HexCharsToBig(y);
	pxy = NewPoint(publicKeyX, publicKeyY);
    otherPK = pxy;
	Encryption();

	char *cc = ccode;
	return cc;
}

char* myDecryption(char* cypher, char* privateKey)
{
	ccode = cypher;
	big dk = mirvar(0);
	dk = HexCharsToBig(privateKey);
    otherKey = (keyPair*)malloc(sizeof(keyPair));

         keyPair xabc = { .Px = mirvar(0),
        		 	 	  .Py = mirvar(0),
         	 	 	 	  .d  = mirvar(0)};
         otherKey = &xabc;

    if(otherKey == NULL)   printf("&otherKey is NULL\n");
	otherKey->d = dk;
	//printf("dk = %s\n\n", BigToHexChars2(otherKey->d));
	Decryption();
	char *caption = outpudata;
	return caption;
}

