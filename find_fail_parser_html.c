/*************************************************************************
    > File Name: find_fail_parser_html.c
    > Author: lvchao0428
    > Mail: 410148119@qq.com 
    > Created Time: Fri Nov 27 09:13:19 2015
 ************************************************************************/

#include<stdio.h>
#include"def.h"
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include"savehtml.h"



void if_parsed_fail(UrlBuf** ub)
{
   //read whole urls
   UrlBuf* wholeUb = NULL;
   int returl = read_urls_to_UrlBuf(&wholeUb, "newurls");
   if(returl != 1)
   {
	  printf("read url failed\n");
	  exit(0);
   }
   UrlBuf* p = wholeUb;
   UrlBuf* tempUb,* ubp;
   char* dest = NULL;
   while(p)
   {
	  dest = NULL;
	  int ret;
	  char filename[40];
	  bzero(filename, sizeof(filename));
	  sprintf(filename,"./kejihtmls/%d.html", p->lineno);
	  ret = file_read_full(&dest, filename);
	  if(ret == -1 || ret == -2)
	  {
		 printf("str:%s\n", dest);
		 if((*ub) == NULL)
		 {
			*ub = (UrlBuf*)malloc(sizeof(UrlBuf));
			(*ub)->next = NULL;
			(*ub)->str = (char*)malloc(sizeof(char)*(strlen(p->str) + 1));
			memcpy((*ub)->str, p->str, sizeof(char)*(strlen(p->str) + 1));
			(*ub)->lineno = p->lineno;
			tempUb = ubp = (*ub);
		 }
		 else
		 {
			UrlBuf* q = (UrlBuf*)malloc(sizeof(UrlBuf));
			q->next = NULL;
			q->str = (char*)malloc(sizeof(char)*(strlen(p->str) + 1));
			memcpy(q->str, p->str, strlen(p->str) + 1);
			q->lineno = p->lineno;
			ubp->next = q;
			ubp = ubp->next;
		 }
		 printf("%d.html failed\n", p->lineno);
	  }//file not exist
	  else
	  {
	  //file not read whole
		 char* tempc = strstr(p->str, "</html>");
		 if(tempc == NULL)
		 {
			if((*ub) == NULL)
			{
			   *ub = (UrlBuf*)malloc(sizeof(UrlBuf));
			   (*ub)->next = NULL;
			   (*ub)->str = (char*)malloc(sizeof(char)*(strlen(p->str) + 1));
			   memcpy((*ub)->str, p->str, sizeof(char)*(strlen(p->str) + 1));
			   (*ub)->lineno = p->lineno;
			   tempUb = ubp = (*ub);
			}
			else
			{
			   UrlBuf* q = (UrlBuf*)malloc(sizeof(UrlBuf));
			   q->next = NULL;
			   q->str = (char*)malloc(sizeof(char)*(strlen(p->str) + 1));
			   memcpy(q->str, p->str, strlen(p->str) + 1);
			   q->lineno = p->lineno;
			   ubp->next = q;
			   ubp = ubp->next;
			}
		 }
		 printf("%d.html failed\n", p->lineno);
	  }
	  //printf("%d.html checked over...\n", p->lineno);
	  free(dest);
	  p = p->next;
   }
   *ub = tempUb;
   //write the fail url to file
   write_urls_to_UrlBuf(tempUb, "failedUrls");

   
}



int main(int argc, char* argv[])
{
   UrlBuf* ub = NULL;

  // if_parsed_fail(&ub);
   read_urls_to_UrlBuf(&ub, "newurls");
   print_UrlBuf(ub);
   return 0;
}
