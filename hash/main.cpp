#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fstream>
#include <string>
#include "hash.h"
using namespace std;
int word_cnt;
char *f_index = 0;
char *f_src = 0;
int haveIndexFile = 0;

char * change_to_lower(char *str);
void word_token(HashTable &T, char *str, unsigned int line);
void dispTitle();
void getTableFromSrc(HashTable &T, char *f_src);
void addAllPosition(HashTable &T, int hash, string word, string buf);
void getTableFromIndex(HashTable &T, char *f_index);
double searchDictionary(HashTable &T);


int main(int argc, char *argv[]){
	/*解析命令行参数*/
	int ch;
	while((ch = getopt(argc, argv, "i:s:h")) != -1){
		switch(ch){
		case 'i': haveIndexFile = 1; f_index = optarg; break;
		case 's': haveIndexFile = 0; f_src = optarg; break;
		case 'h': dispTitle(); break;
		}
	}

	/*建立哈希表*/
	HashTable T;
	initHashTable(T);
	if(haveIndexFile){
		getTableFromIndex(T, f_index);
	}
	else{
		getTableFromSrc(T, f_src);
	}
	cout << "创建hash表完成" << endl;
	cout << word_cnt << endl;

	/*查找操作*/
	double averageTime = searchDictionary(T);
	cout << "平均查找时间: " << averageTime << "ms" << endl;
	/*保存数据结构到索引文件中*/
	if(!haveIndexFile){
		saveIndexFile(T, (char *)"index.dat");
	}

	/*销毁HashTable*/
	destoryHashTable(T);

	return 0;
}

/*==========================================*/

//将字符串转换为小写
char * change_to_lower(char *str){
	for(unsigned int i = 0; i < strlen(str); ++i){
		str[i] = tolower(str[i]);
	}
	return str;
}
//从一整行文本中分离出单词
void word_token(HashTable &T, char *str, unsigned int line){
	char *p = strtok(str, " \":,.?!-\\(\\)';1234567890");
	int offset = 0;
	while(p){
		cout << p << "@\n";
		insertHashTable(T, change_to_lower(p), line, offset);
		offset++;
		p = strtok(NULL, " \":,.?!-\\(\\)';1234567890");
	}
}
//显示提示部分
void dispTitle(){
	cout << "\t-i file: 输入倒排索引文件\n";
	cout << "\t-s file: 输入源文件\n";
	cout << "\t-h 打开帮助" << endl;
	exit(0);
}
/*从源文件中获取数据*/
void getTableFromSrc(HashTable &T, char *f_src){
	ifstream fin(f_src);
	char buf[1024];
	unsigned int line = 0;
	while(!fin.eof()){
		fin.getline(buf, 1024); line++;
		word_token(T, buf, line);
	}
	fin.close();
}
int change(string buf){
	return atoi(buf.c_str());
}

/*从索引文件中获取数据*/
void getTableFromIndex(HashTable &T, char *f_index){
	fstream fin(f_index);
	string buf;
	string word;
	int hash;
	while(!fin.eof()){
		getline(fin, buf, ':');
		hash = change(buf);
		getline(fin, word);
		cout << hash << endl;
		cout << word << endl;
		getline(fin, buf);		//位置信息
		cout << buf << endl;
		addAllPosition(T, hash, word, buf);
	}
	fin.close();
}
/*将结点数据恢复到HashTable中去*/
void addAllPosition(HashTable &T, int hash, string word, string buf){
	char *pc = (char *)buf.c_str();
	int n = buf.size();
	unsigned int line = 0, offset = 0;
	int tmp = 0, num = 0, cnt = 0;

	int i = 0;
	while(i <= n){
		if(pc[i] != '_' && pc[i] != '+'){
			tmp = pc[i] - '0';
			num = 10 * num + tmp;
		}
		else{
			cnt++;
			if(cnt % 2) line = num;
			else offset = num;
			tmp = num = 0;
			if(cnt == 2){
				T.data[hash] = createNode(word, line, offset);
			}
			if(cnt % 2 == 0 && cnt > 3){
				addPosition(T.data[hash], line, offset);
			}
		}
		i++;
	}
	cnt++;
	if(cnt == 2){
		T.data[hash] = createNode(word, line, offset);
	}
	if(cnt % 2 == 0 && cnt > 3){
		addPosition(T.data[hash], line, offset);
	}

}

/*单词的查找操作*/
double searchDictionary(HashTable &T){
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
		search_result = searchHashTable(T, search_word);
		end = (int)clock();
		sumTime += end - begin;
		if(search_result){
			displayResult(search_result);
		}
		else{
			cout << "抱歉没有找到" << endl;
		}
		cout << "输入查找单词(空行结束输入): ";
		cin.getline(search_word, 32);
	}
	return cnt ? sumTime / cnt : -1;//一次也没有输入 返回-1
}
