/*************************************************************************
    > File Name: savehtml.c
    > Author: lvchao0428
    > Mail: 410148119@qq.com 
    > Created Time: Mon Nov  2 14:29:27 2015
 ************************************************************************/

#include<stdio.h>
#include"savehtml.h"
#include"code_convert.h"
#include"def.h"

static size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream)
{
   size_t written = fwrite(ptr, size, nmemb, (FILE*)stream);
   return written;
}

void print_UrlBuf(UrlBuf* ub)
{
   UrlBuf* p = ub;
   while(p)
   {
	  printf("%d       %s\n", p->lineno, p->str);
	  //id cls
	  printf("id:%s\t classes:%s\n", p->id, p->cls);
	  p = p->next;
   }
}

int write_commonpart_to_file(CommonPart* sList, char* filename)
{
   FILE* fp;
   fp = fopen(filename, "w");
   if(!fp)
   {
	  printf("open commonpart write file error\n");
	  exit(-1);
   }

   CommonPart* p = sList;
   while(p)
   {
	  //comstr id cls
	  fprintf(fp, "%s~%s~%s\n", p->comStr, p->id, p->cls);
	  p = p->next;
   }
   fclose(fp);

   return 1;
}

int write_commonpart_and_url_to_file(CommonPart* sList, char* filename)
{
   FILE* fp;
   CommonPart* p = sList;
   fp = fopen(filename, "w");
   if(!fp)
   {
	  printf("open write file %s failed\n", filename);
	  return -1;
   }


   while(p)
   {
	  //存入url特征
	  int num = 0;
	  //fprintf(fp, "%s\n", p->comStr);
	  UrlBuf* tempub = p->ubList;
	  while(num < 3 && tempub)
	  {
		 fprintf(fp, "%s", tempub->str);
		 tempub = tempub->next;
		 num++;
	  }
	  //fprintf(fp,"\n");
	  p = p->next;
   }

   fclose(fp);
   return 1;
}

void make_a_commonpart(CommonPart* cp, char* str, char* id, char* cls, char* auth, char* time)
{
   cp->comStr = (char*)malloc(sizeof(char)*(strlen(str) + 1));
   memcpy(cp->comStr, str, sizeof(char)*(strlen(str) + 1));
   
   cp->id = (char*)malloc(sizeof(char)*(strlen(id) + 1));
   memcpy(cp->id, id, sizeof(char)*(strlen(id) + 1));
   cp->cls = (char*)malloc(sizeof(char)*(strlen(cls) + 1));
   memcpy(cp->cls, cls, sizeof(char)*(strlen(cls) + 1));
   cp->auth = (char*)malloc(sizeof(char)*(strlen(auth) + 1));
   memcpy(cp->auth, auth, sizeof(char)*(strlen(auth) + 1));
   cp->time = (char*)malloc(sizeof(char)*(strlen(time) + 1));
   memcpy(cp->time , time, sizeof(char)*(strlen(time) + 1));

}

CommonPart* push_CommonPart(CommonPart** cp, CommonPart src)
{
   CommonPart* head = *cp;
   CommonPart* p = head;
   CommonPart* retp = NULL;
   if(*cp == NULL)
   {
	  *cp = (CommonPart*)malloc(sizeof(CommonPart));
	  bzero(*cp, sizeof(CommonPart));
	  //str
	  (*cp)->comStr = (char*)malloc(sizeof(char)*(strlen(src.comStr) + 1));
	  memcpy((*cp)->comStr, src.comStr, sizeof(char)*(strlen(src.comStr) + 1));
	  //id
	  (*cp)->id = (char*)malloc(sizeof(char)*(strlen(src.id) + 1));
	  memcpy((*cp)->id, src.id, sizeof(char)*(strlen(src.id) + 1));
	  //cls
	  (*cp)->cls = (char*)malloc(sizeof(char)*(strlen(src.cls) + 1));
	  memcpy((*cp)->cls, src.cls, sizeof(char)*(strlen(src.cls) + 1));
	  //auth
	  (*cp)->auth = (char*)malloc(sizeof(char)*(strlen(src.auth) + 1));
	  memcpy((*cp)->auth, src.auth, sizeof(char)*(strlen(src.auth) + 1));
	  (*cp)->time = (char*)malloc(sizeof(char)*(strlen(src.time) + 1));
	  memcpy((*cp)->time , src.time, sizeof(char)*(strlen(src.time) + 1));  

	  (*cp)->next = NULL;
	  retp = (*cp);
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
	  //id
	  q->id = (char*)malloc(sizeof(char)*(strlen(src.id) + 1));
	  memcpy(q->id, src.id, sizeof(char)*(strlen(src.id) + 1));
	  //cls
	  q->cls = (char*)malloc(sizeof(char)*(strlen(src.cls) + 1));
	  memcpy(q->cls, src.cls, sizeof(char)*(strlen(src.cls) + 1));	
	  //auth
	  q->auth = (char*)malloc(sizeof(char)*(strlen(src.auth) + 1));
	  memcpy(q->auth, src.auth, sizeof(char)*(strlen(src.auth) + 1));
	  //time
	  q->time = (char*)malloc(sizeof(char)*(strlen(src.time) + 1));
	  memcpy(q->time , src.time, sizeof(char)*(strlen(src.time) + 1));

	  p->next = q;
	  *cp = head;
	  retp = q;
   }

   return retp;
}

