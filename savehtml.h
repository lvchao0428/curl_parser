#ifndef SAVEHTML_H_
#define SAVEHTML_H_


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<malloc.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<errno.h>
#include<curl/curl.h>
#include"def.h"

#define HTMLCOUNT 1024

typedef struct FileLink
{
   char* str;
   char type[20];
   struct FileLink* next;
}FileLink;


void print_UrlBuf(UrlBuf* ub);

void url2file(char* url, char* filename);

void single_url_to_file(UrlBuf* urls, int curindex);

int write_urls_to_UrlBuf(UrlBuf* ub, char* urlfilename);

int read_urls_to_UrlBuf(UrlBuf** urls, char* urlfilename);

void all_url_to_file(UrlBuf* urls, int curindex);

void urls_to_file(UrlBuf* urls);

int file_read_full(char** dest, const char* filename);

UrlBuf* push_Url(UrlBuf** head, UrlBuf* son);

int convert_all_files_to_UTF8(UrlBuf* ub, UrlBuf** failub, UrlBuf** sucub);

int test_filelink(FileLink* head);

#endif
