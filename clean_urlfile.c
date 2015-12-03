/*************************************************************************
    > File Name: clean_urlfile.c
    > Author: lvchao0428
    > Mail: 410148119@qq.com 
    > Created Time: Tue Dec  1 10:55:53 2015
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include"savehtml.h"

int clean_url(UrlBuf** ub)
{
   UrlBuf* dummy = (UrlBuf*)malloc(sizeof(UrlBuf));
   bzero(dummy, sizeof(*dummy));
   
   dummy->next = *ub;
   UrlBuf* p = dummy;

   while(p)
   {
	  if(!strstr(p->next->str, "http"))
	  {
		 UrlBuf* q = p->next;
		 p->next = p->next->next;

		 free(q->str);
		 free(q);
	  }
	  else
	  {
		 p = p->next;
	  }
   }

   *ub = dummy->next;
}

int main(int argc, char* argv[])
{
   UrlBuf* ub = NULL;
   read_urls_to_UrlBuf(&ub, "suculswave");
   
   write_urls_to_UrlBuf(ub, "cleansuculswave");

   return 0;
}