void read_commonpart(CommonPart** sList, char* filename)
{
   FILE* fp;
   size_t len;
   ssize_t read;
   fp = fopen(filename, "r");
   if(!fp)
   {
	  exit(-1);
   }
   char* line = NULL;
   while((read = getline(&line, &len, fp)) != -1)
   {
	   CommonPart tempcp;
	   tempcp.comStr = (char*)malloc(sizeof(char)*(strlen(line) + 1));
	   line[strlen(line)-1] = '\0';
	   strcpy(tempcp.comStr, line);
	  
	   push_CommonPart(sList, tempcp);
	   
   }

   //fclose(fp);
}

void print_commonpart(CommonPart* sList)
{
   CommonPart* p = sList;
   while(p)
   {	
	  printf("str:%s, id:%s, cls:%s, auth:%s, time:%s\n", p->comStr, p->id, p->cls, p->auth, p->time);
	  
	  p = p->next;
   }
}

void make_a_urlbuf(UrlBuf* ub, char* id, char* cls, char* str, char* siss)
{
   ub->siss = (char*)malloc(sizeof(char)*(strlen(siss) + 1));
   ub->id = (char*)malloc(sizeof(char)*(strlen(id) + 1));
   ub->str = (char*)malloc(sizeof(char)*(strlen(str) + 1));
   ub->cls = (char*)malloc(sizeof(char)*(strlen(cls) + 1));
   bzero(ub->siss, sizeof(char)*(strlen(siss) + 1));
   bzero(ub->id, sizeof(char)*(strlen(id) + 1));
   bzero(ub->str, sizeof(char)*(strlen(str) + 1));
   bzero(ub->cls, sizeof(char)*(strlen(cls) + 1));

   strcpy(ub->siss, siss);
   strcpy(ub->id, id);
   strcpy(ub->str, str);
   strcpy(ub->cls, cls);
}

void url2file(char* url, char* filename)
{
   CURL* curl_handle;
   FILE* pagefile;
   if(!filename || filename[0] == '\0')
   {
	  return;
   }

   curl_global_init(CURL_GLOBAL_ALL);

   /*init the curl session*/
   curl_handle = curl_easy_init();

   /*set URL to get here*/
   curl_easy_setopt(curl_handle, CURLOPT_URL, url);

   /*switch on full protocol output while testing*/
   curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 0L);		//显示进度条

   /*disable progress meter , set to 0L to enable and disable debug output*/
   curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 0L);

   /*send all data to this function*/
   curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);

   pagefile = fopen(filename, "w");
   if(pagefile)
   {
	  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);

	  curl_easy_perform(curl_handle);

	  fclose(pagefile);
   }

   curl_easy_cleanup(curl_handle);

   return;
}

int write_urls_to_UrlBuf(UrlBuf* ub, char* urlfilename)
{
   FILE* fp;
   UrlBuf* p = ub;
   fp = fopen(urlfilename, "w");
   if(!fp)
   {
	  printf("open write file failed\n");
	  return -1;
   }
   while(p)
   {
	  //char line[200];
	  //bzero(line, sizeof(line));
	  //sprintf(line, "%d^%s", p->lineno, p->str);
	  fprintf(fp, "%d~%s", p->lineno, p->str);
	  //printf("%d.html write over...", p->lineno);
	  p = p->next;
   }
   
   fclose(fp);
   return 1;
}

