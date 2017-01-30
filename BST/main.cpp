#include <iostream>
#include <string.h>
#include <fstream>
#include <string>
#include "bst.h"
using namespace std;

//将字符串转换为小写
char * change_to_lower(char *str){
	for(unsigned int i = 0; i < strlen(str); ++i)
		str[i] = tolower(str[i]);
	return str;
}

//从一整行文本中分离出单词
void word_token(Bstree &T, char *str, unsigned int line){
	char *p = strtok(str, " \":,.?!-\\(\\)';1234567890");
	int offset = 0;
	while(p){
		cout << p << "@\n";
		insertBST(T, change_to_lower(p), line, offset);
		offset++;
		p = strtok(NULL, " \":,.?!-\\(\\)';1234567890");
	}
}


int main(){
	//create binary search tree
	Bstree T;
	initBST(T);
	ifstream fin("bible");
	char buf[1024];
	unsigned int line = 0;
	while(!fin.eof()){
		fin.getline(buf, 1024); line++;
		word_token(T, buf, line);
	}
	fin.close();

	//查找操作
	char search_word[32];
	Node *search_result;
	cout << "输入查找的单词(空行结束输入)：";
	cin.getline(search_word, 32);
	while(search_word[0]){
		search_result = searchBST(T, search_word);
		if(search_result){
			display_result(search_result);
		}
		else{
			cout << "抱歉没有找到" << endl;

		}
		cout << "输入查找的单词(空行结束输入)：";
		cin.getline(search_word, 32);
	}

	inOrderBST(T);//中序遍历一遍查找表
	return 0;
}
