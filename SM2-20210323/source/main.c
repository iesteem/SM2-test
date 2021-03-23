#include<stdio.h>
#include"arithmetic.h"

int main(int argc,char **argv){

	mip= mirsys(Max, 16);
	ecurve_init(HexCharsToBig(a), HexCharsToBig(b), HexCharsToBig(p), MR_PROJECTIVE);	//初始化椭圆曲线内部参数

	    char* input = (char*)malloc(sizeof(char) * 2048);
	    input ="the test";

	    FILE *fp;
	    char* dk = (char*)malloc(sizeof(char) * 2048);

	      /* 打开用于读取的文件 */
	      fp = fopen("/home/pi/yjx/SM2/xxx.txt" , "r");
	      if(fp == NULL) {
	         perror("打开文件时发生错误");
	         return(-1);
	      }
	      if( fgets (dk, 128, fp)!=NULL ) {
	         /* 向标准输出 stdout 写入内容 */
	         puts(dk);
	      }
	      fclose(fp);

		char* cypher = myEncryption(input,dk);//传入参数加密
		printf("密文为:%s\n\n",cypher);
	return 0;
}



