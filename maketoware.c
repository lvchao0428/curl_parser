/*************************************************************************
    > File Name: maketoware.c
    > Author: lvchao0428
    > Mail: 410148119@qq.com 
    > Created Time: Fri Nov 27 16:09:42 2015
 ************************************************************************/

#include<stdio.h>
#include"def.h"
#include"savehtml.h"
#include"code_convert.h"

//把分隔符转换成波浪线

int main(int argc, char* argv[])
{
   UrlBuf* ub = NULL;

   read_urls_to_UrlBuf(&ub, "suculs");
   write_urls_to_UrlBuf(ub, "suculswave");

   return 0;
}
