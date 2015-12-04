/*************************************************************************
    > File Name: test_read_html.c
    > Author: lvchao0428
    > Mail: 410148119@qq.com 
    > Created Time: Mon Nov 23 15:10:32 2015
 ************************************************************************/
#include"read_html.h"
#include<assert.h>

void read_file(FILE* fp, char** output, int* length)
{
   struct stat filestats;
   int fd = fileno(fp);
   fstat(fd, &filestats);
   *length = filestats.st_size;
   *output = malloc(*length + 1);
   int start = 0;
   int bytes_read;
   while((bytes_read = fread(*output + start, 1, *length - start, fp)))
   {
	  start += bytes_read;
   }


}

char* find_title(GumboNode* root) 
{
   GumboVector* root_children = &root->v.element.children;
   GumboNode* head = NULL;
   if(root->type == GUMBO_NODE_ELEMENT || root->v.element.children.length >= 2)
   {
	 
	  int i;
	  for (i = 0; i < root_children->length; ++i) {
		 GumboNode* child = root_children->data[i];
		 if (child->type == GUMBO_NODE_ELEMENT &&
			   child->v.element.tag == GUMBO_TAG_HEAD) {
			head = child;
			break;
		 }
	  }
   }

   if(head != NULL)
   {
	 GumboVector* head_children = &head->v.element.children;
	 int i;
	 for (i = 0; i < head_children->length; ++i) {
		GumboNode* child = head_children->data[i];
		if (child->type == GUMBO_NODE_ELEMENT &&
			  child->v.element.tag == GUMBO_TAG_TITLE) {
		   if (child->v.element.children.length != 1) {
			  return "<empty title>";
		   }
		   GumboNode* title_text = child->v.element.children.data[0];
		   if(title_text->type == GUMBO_NODE_TEXT || title_text->type == GUMBO_NODE_WHITESPACE)
			   return title_text->v.text.text;
		}
	 }
  }

  
  return "<no title found>";
}

void read_all_attr(GumboNode* root)
{
   if(root->type != GUMBO_NODE_ELEMENT)
   {
	  return;
   }
   GumboAttribute* attr;
   attr = gumbo_get_attribute(&root->v.element.attributes, "id");
   if(attr)
	  printf("name:%s, attr:%s\n", attr->name, attr->value);
   int i;
   GumboVector* children = &root->v.element.children;
   for(i = 0; i < children->length; ++i)
   {
	  if(children->data[i] != "")
	  {
		 //printf("element:%d: %s\n", i, children->data[i]);
	  }
	  read_all_attr(children->data[i]);
   }
}

void read_document(GumboOutput* output)
{
   GumboNode* document = output->document;
   //if(root->type == GUMBO_NODE_DOCUMENT)
  // {
		 //print name
		 printf("name:%s\n", document->v.document.name);
		 //print public identifier
		 printf("public identifier:%s\n", document->v.document.public_identifier);
		 //print system identifier
		 printf("system identifier:%s\n", document->v.document.system_identifier);
	//  }
}

void cleanElement(GumboNode* node, PointerPair** ileg_ele_list)
{
   if(node->type == GUMBO_NODE_ELEMENT &&
		 node->type == GUMBO_TAG_SCRIPT &&
		 node->type == GUMBO_TAG_STYLE &&
		 node->type == GUMBO_TAG_ANNOTATION_XML)
   {
	  //insert illegal element node
	  if((*ileg_ele_list == NULL))
	  {
		 *ileg_ele_list = (PointerPair*)malloc(sizeof(PointerPair));
		 (*ileg_ele_list)->next = NULL;
		 (*ileg_ele_list)->left = node->v.element.original_tag.data;
		 (*ileg_ele_list)->right = node->v.element.original_end_tag.data + 
			node->v.element.original_end_tag.length;

	  }
	  else
	  {
		 PointerPair* q, * head, * p;
		 p = *ileg_ele_list;
		 head = p;
		 while(p->next)
		 {
			p = p->next;
		 }
		 q = (PointerPair*)malloc(sizeof(PointerPair));
		 q->next = NULL;
		 q->left = node->v.element.original_tag.data;
		 q->right = node->v.element.original_end_tag.data + 
			node->v.element.original_end_tag.length;
		 p->next = q;
		 *ileg_ele_list = head;
		 
	  }
	  GumboVector* children = &node->v.element.children;
	  int i;
	  for(i = 0; i < children->length; ++i)
	  {
		 cleanElement(children->data[i], ileg_ele_list);
	  }

   }
}

