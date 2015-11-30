/*************************************************************************
    > File Name: mergeurl.c
    > Author: lvchao0428
    > Mail: 410148119@qq.com 
    > Created Time: Fri Nov 27 16:28:01 2015
 ************************************************************************/

#include<stdio.h>
#include"savehtml.h"
#include"code_convert.h"
#include<string.h>
#include"mystring.h"
//编写合并初始合并url方案的函数

//merge common middle part
int cpy_middle_to_str(UrlBuf* ub, char* dest)
{
   int i = 0;
   int beg, end;
   if(!ub || !ub->str|| strlen(ub->str) < 5 || \
		 ub->str[0] == '\0')
   {
	  return 0;
   }

   if(!strstr(ub->str, "http"))
   {
	  return 0;			//不是合法的url
   }
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
   if(strncpy(dest, ub->str + beg, end - beg))
   {
	  return 1;
   }
   else
   {
	  return 0;
   }
}

int push_CommonPart(CommonPart** cp, CommonPart src)
{
   CommonPart* head = *cp;
   CommonPart* p = head;
   if(*cp == NULL)
   {
	  *cp = (CommonPart*)malloc(sizeof(CommonPart));
	  (*cp)->comStr = (char*)malloc(sizeof(char)*(strlen(src.comStr) + 1));
	  memcpy((*cp)->comStr, src.comStr, sizeof(char)*(strlen(src.comStr) + 1));
	  (*cp)->next = NULL;
   }
   else
   {
	  while(p->next)
	  {
		 p = p->next;
	  }
	  CommonPart* q = (CommonPart*)malloc(sizeof(CommonPart));
	  bzero(q, sizeof(*q));
	  q->comStr = (char*)malloc(sizeof(char)*(strlen(src.comStr) + 1));
	  memcpy(q->comStr, src.comStr, sizeof(char)*(strlen(src.comStr) + 1));
	  p->next = q;

   }

   *cp = head;

}


/*
 *ret:0 fail del or null string.
 *ret:1 success del
 * */
int del_dupUb(UrlBuf* dummy, char* siss)
{
   //with dummy
   if(dummy == NULL || dummy->next == NULL)
   {
	  return 0;
   }

   UrlBuf* p = dummy;
   while(p && p->next)
   {
	  if(strstr(p->next->str, siss))
	  {
		 UrlBuf* temp = p->next;
		 
		 p->next = p->next->next;

		 free(temp);
	  }
	  else
	  {
		 if(p->next)
			p = p->next;
		 else
			break;
	  }
   }


   return 1;
}

void extract_mergePart(UrlBuf** ub, CommonPart** sList)
{
   UrlBuf* dummyUb = (UrlBuf*)malloc(sizeof(UrlBuf));
   bzero(dummyUb, sizeof(UrlBuf));
   dummyUb->next = *ub;
   UrlBuf* p = dummyUb->next;
   UrlBuf* p2;
   while(p)
   {
	  if(p && p->next)
	  {
		 p2 = p->next;
		 while(p2)
		 {
			if(!p2)
			   break;
			char part1[100];
			bzero(part1, sizeof(part1));
			char part2[100];
			bzero(part2, sizeof(part2));
			

			//遍历找到第一个有相同中缀的网址对
			int ret1 = cpy_middle_to_str(p, part1);
			int ret2 = cpy_middle_to_str(p2, part2);
		//	printf("p->lineno:%d, str:%s\n", p->lineno, p->str);
			printf("p2->lineno:%d, str:%s\n", p2->lineno, p2->str);
			if(!ret1 || !ret2)
			{
			   if(!ret1)
				  p = p->next;
			   if(!ret2)
				  p2 = p2->next;
			   continue;
			}
			char* comstr = (char*)malloc(sizeof(char)*50);
			strcpy(comstr ,lcs(part1, part2));		//free later
			if(strlen(comstr) < 6)
			{//没有找到合适的公共子串
			   if(p2)
			   {
				  p2 = p2->next;
			   }
			   else
			   {
				  break;
			   }
			   continue;
			}
			else
			{
			   //找到符合条件的公共字符串
			   del_dupUb(dummyUb, comstr);	//删除原urlbuf中的符合的url。剩下的接着处理，直到提取出所有的url特征				//may be del all the element
			   CommonPart tempcp;
			   bzero(&tempcp, sizeof(tempcp));
			   tempcp.comStr = (char*)malloc(sizeof(char)*(strlen(comstr) + 1));
			   strcpy(tempcp.comStr, comstr);
			   push_CommonPart(sList, tempcp);
			   
			   p = dummyUb->next;
			   printf("compart:%s\n", comstr);
			   continue;
			}
			p2 = p2->next;
		 }
	  }//end if 
	  else if(!p->next)
	  {//只剩下一个节点的时候自称一类
		 //如果只有一个节点，则认为无公共子串，然后直接url直接作为网页自己的siss
		 CommonPart tempcp;
		 bzero(&tempcp, sizeof(tempcp));
		 char tempcomStr[100];
		 bzero(tempcomStr, sizeof(tempcomStr));
		 cpy_middle_to_str(p, tempcomStr);
		 tempcp.comStr = (char*)malloc(sizeof(char)*(strlen(tempcomStr) + 1));
		 strcpy(tempcp.comStr, tempcomStr);
		 push_CommonPart(sList, tempcp); 

		 printf("compart:%s\n", tempcp.comStr);
		 //最后一个特征录入完毕，跳出ub循环
		 break;
	  }
	  else if(!p)
	  {
		 break;
	  }

	  //p = p->next;
   }
}

int write_commonpart_to_file(CommonPart* cp, char* filename)
{
   FILE* fp;
   CommonPart* p = cp;
   fp = fopen(filename, "w");
   if(!fp)
   {
	  printf("open write file %s failed\n", filename);
	  return -1;
   
   }


   while(p)
   {
	  //存入url特征
	  fprintf(fp, "%s\n", p->comStr);
	  p = p->next;
   }

   fclose(fp);
}

void test_copy_middle(UrlBuf* ub1, UrlBuf* ub2)
{
   char part1[100];
   bzero(part1, sizeof(part1));
   char part2[100];
   bzero(part2, sizeof(part2));

   cpy_middle_to_str(ub1, part1);
   cpy_middle_to_str(ub2, part2);
   
   printf("mid1:%s\n", part1);
   printf("mid2:%s\n", part2);
   //test lcs
   printf("lcs:%s\n", lcs(part1, part2));
}

int main(int argc, char* argv[])
{
   UrlBuf* ub = NULL;
   CommonPart* sList = NULL;
   read_urls_to_UrlBuf(&ub, "suculswave");

  // print_UrlBuf(ub);
   
   UrlBuf* ub1,* ub2;
   ub1 = ub;
   ub2 = ub->next;

   //test_copy_middle(ub1, ub2);
   
   extract_mergePart(&ub, &sList);
   write_commonpart_to_file(sList, "compartFile");
   
   
   return 0;
}
