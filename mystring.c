/*************************************************************************
    > File Name: mystring.c
    > Author: lvchao0428
    > Mail: 410148119@qq.com 
    > Created Time: Fri Nov 27 17:02:06 2015
 ************************************************************************/

#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

int count_num(char* a)
{
   int i = 0;
   int num = 0;
   while(a[i] != '\0')
   {
	  if(isdigit(a[i]))
	  {
		 num++;
	  }
	  i++;
   }

   return num;
}

//change b to c if b == c
char* replace(char* a, char* b, char * c)
{
   int i = 0;
   while(a[i] != '\0')
   {
	  if(a[i] == b[0])
	  {
		 a[i] = c[0];
	  }
	  i++;
   }
   return a;
}

char* lcs(char* a, char* b)
{
   char** c;
   
   int i, j, k, len1, len2, len3,max, x, y;
   len1 = strlen(a);
   len2 = strlen(b);
   len3 = len1 < len2 ? len1 : len2;

   //初始化c
   c = (char**)malloc(sizeof(char*)*(len1 + 1));
   for(i = 0; i < len1+1; ++i)
   {
	  c[i] = (char*)malloc(sizeof(char)*(len2 + 1));
	  bzero(c[i], sizeof(char)*(len2 + 1));
   }
   max = -1;
   for(i = 1; i < len1 + 1; ++i)
   {
	  for(j = 1; j < len2 + 1; ++j)
	  {
		 if(a[i-1] == b[j-1])
			c[i][j] = c[i-1][j-1] + 1;
		 else
		 {
			c[i][j] = 0;

		 }
		 if(c[i][j] > max)
		 {
			max = c[i][j];
			x = i;
			y = j;
		 }
	  }
   }

   char* dest = (char*)malloc(sizeof(char)*(len3 + 1));
   k = max;
   i = x-1;
   j = y-1;
   dest[k--] = '\0';
   while(i >= 0 && j >= 0)
   {
	  if(a[i] == b[j])
	  {
		 dest[k--] = a[i];
		 i--;
		 j--;
	  }
	  else
	  {
		 break;
	  }
   }
   for(i = 0; i < len1 + 1; ++i)
   {
	  free(c[i]);
   }
   free(c);


   return dest;
}
