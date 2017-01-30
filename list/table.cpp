#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include "table.h"
using namespace std;

int cnt_words;

//初始化单链表
void initList(List *T){
	(*T) = new Node;
	(*T)->next = NULL;
	cnt_words = 0;	
}

//在p结点后面添加一个新项
void addword(Node *p,char *word,unsigned int line,unsigned int offset){
	Node *item = new Node;
	strcpy(item->word, word);
	item->pos.line = line;
	item->pos.offset = offset;
	item->pos.next = NULL;
	item->next = p->next;
	p->next = item;
	cnt_words++;
}

//在p结点位置链表中添加一个新位置
void addpos(Node *p, unsigned int line, unsigned int offset){
	Position *newpos = new Position;
	newpos->line = line;
	newpos->offset = offset;
	newpos->next = p->pos.next;//头插
	p->pos.next = newpos;
}

//将单词信息加入到单链表中
void addList(List T, char *word,unsigned int line,unsigned int offset){
	Node *p = T;
	while(p->next && strcmp(word, p->word)){
		p = p->next;
	}
	if(!p->next){//末元素
		if(strcmp(word, p->word)) addword(p, word, line,offset);
		else addpos(p, line, offset);
	}
	else{
		addpos(p, line, offset);
	}
}

//单链表遍历函数
void traverse(List T, char * filename, void (*visit)(Node *, char *)){
	Node *p = T->next;
	while(p){
		visit(p, filename);	//调用访问函数
		p = p->next;
	}
}

//搜索单词
void search_word(List T, void(*display)(Node *)){
	char find_word[32];
	cout << "输入查找的单词(空行结束输入)：";
	cin.getline(find_word, 32);
	while(find_word[0]){
		cout << "查找结果：\n";
		Node *p = T->next;
		while(p){
			if(!strcmp(p->word, find_word)) {//找到了这个单词
				break;
			}
			p = p->next;
		}
		display(p);
		cout <<  "输入查找单词(空行结束输入)：";
		cin.getline(find_word, 32);
	}
}


//将字符串转换为小写
char * change_to_lower(char *str){
	for(int i = 0; i < strlen(str); ++i)
		str[i] = tolower(str[i]);
	return str;
}

//从一整行文本中分离出单词
void word_token(List L, char * str, unsigned int line){
	char *p = strtok(str, " \":,.?!-\\(\\)';1234567890");
	int offset = 0;
	while(p){
		cout << p << "@\n";
		addList(L, change_to_lower(p), line, offset);
		offset++;
		p = strtok(NULL, " \":,.?!-\\(\\)';1234567890");
	}
}


//从bible文件中建立数据结构
void build_dictionary(List L, char *srcfile){
	ifstream fin(srcfile, ios::in);
	char buf[1024];
	unsigned int line = 0;
	while(!fin.eof()){
		fin.getline(buf, 1024); line++;
		word_token(L, buf, line);//分离出单词病建立数据结构
	}
	fin.close();
}

//从倒排文件中恢复数据结构
void reback(List T, char *filename){
	ifstream fback(filename);
	string buf;
	Node *p = T;
	
	while(!fback.eof()){
		getline(fback, buf);
		for(int i = 0; i < buf.size(); ++i){
			
		}
	}
	fback.close();
}
