#ifndef _CODE_CONVERT_H_
#define _CODE_CONVERT_H_
   
#include<stdio.h>
#include<iconv.h>

int code_type_get(char* dest, char* type);


int code_convert(char* from_charset, char* to_charset, char* inbuf, size_t inlen, char* outbuf, size_t outlen);

#endif
