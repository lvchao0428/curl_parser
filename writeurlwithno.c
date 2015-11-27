/*************************************************************************
    > File Name: writeurlwithno.c
    > Author: lvchao0428
    > Mail: 410148119@qq.com 
    > Created Time: Thu Nov 26 11:10:15 2015
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"def.h"

void read_file(UrlBuf** ub, char* fileurl)
{
   FILE* fp;
   char* line = NULL;
   size_t len = 0;
   ssize_t read;
   UrlBuf* p, * temp;
   //p = temp = *ub;
   fp = fopen(fileurl, "r");
   if(!fp)
   {
	  printf("cannot open the fileurl\n");
	  exit(EXIT_FAILURE);
   }
   int lineno = 1;
   while((read = getline(&line, &len, fp)) != -1)
   {
	  if(*ub == NULL)
	  {
		 *ub = (UrlBuf*)malloc(sizeof(UrlBuf));
		 memset(*ub, 0, sizeof(UrlBuf));
		 (*ub)->lineno = lineno;		 
		 (*ub)->str = (char*)malloc(sizeof(char)*(strlen(line) + 1));
		 strcpy((*ub)->str, line);
		 p = temp = *ub;
	  }
	  else
	  {
		 UrlBuf* q = (UrlBuf*)malloc(sizeof(UrlBuf));
		 memset(q, 0, sizeof(*q));
		 q->str = (char*)malloc(sizeof(char)*(strlen(line) + 1));
		 q->lineno = lineno;
		 strcat(q->str, line);
		 p->next = q;
		 p = p->next;
	  }
	  
	  lineno++;
   }
   *ub = temp; 
   free(line);
   fclose(fp); 

}

void write_to_urlfile_with_no(UrlBuf* ub, char* urlfilewithno)
{
   FILE* fp;
	  
   fp = fopen(urlfilewithno, "w");
   if(!fp)
   {
	  printf("cannot open urlfilewithno");
	  exit(EXIT_FAILURE);
   }

   UrlBuf* p = ub;
   while(p)
   {
	  fprintf(fp, "%d#%s", p->lineno, p->str);
	  p = p->next;
   }

   fclose(fp);
}

int main(int argc, char* argv[])
{
   if(argc < 3)
   {
	  printf("need raw file and new urlfile\n");
	  exit(EXIT_FAILURE);
   }

   UrlBuf* ub;
   read_file(&ub, argv[1]);	
   write_to_urlfile_with_no(ub, argv[2]);

   return 0;
}
