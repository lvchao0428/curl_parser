#ifndef DEF_H_
#define DEF_H_

#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<gumbo.h>

typedef struct UrlBuf
{
   int lineno;
   char* siss; //存储最短可识别特征的字符串
   char* str;
   char* id;
   char* cls;
   struct UrlBuf* next;
}UrlBuf;

typedef struct CommonPart
{
   int* urlIndexs;
   int lineno;
   char* comStr;
   char* id;
   char* cls;
   char* auth;
   char* time;
   UrlBuf* ubList;
   struct CommonPart* next;
}CommonPart;

typedef struct PointerPair
{
   const char* left;
   const char* right;
   struct PointerPair* next;
}PointerPair;

typedef struct TargetPercent
{
   int totalhtmlnum;
   int hassissnum;
   int content_hit_num;
   int auth_hit_num;
   int time_hit_num;
    
}TargetPercent;

#endif
