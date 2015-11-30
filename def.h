#ifndef DEF_H_
#define DEF_H_

#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>

typedef struct UrlBuf
{
   int lineno;
   char* siss; //存储最短可识别特征的字符串
   char* str;
   struct UrlBuf* next;
}UrlBuf;

typedef struct CommonPart
{
   int* urlIndexs;
   char* comStr;
   struct CommonPart* next;
}CommonPart;

#endif