int read_urls_to_UrlBuf(UrlBuf** ub, char* urlfilename)
{
   FILE* fp;
   char* line = NULL;
   size_t len = 0;
   ssize_t read;
   UrlBuf* p, * temp;
   //p = temp = *ub;
   fp = fopen(urlfilename, "r");
   if(!fp)
   {
	  printf("cannot open the file %s\n", urlfilename);
	  return -1;
   }
   int lineno = 1;
   while((read = getline(&line, &len, fp)) != -1)
   {
	  char tempno[20];
	  bzero(tempno, sizeof(tempno));
	  //读取lineno
	  int j = 0;
	  while(line[j] != '~')
	  {
		 tempno[j] = line[j];
		 j++;
	  }
	  tempno[j] = '\0';
	  j++;		//skip '~'
	  int intNo = atoi(tempno);
	  if(*ub == NULL)
	  {
		 *ub = (UrlBuf*)malloc(sizeof(UrlBuf));
		 memset(*ub, 0, sizeof(UrlBuf));
		 
		 (*ub)->lineno = intNo;
		 (*ub)->str = (char*)malloc(sizeof(char)*(strlen(line) + 1));
		 //strcpy((*ub)->str, line);
		 memcpy((*ub)->str, line + j, sizeof(char)*(strlen(line) + 1));
		 p = temp = *ub;
	  }
	  else
	  {
		 UrlBuf* q = (UrlBuf*)malloc(sizeof(UrlBuf));
		 memset(q, 0, sizeof(*q));
		 q->str = (char*)malloc(sizeof(char)*(strlen(line) + 1));
		 q->lineno = intNo;
		 memcpy(q->str, line+j, sizeof(char)*(strlen(line) + 1));
		 p->next = q;
		 p = p->next;
	  }
	  
	  lineno++;
   }
   *ub = temp; 
   free(line);
   fclose(fp); 

   return 1;

}
void single_url_to_file(UrlBuf* urls, int curindex)
{
   if(!urls)
   {
	  return;
   }

   UrlBuf* p = urls;
   UrlBuf* curUrl = NULL;
   //find current url
   while(p)
   {
	  if(p->lineno == curindex)
	  {
		 curUrl = p;
		 break;
	  }
	  else
	  {
		 p = p->next;
	  }
   }

   char filename[30];
   bzero(filename, sizeof(filename));
   sprintf(filename, "./kejihtmls/%d.html", p->lineno);
   url2file(p->str, filename);	   
   printf("%d.html complete\n", p->lineno);
}

void all_url_to_file(UrlBuf* urls, int curindex)
{
   if(!urls)
   {
	  return;
   }

   UrlBuf* p = urls;
   UrlBuf* curUrl = NULL;
   //find current url
   while(p)
   {
	  if(p->lineno == curindex)
	  {
		 curUrl = p;
		 break;
	  }
	  else
	  {
		 p = p->next;
	  }
   }
   while(p)
   {
	  char filename[30];
	  bzero(filename, sizeof(filename));
	  sprintf(filename, "./kejihtmls/%d.html", p->lineno);
	  url2file(p->str, filename);	   
	  printf("%d.html complete\n", p->lineno);
	  p = p->next;
   }
}

void urls_to_file(UrlBuf* urls)
{
   if(!urls)
   {
	  return;
   }

   UrlBuf* p = urls;

   while(p)
   {
	  char filename[50];
	  bzero(filename, sizeof(filename));
	  sprintf(filename, "./kejihtmls/%d.html", p->lineno);
	  url2file(p->str, filename);
	  printf("%d.html complete\n", p->lineno);
	  p = p->next;
   }
}

int file_read_full(char** dest, const char* filename)
{//把整个文件读成一个字符串
   FILE* fp;
   struct stat file_stats;
   int nItems, nBytesRead;

   *dest = NULL;
   if((fp = fopen(filename, "r")) == NULL)
   {
	  fprintf(stderr, "failed to open %s - %s\n", filename, strerror(errno));
	  return(-1);
   }
   fstat(fileno(fp), &file_stats);

   if((*dest = (char*)malloc(file_stats.st_size+1)) == NULL)
   {
	  fclose(fp);
	  fprintf(stderr, "failed to allocate memory for reading file%s\n", filename);

	  return(-2);
   }
   nBytesRead = 0;
   while((nItems = fread(*dest + nBytesRead, 1, BUFSIZ,fp)) > 0)
   {
	  nBytesRead += nItems;
   }
   fclose(fp);
   *(*dest + nBytesRead) = '\0';
   
   return (nBytesRead);
}

