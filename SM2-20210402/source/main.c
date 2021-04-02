#include<stdio.h>
#include "arithmetic.h"

int main(int argc,char **argv){

	mip= mirsys(Max, 16);
	ecurve_init(HexCharsToBig(a), HexCharsToBig(b), HexCharsToBig(p), MR_PROJECTIVE);	//初始化椭圆曲线内部参数

	char* input = (char*)malloc(sizeof(char) * 2048);
	input ="the test";
	FILE *fp;
	char* pk = (char*)malloc(sizeof(char) * 2048);
	/* 打开用于读取的文件 */
	fp = fopen("/home/pi/yjx/SM2/xxx.txt" , "r");
	if(fp == NULL) {
	     perror("打开文件时发生错误");
	     return(-1);
	}
	if(fgets (pk, 129, fp)!=NULL ) {

	}
	fclose(fp);
	char* cypher = myEncryption(input,pk);//传入参数加密
	printf("密文为:%s\n\n",cypher);


		FILE *ffp;
		char* free = (char*)malloc(sizeof(char) * 2048);
		/* 打开用于读取的文件 */
		ffp = fopen("/home/pi/yjx/SM2/xxx.txt" , "r");
		if(ffp == NULL) {
			perror("打开文件时发生错误");
			return(-1);
	    }
		if( fgets (free, 129, ffp) == NULL ) {
			printf("fgets error\n");
			return 0;
		}
		char* xx = (char*)malloc(sizeof(char) * 2048);
		if (fgets(xx, 5, fp) == NULL)
		{
			printf("fgets error\n");
			return 0;
		}
		char* dk = (char*)malloc(sizeof(char) * 2048);
		if (fgets(dk, 65, fp) == NULL)
		{
		    printf("fgets error\n");
		    return 0;
		}
		fclose(ffp);
		char* caption = myDecryption(cypher, dk);
		printf("明文为:%s\n\n",caption);
	return 0;
}



