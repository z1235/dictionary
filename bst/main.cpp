#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fstream>
#include <string>
#include "bst.h"
using namespace std;
unsigned int word_cnt;
char *f_index = 0;
char *f_src = 0;
int haveIndexFile = 0;//是否存在倒排索引文件
ofstream fout_index;//写入数据到倒排索引文件

char * change_to_lower(char *str);
void word_token(Bstree &T, char *str, unsigned int line);
void dispTitle();
void getTableFromSrc(Bstree &T, char *f_src);
void getTableFromIndex(Bstree &T, char *f_index);
double searchDictionary(Bstree &T);

int main(int argc, char *argv[]){
	/*解析参数字符串*/
	int ch;
	while((ch = getopt(argc, argv, "s:h")) != -1){
		switch(ch){
		case 's': haveIndexFile = 0; f_src = optarg; break;
		case 'h': dispTitle(); break;
		}
	}
	/*建立binary search tree*/
	Bstree T;
	initBST(T);
	if(haveIndexFile){//存在
		getTableFromIndex(T, f_index);
	}
	else{
		getTableFromSrc(T, f_src);
	}
	//deleteBST(T, "god");	
	/*查找操作*/
	double averageTime = searchDictionary(T);
	cout << "平均查找时间: " << averageTime << "ms" << endl;
	cin.get();
	/*保存数据到索引文件*/
	f_index = (char *)"index.dat";
	fout_index.open(f_index);	
	if(!haveIndexFile){
		saveIndexFile(T);
	}
	fout_index.close();
	/*中序遍历*/
	inOrderBST(T);//中序遍历一遍查找表
	cout << "中序遍历完成" << endl;
	cout << word_cnt << endl;
	/*销毁BST*/
	destoryBST(T);
	return 0;
}

/*=====================================================*/

//将字符串转换为小写
char * change_to_lower(char *str){
	for(unsigned int i = 0; i < strlen(str); ++i)
		str[i] = tolower(str[i]);
	return str;
}

//从一整行文本中分离出单词
void word_token(Bstree &T, char *str, unsigned int line){
	char *p = strtok(str, " \":,.?!-\\(\\)][/';1234567890");
	int offset = 0;
	while(p){
		cout << p << "@\n";
		insertBST(T, change_to_lower(p), line, offset);
		offset++;
		p = strtok(NULL, " \":,.?!-\\(\\)'/][;1234567890");
	}
}
//显示提示部分
void dispTitle(){
	cout << "\t-s file: 输入源文件\n";
	cout << "\t-h 打开帮助" << endl;
	exit(0);
}

/*从源文件中获取数据*/
void getTableFromSrc(Bstree &T, char *f_src){
	ifstream fin(f_src);
	char buf[1024];
	unsigned int line = 0;
	while(!fin.eof()){
		fin.getline(buf, 1024); line++;
		word_token(T, buf, line);
	}
	fin.close();
}

/*从索引文件中获取数据*/
void getTableFromIndex(Bstree &T, char *f_index){
	fstream fin(f_index);
	string buf;
	while(!fin.eof()){
		getline(fin, buf);
		//添加单词结点
		getline(fin, buf);
		//添加位置信息
	}
	fin.close();
}

/*单词的查找操作*/
double searchDictionary(Bstree &T){
	int begin, end;
	double sumTime = 0;
	int cnt = 0;
	char search_word[32];
	Node *search_result;
	cout << "输入查找的单词(空行结束输入): ";
	cin.getline(search_word, 32);
	while(search_word[0]){
		cnt++;
		begin = (int)clock();
		search_result = searchBST(T, search_word);
		end = (int)clock();
		sumTime += end - begin;
		if(search_result){
			display_result(search_result);
		}
		else{
			cout << "抱歉没有找到" << endl;
		}
		cout << "输入查找单词(空行结束输入): ";
		cin.getline(search_word, 32);
	}
	return cnt ? sumTime / cnt : -1;//一次也没有输入 返回-1
}