int cleantext(GumboNode* node, char** text)
{
   if(!node)
   {
	  return -1;
   }
   if(node->type == GUMBO_NODE_TEXT)
   {
	  //strncat(content, node->v.text.original_text.data, node->v.text.original_text.length);
	  //char* str = (char*)malloc(sizeof(char)*(strlen(node->v.text.text) + 1));
	  
	  int len = strlen(node->v.text.text);
	  if(*text == NULL)
	  {
		 *text = (char*)malloc(sizeof(char)*(len + 1));
		 bzero(*text, sizeof(char)*(strlen(*text) + 1)); 
		 strcpy(*text, node->v.text.text);
	  }
	  else
	  {
		 *text = (char*)realloc(*text, sizeof(char)*(strlen(*text) + len+1));
		 strcat(*text, node->v.text.text);
	  }

   }
   else if(node->type == GUMBO_NODE_ELEMENT &&
		 node->v.element.tag != GUMBO_TAG_SCRIPT &&
		 node->v.element.tag != GUMBO_TAG_STYLE &&
		 node->v.element.tag != GUMBO_TAG_ANNOTATION_XML)
   {
	  //char* content = (char);
   
	  GumboVector* children = &node->v.element.children;
	  int i;
	  for(i = 0; i < children->length; ++i)
	  {
		 cleantext(children->data[i], text);
	  }
   }

   return 1;
}

void print_with_scope(const char* str, int begpos, int endpos)
{
   int i;
   for(i = begpos; i < endpos; ++i)
   {
	  printf("%c", str[i]);
   }
}

int get_content_with_attr(GumboNode* node, char* content)
{
   GumboVector* attrVec = &node->v.element.attributes;
   int i;
   for(i = 0; i < attrVec->length; ++i)
   {
	  GumboAttribute* attr = attrVec->data[i];
	  if(strcmp(attr->value, "01") == 0)
	  {
		 print_with_scope(node->v.element.original_tag.data, 
			   node->v.element.original_tag.length, node->v.element.end_pos.offset);
		 printf("\n");
		 break;
	  }
   }
}

void print_with_length(const char* str, int length)
{
   int i;
   for(i = 0; i < length; ++i)
   {
	  printf("%c", str[i]);
   }
}

void print_lable(GumboNode* node, const char* tag)
{
   if(strcmp(tag, "beg") == 0)
   {
	  int i;
	  for(i = 0; i < node->v.element.original_tag.length && 
			node->v.element.original_tag.data[i] != ' ' &&
			node->v.element.original_tag.data[i] != '>'; ++i)
	  {
		 printf("%c", node->v.element.original_tag.data[i]);
	  }
   }
   else if(strcmp(tag, "end") == 0)
   {
	  int i;
	  for(i = 0; i < node->v.element.original_end_tag.length && 
			node->v.element.original_end_tag.data[i] != ' ' &&
			node->v.element.original_end_tag.data[i] != '>'; ++i)
	  {
		 printf("%c", node->v.element.original_end_tag.data[i]);
	  }
   }
}

void print_with_pointer(char* beg, char* end)
{
   char* b = beg;
   while(b != end)
   {
	  printf("%c", *b);
	  b++;
   }
}

void split_lable(char* multiLable, char* part1, char* part2)
{
  //多级标签用'>'隔开,目前最多两级
   int i = 0;
   //char part1[30], part2[30];
   //bzero(part1, sizeof(part1));
   //bzero(part2, sizeof(part2));

   int a = 0, b = 0;
   //part1
   while(multiLable[i] != '>')
   {
	  part1[a++] = multiLable[i++];
   }
   part1[a] = '\0';
   i++;
   //part2
   while(multiLable[i] != '\0')
   {
	  part2[b++] = multiLable[i++];
   }
   part2[b] = '\0';
 
}


void multi_layer_Nodefind(GumboNode* root, GumboNode** deepNode, char* part1, char* part2)
{
    //已经定位到根节点，再遍历一级即可
	GumboVector* children = &root->v.element.children;
	int i;
	for(i = 0; i < children->length && *deepNode != NULL; ++i)
	{
	  GumboVector*	attrVec = &root->v.element.attributes;
	  GumboAttribute* attr;
	  int j;
	  for(j = 0; j < attrVec->length; ++j)
	  {
		 attr = attrVec->data[i];
		 if(strcmp(attr->value, part2) == 0)
		 {
			*deepNode = children->data[i];
			break;
		 }
	  }
	}

   
}
/*
 *定位到内容节点，和时间节点
 * */
