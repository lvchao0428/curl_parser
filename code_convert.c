/*************************************************************************
    > File Name: code_convert.c
    > Author: lvchao0428
    > Mail: 410148119@qq.com 
    > Created Time: Mon Nov  2 14:22:17 2015
 ************************************************************************/

#include<stdio.h>
#include<string.h>

#include"code_convert.h"
char* mystrstri(char* str, char* subStr)
{
   int len = strlen(subStr);
   if(len == 0)
   {
	  return NULL;
   }

   while(*str)
   {
	  if(strncasecmp(str, subStr, len) == 0)
	  {
		 return str;
	  }
	  ++str;
   }

   return NULL;
}

int code_type_get(char* dest, char* type)
{
   int i = 0;
   char* begpos;
   begpos = mystrstri(dest, "charset");
   if(begpos == NULL)
   {
	  *type = '\0';
	  return 0;
   }
   while(*begpos != '=' && *begpos != '"')
   {
	  begpos++;
   }

   if(*begpos == '=')
   {
	  if(*(begpos+1) == '"')
	  {
		 begpos += 2;
	  }
	  else
	  {
		 begpos++;
	  }
   }

   while(*begpos != '"')
   {
	  *type++ = *begpos++;
   }

   type[0] = '\0';
}



int code_convert(char* from_charset, char* to_charset, char* inbuf, size_t inlen, char* outbuf, size_t outlen)
{
   iconv_t cd;
   int rc;
   char** pin = &inbuf;
   char** pout = &outbuf;
   cd = iconv_open(to_charset, from_charset);
   if(cd == 0)
	  return -1;
   memset(outbuf, '\0', outlen);
   //bzero(outbuf, outlen);
   if(iconv(cd, pin, &inlen, pout, &outlen) == -1)
	  return -1;
   iconv_close(cd);
   return 0;
}


