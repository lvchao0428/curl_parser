/*************************************************************************
    > File Name: read_feather.c
    > Author: lvchao0428
    > Mail: 410148119@qq.com 
    > Created Time: Tue Dec  1 18:37:13 2015
 ************************************************************************/

#include<stdio.h>
#include"savehtml.h"
#include"code_convert.h"
#include"mystring.h"
#include"read_html.h"

int read_feather_with_siss(CommonPart** sList , char* filename)
{
   FILE* fp;
   fp = fopen(filename, "r");
   char* line = NULL;
   size_t len = 0;
   ssize_t read;
   if(!fp)
   {
	  printf("cannot open the feather file\n");
	  exit(-1);
   }
   
   while((read = getline(&line, &len, fp)) != -1)
   {
	  char tempstr[100];
	  bzero(tempstr, sizeof(tempstr));

	  int i = 0;
	  int j = 0;
	  while(line[i] != '~')
	  {
		 tempstr[j++] = line[i++];
	  }
	  tempstr[j] = '\0';
	  char tempid[50];
	  bzero(tempid, sizeof(tempid));
	  char tempcls[50];
	  bzero(tempcls, sizeof(tempcls));
	  char tempauth[50];
	  bzero(tempauth, sizeof(tempauth));
	  char temptime[50];
	  bzero(temptime, sizeof(temptime));

	  int a = 0;
	  int b = 0;
	  int c = 0;
	  int d = 0;
	  int featPos[4];
	  bzero(featPos, sizeof(featPos));
	  int index = 0;
	  
	  //记录四个特征的位置 id cls auth time
	  while(line[i] != '\n' && line[i] != '\0')
	  {
		 if(line[i] == '~')
		 {
			featPos[index++] = i;
		 }
		 i++;
	  }
	  //id
	  for(index = 0; index < 4; ++index)
	  {
		 i = featPos[index];
		 printf("i:%d\n", i);
		 switch(index)
		 {
			case 0:
			   if(line[i+1] != '~')
			   {
				  i++;	//跳过‘~’
				  while(line[i] != '~')
					 tempid[a++] = line[i++];
				  tempid[a] = '\0';
			   }
			   else
			   {
				  //无id信息

			   }
			   break;
			case 1:
			   if(line[i+1] != '~')
			   {
				  i++;
				  while(line[i] != '~')
					 tempcls[b++] = line[i++];
				  tempcls[b] = '\0';
			   }
			   else
			   {
				  //无cls信息
			   }
			   break;
			case 2:
			   if(line[i+1] != '~')
			   {
				  i++;
				  while(line[i] != '~')
					 tempauth[c++] = line[i++];
				  tempauth[c] = '\0';
			   }
			   else
			   {
				  //无auth信息
			   }
			   break;
			case 3:
			   if(line[i+1] != '\n' && line[i+1] != '\0')
			   {
				  i++;
				  while(line[i] != '~' && line[i] != '\n' && line[i] != '\0')
					 temptime[d++] = line[i++];
				  temptime[d] = '\0';
			   }
			   else
			   {
				  //无time信息
			   }
			   break;
			default:
			   break;
		 }

	  }
	  
	  /*
	  //id 在前 class 在后
	  if(line[i+1] != '~')
	  {//有id
		 i++;
		 while(line[i] != '~' && line[i] != '\0' && line[i] != '\n')
		 {
			tempid[m++] = line[i++];
		 }
		 tempid[m] = '\0';
	  }
	  else
	  {
		 i++;
		 //无id 直接去cls
	  }

	  if(line[i+1] != '\n' && line[i+1] != '\0')
	  {
		 i++;
		 while(line[i] != '~' && line[i] != '\0' && line[i] != '\n')
		 {
			tempcls[n++] = line[i++];
		 }
		 i++;
		 tempcls[n] = '\0';
		 
	  }
	  else
	  {
		 //无特:q
		 //
		 
	  }
	  */
	  CommonPart cp;
	  make_a_commonpart(&cp, tempstr, tempid, tempcls, tempauth, temptime);
	  push_CommonPart(sList, cp);
		 
   }
}

