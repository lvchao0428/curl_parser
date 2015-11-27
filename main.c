/*************************************************************************
    > File Name: main.c
    > Author: lvchao0428
    > Mail: 410148119@qq.com 
    > Created Time: Mon Nov  2 14:49:08 2015
 ************************************************************************/

#include<stdio.h>
#include"code_convert.h"
#include"savehtml.h"
/*
void read_file_after_encode(FileLink* fl)
{
   FileLink* p = fl->next;

   FILE* fp;

   int filenum = 1;
   while(p)
   {
	  char type[20];
	  int len = strlen(p->str);
	 // code_type_get(p->str, type);
	  char* filestr = (char*)malloc(sizeof(char)*(2*len));
	  printf("type:%s\n", p->type);
//	  printf("file:%d\t type:%s\n", filenum, type); 
	  if(strcmp(p->type, "utf-8") != 0 && p->str[0] != '\0')
	  {
		 printf("%d: checked, type:%s, len:%d\n", filenum, p->type, len);
		 code_convert(p->type, "utf-8", p->str, len, filestr, 2*len);
	  }
	  
	  char newfile[50];
	  sprintf(newfile, "./utf_html/%d.html", filenum);
	  printf("%s\b", newfile);
	  if((fp = fopen(newfile, "w")) == NULL)
	  {
		 perror("cannot open the file");
		 exit(-1);
	  }
	
	  if(p->str[0] == '\0' || strcmp(type, "utf-8") == 0)
	  {
		 fwrite(p->str, sizeof(char), strlen(p->str), fp);	 	 
	  }
	  else
	  {
		 printf("writeing\n");
		 fwrite(filestr, sizeof(char), strlen(filestr), fp);
	  }
	  fclose(fp);
	  printf("file:%d complete...\n", filenum);
	  filenum++;

	  p = p->next;
   }
}
*/
int main(int argc, char* argv[])
{
   //down load keji html
   UrlBuf* urls = NULL;
   if(argc < 2)
   {
	  printf("need current index\n");
	  exit(EXIT_FAILURE);
   }
   int curindex = atoi(argv[2]);

   read_urls_to_UrlBuf(&urls, argv[1]);
   all_url_to_file(urls, curindex);
   //urls_to_file(urls);
   
   //single_url_to_file(urls, curindex);
   //read_all_files();
  /*
   if(argc < 2)
   {
	  printf("need 2 argv");
	  exit(-1);
   }
   */
 //  FileLink* fl = (FileLink*)malloc(sizeof(FileLink));
   //fl->next = NULL;
   /*
   char* filestr;
   char type[30];
   file_read_full(&filestr, argv[1]);
   code_type_get(filestr, type);
   printf("type is:%s str:%s\n", type, filestr);

   //read_file_after_encode(fl); 

   size_t len = strlen(filestr);
   size_t outlen = 2*len;
   char* outfile = (char*)malloc(sizeof(char)*outlen);
   code_convert(type, "utf-8", filestr, len, outfile, outlen);
   */
/*
   FILE* fp;
   if((fp = fopen("./test.html", "w")) == NULL)
   {
	  perror("failed to open the test html file");
	  exit(-1);
   }

   printf("len:%d\n", strlen(outfile));
   fwrite(outfile, sizeof(char), strlen(outfile), fp);
   fclose(fp);
   */
  // printf("out:%s\n", outfile);
   //   test_filelink(fl);
   return 0;
}
