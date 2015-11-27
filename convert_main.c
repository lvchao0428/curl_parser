/*************************************************************************
    > File Name: convert_main.c
    > Author: lvchao0428
    > Mail: 410148119@qq.com 
    > Created Time: Fri Nov 27 10:29:45 2015
 ************************************************************************/

#include<stdio.h>
#include"def.h"
#include"savehtml.h"
#include"code_convert.h"

int main(int argc, char* argv[])
{
   UrlBuf* ub = NULL;
   UrlBuf* failub = NULL;	 
   UrlBuf* sucub = NULL;
   read_urls_to_UrlBuf(&ub, "newurls");

   convert_all_files_to_UTF8(ub, &failub, &sucub);

   write_urls_to_UrlBuf(failub, "failuls");
   write_urls_to_UrlBuf(sucub, "suculs");
   printf("begin write failub\n");
   print_UrlBuf(failub);

   return 0;
}