UrlBuf* push_Url(UrlBuf** head, UrlBuf* son)
{
   UrlBuf* tail = (*head);
   UrlBuf* temphead = (*head);
   if(*head == NULL)
   {
	  (*head) = (UrlBuf*)malloc(sizeof(UrlBuf));
	  (*head)->next = NULL;
	  (*head)->lineno = son->lineno;
	  (*head)->str = (char*)malloc(sizeof(char)*(strlen(son->str) + 1));
	  //id cls str siss
	  memcpy((*head)->str, son->str, sizeof(char)*(strlen(son->str) + 1));
	  (*head)->id = (char*)malloc(sizeof(char)*(strlen(son->id) + 1));
	  memcpy((*head)->id, son->id, sizeof(char)*(strlen(son->id) + 1));
	  (*head)->cls = (char*)malloc(sizeof(char)*(strlen(son->cls) + 1));
	  memcpy((*head)->cls, son->cls, sizeof(char)*(strlen(son->cls) + 1));
	  //siss...
	  temphead = *head; 
   }
   else
   {
	  //find tail;
	  while(tail->next)
	  {
		 tail = tail->next;
	  }
	  UrlBuf* q = (UrlBuf*)malloc(sizeof(UrlBuf));
	  q->next = NULL;
	  q->lineno = son->lineno;
	  //id cls str siss
	  q->id = (char*)malloc(sizeof(char)*(strlen(son->id) + 1));
	  memcpy(q->id, son->id, sizeof(char)*(strlen(son->id) + 1));
	  q->cls = (char*)malloc(sizeof(char)*(strlen(son->cls) + 1));
	  memcpy(q->cls, son->cls, sizeof(char)*(strlen(son->cls) + 1));
	  q->str = (char*)malloc(sizeof(char)*(strlen(son->str) + 1));
	  memcpy(q->str, son->str, sizeof(char)*(strlen(son->str) + 1));
	  tail->next = q;

   }
   *head = temphead;
   return *head;
}

int convert_all_files_to_UTF8(UrlBuf* ub, UrlBuf** failub, UrlBuf** sucub)
{
  // FileLink* p = head;
   int filenum = 1;
   //统计编码类型
   
   UrlBuf* p = ub;
   while(p)
   {
	  FILE* fp;
	  char filename[50];
	  bzero(filename, sizeof(filename));
	  char type[20];
	  bzero(type, sizeof(type));
	  sprintf(filename, "./kejihtmls/%d.html", p->lineno);
	  char outfilename[50];
	  bzero(outfilename, sizeof(outfilename));
	  sprintf(outfilename, "./kejiUtfhtmls/%d.html", p->lineno);
	  
	  char* str = NULL;
	  //q->next = NULL;
	  int retread = file_read_full(&str, filename);
	  if(retread == -1 || retread == -2)
	  {
		 printf("failed, to open %s\n", filename);
		 *failub = push_Url(failub, p); 
		 p = p->next;

		 continue;
	  }
	 
	  //p->next = q;
	  //p = p->next;
	  size_t inlen, outlen;
	  inlen = strlen(str);
	  outlen = 2*inlen;
	  code_type_get(str, type);
	  char* outfile;
	  
	  if(strcmp(type, "utf-8") == 0 || type[0] == '\0')
	  {
		 if((fp = fopen(outfilename, "w")) == NULL)
		 {
			printf("failed to open write file\n");
			*failub = push_Url(failub, p);
			p = p->next;
			continue;
		 }

		 fwrite(str, sizeof(char), strlen(str), fp);
	  }
	  else
	  {
	
		 char* outfile = (char*)malloc(sizeof(char)*(2*strlen(str)));
		 code_convert(type, "utf-8", str, inlen, outfile, outlen);
		 
		 //检查 文件是否有尾部
		 if(!strstr(outfile, "</html>"))
		 {
			*failub = push_Url(failub, p);
			p = p->next;
			continue;
		 }
		 if((fp = fopen(outfilename, "w")) == NULL)
		 {
			printf("failed to open write file\n");
			*failub = push_Url(failub, p);
			p = p->next;
			continue;
		 }


		 fwrite(outfile, sizeof(char), strlen(outfile), fp);


		 free(outfile);
	  }
	  *sucub = push_Url(sucub, p);
	  fclose(fp);
	  free(str);
	  printf("%d complete,\t\ttype:%s \n", p->lineno, type );
	  p = p->next;
   }

}

int test_filelink(FileLink* head)
{
   FileLink* p = head->next;
   while(p)
   {
	  printf("%s\n", p->str);
	  p = p->next;
   }
   return 0;
}