int read_feather(UrlBuf** ub, char* filename)
{
   FILE* fp;
   fp = fopen(filename, "r");
   char* line = NULL;
   size_t len = 0;
   ssize_t read;
   if(!fp)
   {
	  printf("cannot open the feather file\n");
	  exit(-1);
   }
   
   while((read = getline(&line, &len, fp)) != -1)
   {
	  char tempstr[100];
	  bzero(tempstr, sizeof(tempstr));

	  int i = 0;
	  int j = 0;
	  while(line[i] != '~')
	  {
		 tempstr[j++] = line[i++];
	  }
	  tempstr[j] = '\0';
	  char tempid[50];
	  bzero(tempid, sizeof(tempid));
	  char tempcls[50];
	  bzero(tempcls, sizeof(tempcls));
	  i++;
	  int m = 0;
	  int n = 0;
	  if(strstr(line + i, "#"))
	  {//id 在前 class 在后
		 while(line[i] != '#')
		 {
			i++;
		 }
		 i++;
		 while(line[i] != '.' && line[i] != '\0' && line[i] != '\n')
		 {
			tempid[m++] = line[i++];
		 }
		 //printf("id:%s\n", tempid);
		 tempid[m] = '\0';
		 //num count > 5 remove num
		 if(count_num(tempid) > 5)
		 {
			m--;
			while(isdigit(tempid[m]) && m > 0)
			{
			   tempid[m] = '\0';
			   m--;
			}
		 }
		 
	  }
	  else if(strstr(line + i, "."))
	  {
		 while(line[i] != '.')
		 {
			i++;
		 }
		 i++;
		 while(line[i] != '\n' && line[i] != '\0')
		 {
			tempcls[n++] = line[i++];
		 }
		 replace(tempcls, ".", " "); 
		  
		 //printf("cls:%s\n", tempcls);
		 tempcls[n] = '\0';
		 if(count_num(tempcls) > 5)
		 {
			n--;
			while(isdigit(tempcls[n]) && n > 0)
			{
			   tempcls[n] = '\0';
			   n--;
			}
		 }
		 
	  }
	  else
	  {
		 //无特:q
		 //
		 
	  }

	  UrlBuf tempub;
	  make_a_urlbuf(&tempub, tempid, tempcls, tempstr, "");
	  push_Url(ub, &tempub); 
   }
}

void cpy_freath_to_Commonpart(UrlBuf* ub, CommonPart* sList)
{
   UrlBuf* pub = ub;
   CommonPart* psl = sList;

   while(pub)
   {
	  psl = sList;
	  while(psl)
	  {
		 if(strstr(pub->str, psl->comStr))
		 {
			printf("id:%s, class:%s\n", pub->id, pub->cls);
			psl->id = (char*)malloc(sizeof(char)*(strlen(pub->id) + 1));
			memcpy(psl->id, pub->id, sizeof(char)*(strlen(pub->id) + 1));

			psl->cls = (char*)malloc(sizeof(char)*(strlen(pub->cls) + 1));
			memcpy(psl->cls, pub->cls, sizeof(char)*(strlen(pub->cls) + 1));


			break;
		 }
		 else 
			psl = psl->next;
	  }
	   
	  pub = pub->next;
   }
}

int out_siss_with_ub_sList(UrlBuf* mainurl, CommonPart* sList)
{
//   UrlBuf* mainurl
}

/*
 *	ret == 1:正确爬取
 *	ret == -1:无对应特征
 *	ret == -2:无对应网页
 *
*/
int test_ext_answer(UrlBuf* mainurl, CommonPart* sList, int num, 
	  char** content, char** auth, char** time)
{
   UrlBuf* pul = mainurl;
   while(pul)
   {
	  //printf("lineno:%d\n", pul->lineno);
	  if(pul->lineno < num)
	  {
		 pul = pul->next;
		 continue;
	  }
	  else if(pul->lineno > num)
	  {
		 return -2;
	  }
	  else
	  {
		 CommonPart* cpl = sList;
		 while(cpl)
		 {
			if(strstr(pul->str, cpl->comStr))
			{
			   break;
			}
			cpl = cpl->next;
		 }

		 if(cpl == NULL)
		 {
			return -1;
		 }
		 printf("cpl->id:%s, cpl->cls:%s, cpl->auth:%s, cpl->time:%s\n", 
			   cpl->id, cpl->cls, cpl->auth, cpl->time);
		 //deal content;
		 int fileno;
		 fileno = pul->lineno;
		 char filename[100];
		 bzero(filename, sizeof(filename));  
		 //char* content = NULL;
		 //bzero(content, sizeof(content));
		 sprintf(filename, "./kejiUtfhtmls/%d.html", fileno);
		 int ret = fill_content(filename, 
			   content, 
			   auth,
			   time,
			   cpl->id, cpl->cls, cpl->auth, cpl->time);
		 if(ret == -1)
		 {
			return -1;
		 }
		 //free(content);
		 return 1;
	  }

   }
}

