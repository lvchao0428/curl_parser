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
	  printf("%d %s\n", p->lineno, p->str);
	  p = p->next;
   }
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
	  printf("%d.html write over...", p->lineno);
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
	  printf("cannot open the fileurl\n");
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
	  j++;		//skip '#'
	  int intNo = atoi(tempno);
	  if(*ub == NULL)
	  {
		 *ub = (UrlBuf*)malloc(sizeof(UrlBuf));
		 memset(*ub, 0, sizeof(UrlBuf));
		 
		 (*ub)->lineno = intNo;
		 (*ub)->str = (char*)malloc(sizeof(char)*(strlen(line) + 1));
		 //strcpy((*ub)->str, line);
		 memcpy((*ub)->str, line + j, strlen(line));
		 p = temp = *ub;
	  }
	  else
	  {
		 UrlBuf* q = (UrlBuf*)malloc(sizeof(UrlBuf));
		 memset(q, 0, sizeof(*q));
		 q->str = (char*)malloc(sizeof(char)*(strlen(line) + 1));
		 q->lineno = intNo;
		 memcpy(q->str, line+j, strlen(line));
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
	  memcpy((*head)->str, son->str, sizeof(char)*(strlen(son->str) + 1));
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

