/*************************************************************************
    > File Name: test_url.c
    > Author: lvchao0428
    > Mail: 410148119@qq.com 
    > Created Time: Fri Nov 27 11:22:15 2015
 ************************************************************************/

#include<stdio.h>
#include"def.h"
#include"savehtml.h"
#include"code_convert.h"

void push_three_url(UrlBuf** ub)
{
   int i;
   for(i = 0; i < 3; ++i)
   {
	  char word[10];
	  scanf("%s", word);
	  UrlBuf* q = (UrlBuf*)malloc(sizeof(UrlBuf));
	  q->next = NULL;
	  q->str = (char*)malloc(sizeof(char)*(strlen(word) + 1));
	  memcpy(q->str, word, sizeof(char)*(strlen(word) + 1));
	  q->lineno = i;

	  push_Url(ub, q);

   }
}

int main(int argc, char* argv[])
{
   UrlBuf* ub;

   push_three_url(&ub);

   print_UrlBuf(ub);


   return 0;
}
