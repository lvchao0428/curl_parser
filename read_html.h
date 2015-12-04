#ifndef READ_HTML_H_
#define READ_HTML_H_

#include<stdio.h>
#include<stdlib.h>
#include<gumbo.h>
#include<sys/stat.h>
#include<string.h>
#include"def.h"

void read_file(FILE* fp, char** output, int* length);


void read_all_attr(GumboNode* root);


void read_document(GumboOutput* output);


void cleanElement(GumboNode* node, PointerPair** ileg_ele_list);

void read_html(GumboNode* root, 
	  GumboNode** contentNode,
	  GumboNode** authNode,
	  GumboNode** timeNode,
	  char* id, char* cls, char* auth, char* time);

int fill_content(char* htmlsFile, 
	  char** contentCon, 
	  char** authCon,
	  char** timeCon,
	  char* id, char* cls, char* auth, char* time);


#endif