void locate_node(GumboNode* root, GumboNode** contentNode, GumboNode** timeNode)
{
   if(!root)
   {
	  return;
   }
   else if(*contentNode == NULL || *timeNode == NULL)
   {
	  if(root->type == GUMBO_NODE_ELEMENT)
	  {

	  }
   }
}

void read_html(GumboNode* root, 
	  GumboNode** contentNode,
	  GumboNode** authNode,
	  GumboNode** timeNode,
	  char* id, char* cls, char* auth, char* time)
{
   if(!root)
   {
	  return;
   }
   else if(*contentNode == NULL || *authNode == NULL || *timeNode == NULL)
   {
	  /*
	  if(root->type == GUMBO_NODE_DOCUMENT)
	  {
		 printf("docu-name:%s\n", root->v.document.name);
		 GumboVector* children = &root->v.document.children;
		 int i;	  print_with_length(child->v.element.original_tag.data, 
						child->v.element.end_pos.offset);
		 for(i = 0; i < children->length; ++i)
		 {
			read_html(children->data[i]);
		 }
	  }
	  */
	  if(root->type == GUMBO_NODE_ELEMENT)
	  {
	/*	 
		 printf("lablenum:%d\n", lable_num++);
		 //print beg tag
		 printf("elem-beg tag:");
		 print_with_length(root->v.element.original_tag.data, root->v.element.original_tag.length);
		 printf("\nelem-beg str:");
		 print_lable(root, "beg");
		 printf("\n");
		 print_with_length(root->v.element.original_end_tag.data, root->v.element.original_end_tag.length);
		 printf("start pos: line:%d column:%d offset:%d ", root->v.element.start_pos.line, 
			   root->v.element.start_pos.column, root->v.element.start_pos.offset);
//		 printf("start lable:%s")
		 printf("endpos: line:%d column:%d offset:%d ", root->v.element.end_pos.line, 
			   root->v.element.end_pos.column, root->v.element.end_pos.offset);
		 printf("end str:");
	*/	 
	//	 print_lable(root, "end");
	//	 printf("\n");
		 //print end tag
		 //print attr
		 
		 int i;
		 GumboVector* attrVec = &root->v.element.attributes;
		 //printf("\nattr:");
		 if(attrVec->length == 0)
		 {
			//printf("null attr\n");
		 }
		 else
		 {
			GumboAttribute* attr;
			for(i = 0; i < attrVec->length; ++i)
			{
			   attr = attrVec->data[i];
			   //printf(" %d:", i);
			   //print_with_length(attr->name)
			   // printf("attr:%d name:%s value:%s ", i, attr->name, attr->value);
			   if(*contentNode == NULL)
			   {
				  if((cls != NULL || id != NULL))
				  {
					 if((cls && cls[0] != '\0' && 
							  (strstr(cls, "postmessage") || strstr(cls, "post_content")) )
						   || (id && id[0] != '\0' && 
							  (strstr(id, "postmessage") || strstr(id, "post_content"))))
					 {
						if(cls && cls[0] != '\0' && strstr(attr->value, cls))
						{
						   printf("message nutualize\n");
						   *contentNode = root;
						   return;
						}
						if(id && id[0] != '\0' && strstr(attr->value, id))
						{
						   printf("message nutualize\n");
						   *contentNode = root;
						   return;
						}
					 }
					 else
					 {
						if(cls != NULL && cls[0] != '\0')
						{
						   if(strcmp(attr->value, cls) == 0)
						   {
							  printf("target nurtualize\n");
							  // print_with_length(root->v.element.original_tag.data + 
							  //		 root->v.element.original_tag.length, 
							  //		root->v.element.end_pos.offset - root->v.element.original_end_tag.length);
							  *contentNode = root;
							  return;
						   }

						}
						if(id != NULL && id[0] != '\0')
						{
						   if(strcmp(attr->value, id) == 0)
						   {
							  printf("target nurtualize\n");
							  //print_with_length(root->v.element.original_tag.data + 
							  //	 root->v.element.original_tag.length, 
							  //	root->v.element.end_pos.offset);
							  *contentNode = root;
							  return;
						   }
						}
					 }


				  }
			   }

			   if(*authNode == NULL)
			   {
				  if(auth[0] != '\0')
				  {
					 if(strcmp(attr->value, auth) == 0)
					 {
						printf("auth pointed\n");
						*authNode = root;
					 }
				  }
			   }

			   if(*timeNode == NULL)
			   {
				  if(strstr(time, ">"))
				  {//包含多级规则
					 char part1[30], part2[30];
					 bzero(part1, sizeof(part1));
					 bzero(part2, sizeof(part2));
					 split_lable(time, part1, part2);
					 if(strcmp(attr->value, part1) == 0)
					 {
						multi_layer_Nodefind(root,timeNode,part1, part2);
						return;
					 }
				  }
				  else if((strstr(time, "authorposton") && strstr(attr->value, time))||
						(!strstr(time, "autorposton") && strcmp(attr->value, time) == 0))
				  {//大概是discuz使用部分匹配
					 *timeNode = root;
					 return;
				  }
			   }

			}
		 }

		 GumboVector* children = &root->v.element.children;
		 
		 for(i = 0; i < children->length; ++i)
		 {
			read_html(children->data[i], 
				  contentNode, authNode, timeNode, id, cls, auth, time);
		 }
	
	  }

   }

}