void test_ext_content_with_filenum(UrlBuf* mainurl, CommonPart* sList)
{
   int num = 1;
   while(num > 0)
   {
	  printf("input filenum:");
	  char* content = NULL;
	  char* auth = NULL;
	  char* time = NULL;
	  scanf("%d", &num);
	  int ret = test_ext_answer(mainurl, sList, num, &content, &auth, &time);
	  if(ret == -1)
	  {	
		 printf("no patten\n");
		 continue;
	  }
	  else if(ret == -2)
	  {
		 printf("no file exist\n");
		 continue;
	  }
	  else 
	  {
		 printf("content:%s\n", content);
		 printf("auth:%s\n", auth);
		 printf("time:%s\n", time);
		 free(content);
		 free(auth);
		 free(time);
		 printf("ext suc\n");
	  }
   }
}

/*
 *ret 1: 成功抽取
   ret -1:无文件抽取错误
   ret -2:无siss
 * */
int search_content_with_num(UrlBuf* ub, CommonPart* sList, int fileno, 
	  char** content, char** auth, char** time)
{
   UrlBuf* pub = ub;
   CommonPart* psl = sList;
   int ret;
   char filename[50];
   bzero(filename, sizeof(filename));
   sprintf(filename, "./kejiUtfhtmls/%d.html", fileno);

   while(psl)
   {
	  if(strstr(pub->str, psl->comStr))
	  {
		 break;		 
	  }
	  else
	  {
		 psl = psl->next;
	  }
   }
   
   if(psl == NULL)
   {
	  return -2;
   }
   
   ret = fill_content(filename, content, auth, time, psl->id, psl->cls, psl->auth, psl->time);
   if(ret == -1)
   {//找到siss 但是没有抓到正文
	  return -1;
   }
   else 
   {
	  return 1;
   }
}

void test_targetPercent(UrlBuf* mainurl, CommonPart* sList, TargetPercent* tp)
{
   UrlBuf* pub = mainurl;
   while(pub)
   {
	  char* content = NULL;
	  char* auth = NULL;
	  char* time = NULL;
	  int ret = search_content_with_num(pub, sList, pub->lineno, &content, &auth, &time);

	  if(ret == -2)
	  {//无siss
		 tp->totalhtmlnum++;
		 printf("file:%d.html no samplecomplete...\n", pub->lineno);

		 pub = pub->next;
		 if(!pub)
			break;
		 continue;
	  }
	  else if(ret == -1)
	  {//找到siss但是无一命中
		 tp->hassissnum++;
	  }
	  else
	  {
		 if(content && strlen(content) > 0)
		 {
			//printf("content:%s\n", content);
			tp->content_hit_num++;
		 }
		 if(auth && strlen(auth) > 0)
		 {
			tp->auth_hit_num++;
		 }
		 if(time && strlen(time) > 0)
		 {
			tp->time_hit_num++;
		 }

		 tp->hassissnum++;
	  }

	  tp->totalhtmlnum++;
	  printf("file:%d.html complete...\n", pub->lineno);
	  free(content);
	  free(auth);
	  free(time);
	  pub = pub->next;
   }
}

int writePercentToFile(TargetPercent* tp, char* filename)
{
   FILE* fp;
   fp = fopen(filename, "w");

   if(!fp)
   {
	  printf("write file %s error\n", filename);
	  return -1;	  
   }

   fprintf(fp, "total file:%d\n", tp->totalhtmlnum);
   fprintf(fp, "content hit:%d times\n", tp->content_hit_num);
   fprintf(fp, "auth hit:%d times\n", tp->auth_hit_num);
   fprintf(fp, "time hit:%d times\n", tp->time_hit_num);
   fprintf(fp, "content accurate rate:%lf\n", (double)tp->content_hit_num/tp->hassissnum);
   fprintf(fp, "auth accurate rate:%lf\n", (double)tp->auth_hit_num/tp->hassissnum);
   fprintf(fp, "time accurate rate:%lf\n", (double)tp->time_hit_num/tp->hassissnum);
   fprintf(fp, "recall rate:%lf\n", (double)tp->hassissnum/tp->totalhtmlnum);
   fclose(fp);
}

int main(int argc, char* argv[])
{
   UrlBuf* ub = NULL;
   //read_feather(&ub, "id_classes");
   
   CommonPart* sList = NULL;
   read_feather_with_siss(&sList, "newcompartFile");
   //read_commonpart(&sList, "compartFile");
   //print_UrlBuf(ub);
   print_commonpart(sList); 
   
   //cpy_freath_to_Commonpart(ub, sList);
   //write_commonpart_to_file(sList, "newcompartFile");
   
   UrlBuf* mainurl = NULL;
   read_urls_to_UrlBuf(&mainurl, "suculswave");
//   print_UrlBuf(mainurl);
   
   //test ext content with filenum
   //test_ext_content_with_filenum(mainurl, sList);
   TargetPercent tp;
   bzero(&tp, sizeof(tp));
   test_targetPercent(mainurl, sList, &tp);
   printf("begin write ans\n");
   writePercentToFile(&tp, "gumboextAnswer");
   
   return 0;
}
