/*************************************************************************
    > File Name: mergeurl.c
    > Author: lvchao0428
    > Mail: 410148119@qq.com 
    > Created Time: Fri Nov 27 16:28:01 2015
 ************************************************************************/

#include<stdio.h>
#include"savehtml.h"
#include"code_convert.h"



//merge common middle part
void cpy_middle_to_str(UrlBuf* ub, char* str)
{
   int i = 0;
   int beg, end;
   while(ub->str[i] != ':')
   {
	  i++;
   }
   i += 3;//移动到“//”后面第一个字符
   beg = i;
   while(ub->str[i] != '/')
   {
	  i++;
   }
   end = i;
   strncpy(str, ub->str + beg, end - beg);
}

void push_CommonPart(CommonPart** cp, char* comstr)
{
   CommonPart* head = *cp;
   CommonPart* p = head;
   if(*cp == NULL)
   {
	  *cp = (CommonPart*)malloc(sizeof(CommonPart));

	  (*cp)->next = NULL;
   }
   else
   {
	  while(p->next)
	  {
		 p = p->next;
	  }
	  CommonPart* q = (CommonPart*)malloc(sizeof(CommonPart));
	  q->next = NULL;
	  q->comStr = (char*)malloc(sizeof(char)*(strlen(comstr) + 1));
	  memcpy(q->comStr, comstr);
	  p->next = q;

   }

   *cp = head;

}

void del_dupUb(UrlBuf* head, CommonPart* cp)
{
   
}

void extract_mergePart(UrlBuf* ub, CommonPart** cp)
{
   UrlBuf* dummyUb = (UrlBuf*)malloc(sizeof(UrlBuf));
   bzero(dummyUb, sizeof(UrlBuf));
   dummyUb->next = ub;
   UrlBuf* p = dummyUb->next;
   UrlBuf* p2;
   while(p)
   {
	  p2 = p;
	  if(p->next)
	  {
		 p2 = p->next;
		 while(p2)
		 {
			p2 = p2->next;
			char part1[100];
			bzero(part1, sizeof(part1));
			char part2[100];
			bzero(part2, sizeof(part2));
			//遍历找到第一个有相同中缀的网址对
			cpy_middle_to_str(p->str, part1);
			cpy_middle_to_str(p2->str, part2);
			char* comstr;
			comstr = lcs(part1, part2);		//free later
			if(strlen(comstr) < 6)
			{
			   if(p2)
			   {
				  p2 = p2->next;
			   }
			   else
			   {//
				  push_CommonPart(cp, comstr);
			   }
			   continue;
			}
			else
			{
			   p2 = p2->next;
			   continue;
			}
			
		 }

		 
	  }

	  p = p->next;
   }
}


int main(int argc, char* argv[])
{
   UrlBuf* ub;
   
   read_urls_to_UrlBuf(&ub, "sucubware");

   
   
   return 0;
}