void read_content(GumboNode* node, char* cls_id)
{
   if(node->type == GUMBO_NODE_ELEMENT)
   {
	  GumboVector* children = &node->v.element.children;
	  int i;
	  for(i = 0; i < children->length; ++i)
	  {
		 GumboNode* child = children->data[i];
		 //GumboAttribute* attr = &children->v.element.attributes;
		 GumboVector* attrVec = &child->v.element.attributes;
		 int j;
		 //printf("")
		 for(j = 0; j < attrVec->length; ++j)
		 {
			GumboAttribute* attr = attrVec->data[j];
			printf("attr:%s\n", attr->value);
			if(strcmp(cls_id, attr->value) == 0)
			{
			   printf("content:%s\n", child->v.element.original_tag.data);
			   print_with_length(child->v.element.original_tag.data, 
					 child->v.element.end_pos.offset);
			}
			else
			{
			   read_content(children->data[i], cls_id);
			}

		 }


	  }
   }
}



int fill_content(char* htmlsFile, 
	  char** contentCon, 
	  char** authCon,
	  char** timeCon,
	  char* id, char* cls, char* auth, char* time)
{
   FILE* fp = fopen(htmlsFile, "r");
   if(!fp)
   {
	  printf("FILE %s not found!\n", htmlsFile);
	  return -1;
   }

   char* input;
   int input_length;
   //char* text;
   read_file(fp, &input, &input_length);
   

 //  GumboOutput* output = gumbo_parse_with_options(
//		 &kGumboDefaultOptions, input, input_length);
   GumboOutput* output = gumbo_parse(input); 
   if(!output)
   {
	  printf("gumbo error\n");
   }
   //int num = 0;
   GumboNode* contentNode = NULL;
   GumboNode* authNode = NULL;
   GumboNode* timeNode = NULL;
   //char* content = NULL;
   //如果title里面有302或者404 则不计数
   char* title = find_title(output->root);
   if(strstr(title, "302") || strstr(title, "404") || strstr(title, "403"))
   {
	  return -2;
   }
   read_html(output->root, &contentNode, &authNode, &timeNode, id, cls, auth, time);
   //printf("elementstr:\n");
  // char* tempend = contentNode->v.element.original_end_tag.data + contentNode->v.element.original_end_tag.length;
  // print_with_pointer(contentNode->v.element.original_tag.data, tempend);
   int ret1 = cleantext(contentNode, contentCon);
   int ret2 = cleantext(authNode, authCon);
   int ret3 = cleantext(timeNode, timeCon);
   //deal auth
   //deal time
   int ret = ret1 + ret2 + ret3;
//   printf("content:%s\n", *content);
   //read_content(output->root);
//   printf("text:%s\n", text);
   //read_all_attr(output->root);
   // 
   //read_document(output);
   gumbo_destroy_output(&kGumboDefaultOptions, output);
   free(input);
   
   fclose(fp);
   
   if(ret1 > 0)
   {
	  return 1;
   }
   else
   {
	  return -1;
   }
	  
}
