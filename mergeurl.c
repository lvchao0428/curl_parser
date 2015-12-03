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


/*
 *ret:0 fail del or null string.
 *ret:1 success del
 * */
int del_dupUb(UrlBuf** ub, char* siss)
{
   UrlBuf* dummy = (UrlBuf*)malloc(sizeof(UrlBuf));
   bzero(dummy, sizeof(*dummy));
   dummy->next = *ub;
   UrlBuf* headdummy = dummy;
   //with dummy
   if(dummy == NULL || dummy->next == NULL)
   {
	  return 0;
   }
   
   UrlBuf* p = dummy;
   int sampleNum = 0;
   while(p && p->next)
   {
	  if(strstr(p->next->str, siss))
	  {
		 UrlBuf* temp = p->next;
		 
		 p->next = p->next->next;
		 //	insert 3 sample url
		 free(temp->str);
		 free(temp);
		 sampleNum++;
	  }
	  else
	  {
		 if(p && p->next)
			p = p->next;
	  }
   }
   *ub = headdummy->next;	//赋予新的头指针	 

   return 1;
}

void find_common_part(UrlBuf* head, char* siss)
{
   
}

void dedot(char* str)
{
   int i;
   for(i = 0; i < strlen(str); ++i)
   {
	  str[i] = str[i+1];
   }
   str[i] = '\0';
}

void extract_mergePart(UrlBuf** ub, CommonPart** sList)
{
   //UrlBuf* dummyUb = (UrlBuf*)malloc(sizeof(UrlBuf));
   //bzero(dummyUb, sizeof(UrlBuf));
   //dummyUb->next = *ub;
   UrlBuf* p = *ub;
   UrlBuf* p2 = NULL;
   int flag = 1;
   int singleflag = 1; 
   while(p)
   {
	  singleflag = 1;
	  char part1[100];
	  bzero(part1, sizeof(part1));
	  int ret1 = cpy_middle_to_str(p, part1);
	  //printf("check p:str:%s\n", p->str);
	  if(!ret1)
	  {//非法的url
		 p = p->next;
		 //printf("illegal p lineno:%d\n", p->lineno);
		 continue;
	  }
	  if(p->next)
	  {
		 p2 = p->next;
		 //printf("p2 check p2:str:%s\n", p2->str);
		 while(p2)
		 {
				
			char part2[100];
			bzero(part2, sizeof(part2));
				
			//printf("p2 check\n");
			//遍历找到第一个有相同中缀的网址对
			//printf("part1:%s\n", part1);
			int ret2 = cpy_middle_to_str(p2, part2);
			//printf("part2:%s\n", part2);
			//printf("p->lineno:%d, str:%s\n", p->lineno, p->str);
			//printf("p2->lineno:%d, str:%s\n", p2->lineno, p2->str);
			if(!ret2)
			{
			   if(p2)
			   {//url不合法但是存在
				  p2 = p2->next;
				  continue;
			   }
			   else
			   {//此行为空
				  break;
			   }
			}
			char* comstr = (char*)malloc(sizeof(char)*100);
			strcpy(comstr ,lcs(part1, part2));		//free later
			//de dot
			if(strlen(comstr) < 10 || (!strstr(comstr, ".com") && 
					 !strstr(comstr, ".cn") && 
					 !strstr(comstr, ".net")))
			{//没有找到合适的公共子串
			   if(p2)
			   {
				  p2 = p2->next;
			   }
			   else
			   {
				  break;
			   }
			   free(comstr);
			   continue;
			}
			else
			{

			   //printf("plineno:%d:str:%s,\np2:lineno:%dstr:%s\n", p->lineno, p->str, p2->lineno,p2->str);
			   //找到符合条件的公共字符串
			   CommonPart tempcp;
			   bzero(&tempcp, sizeof(tempcp));
			   tempcp.comStr = (char*)malloc(sizeof(char)*(strlen(comstr) + 1));
			   strcpy(tempcp.comStr, comstr);
			   push_CommonPart(sList, tempcp);
			  //ret
			   //printf("retp:%s", retp->comStr);
			   del_dupUb(ub, comstr);	//删除原urlbuf中的符合的url。剩下的接着处理，直到提取出所有的url特征				//may be del all the element
			   free(tempcp.comStr); 
			   //printf("")
			   p = (*ub);		//在从头开始遍历，直到每个节点都找到相应的特征对
			   //printf("p:p->lineno:%d, p->str:%s\n", p->lineno, p->str);
			   //printf("p2:p->lineno:%d, p->str:%s\n", p->next->lineno, p->next->str);
			   //printf("p2:p->lineno:%d, p->str:%s\n", p->next->next->lineno, p->next->next->str);
			   //printf("part1:%s, part2:%s\n", part1, part2);
				 printf("compart:%s\n", comstr);
			   flag++;
			  // printf("flag:%d\n", flag);
			   singleflag = 0;		
			   break;
			}
			free(comstr);
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
		 del_dupUb(ub, tempcomStr);
		 p = (*ub);
		 printf("compart:%s\n", tempcp.comStr);
		 free(tempcp.comStr);
		 //最后一个特征录入完毕，跳出ub循环
		 break;
	  }
	  else if(!p)
	  {
		 break;
	  }
	  if(singleflag == 1)
	  {//此p指向的url为单个的url无匹配，所以需要单独存储为特征
		 CommonPart tempcp;
		 bzero(&tempcp, sizeof(tempcp));
		 char tempcomStr[100];
		 bzero(tempcomStr, sizeof(tempcomStr));
		 cpy_middle_to_str(p, tempcomStr);
		 tempcp.comStr = (char*)malloc(sizeof(char)*(strlen(tempcomStr) + 1));
		 strcpy(tempcp.comStr, tempcomStr);
		 push_CommonPart(sList, tempcp);

		 del_dupUb(ub, tempcomStr);
		 p = (*ub);
		 printf("sing compart:%s\n", tempcomStr);
		 free(tempcp.comStr);
		 //p = p->next;  //继续遍历下一个
	  }
	  //p = p->next;
   }
}


void insert_url_with_same_siss(UrlBuf** ub1, CommonPart* sList)
{
   UrlBuf* p = (*ub1);
   UrlBuf* head; 
   CommonPart* slp = sList;
   while(slp)
   {
	  p = (*ub1);
	  head = p;
	  while(p)
	  {
		 if(strstr(p->str, slp->comStr))
		 {
			push_Url(&slp->ubList, p);
		 }
		 p = p->next;
	  }
	  *ub1 = head;
	  //del_dupUb(ub1, sList->comStr);

	  slp = slp->next;
   }
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
   read_urls_to_UrlBuf(&ub, "cleansuculswave");
   UrlBuf* ub1 = NULL;
   read_urls_to_UrlBuf(&ub1, "cleansuculswave");
  // print_UrlBuf(ub);
   

   //test_copy_middle(ub1, ub2);
   
   extract_mergePart(&ub, &sList);
   write_urls_to_UrlBuf(ub, "firstDelBuf");
   write_commonpart_to_file(sList, "compartFile");
   //select 3 url for per slist;
   insert_url_with_same_siss(&ub1, sList);
   printf("insert complete\n");
   write_commonpart_and_url_to_file(sList, "threeCommonUrl");
   return 0;
}
