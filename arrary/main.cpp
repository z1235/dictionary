#include <iostream>
#include <unistd.h>
#include <string.h>
#include <string>
#include <fstream>
#include "SqList.h"
#include <stdlib.h>
#include <time.h>
using namespace std;

char *f_index = 0;
char *f_src = 0;
int haveIndexFile = 0;//是否存在倒排索引文件
int sort = 1;	//是否进行排序

double searchDictionary(Table &T, int sort);
char * change_to_lower(char *str);
void word_token(Table &T, char *str, unsigned int line);
void dispTitle();
void getTableFromSrc(Table &T, char *f_src);
void getTableFromIndex(Table &T, char *f_index);	//索引文件中获取数据
void addPosition(Node &node, string buf);


int main(int argc, char * argv[]){
	/*解析参数字符串*/
	int ch;
	while((ch = getopt(argc, argv, "i:s:uh")) != -1){
		switch(ch){
		case 'i': haveIndexFile = 1; f_index = optarg;  break;//是否有倒排索引文件
		case 's': haveIndexFile = 0; f_src = optarg; break;//输入源文件
		case 'u': sort = 0; break;			//是否禁止排序
		case 'h': dispTitle(); break;		//打印帮助
		}
	}

	/*建立索引表*/
	Table T;
	createSqList(T);
	if(haveIndexFile){
		getTableFromIndex(T, f_index);
	}
	else{
		getTableFromSrc(T, f_src);
	}

	/*对静态查找表进行排序*/
	if(sort){
		sortTable(T);
		traverse(T);
	}

	/*查找操作*/
	double averageTime = searchDictionary(T, sort);
	cout << "平均查找时间: " << averageTime << "ms" << endl;

	/*保存数据到索引文件中*/
	if(!haveIndexFile){
		saveIndexFile(T, (char *)"index.dat");
	}

	/*销毁静态查找表*/
	destorySqList(T);
	return 0;
}

/*==========================================================================*/
//将字符串转换为小写
char * change_to_lower(char *str){
	for(unsigned int i = 0; i < strlen(str); ++i)
		str[i] = tolower(str[i]);
	return str;
}
/*查找单词操作*/
double searchDictionary(Table &T, int sort){
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
		if(sort){
			search_result = bin_search(T, search_word);
		}
		else{
			search_result = search_unsort(T, search_word);
		}
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
	return cnt ? sumTime / cnt : -1;
}
//从一整行文本中分离出单词
void word_token(Table &T, char *str, unsigned int line){
	char *p = strtok(str, " \":,.?!-\\(\\)][/';1234567890");
	int offset = 0;
	while(p){
		cout << p << "@\n";
		addSqList(T, change_to_lower(p), line, offset);
		offset++;
		p = strtok(NULL, " \":,.?!-\\(\\)'/][;1234567890");
	}
}
void dispTitle(){
	cout << "\t-i file: 输入倒排索引文件\n";
	cout << "\t-s file: 输入源文件\n";
	cout << "\t-u 关闭排序,使用暴力搜索\n";
	cout << "\t-h 打开帮助" << endl;
	exit(0);
}

/*从源文件中获取数据信息*/
void getTableFromSrc(Table &T, char *f_src){
	ifstream fin(f_src);
	char buf[1024];
	unsigned int line = 0;
	while(!fin.eof()){
		fin.getline(buf, 1024); line++;
		word_token(T, buf, line);
	}
	fin.close();
}

/*从索引文件中恢复数据结构*/
void getTableFromIndex(Table &T, char *f_index){
	fstream fin(f_index);
	string buf;
	while(!fin.eof()){
		getline(fin, buf);
		addSqList(T, buf);//添加单词结点
		cout << buf << endl;
		getline(fin, buf);
		addPosition(T.data[T.cnt - 1], buf);
	}
	fin.close();
}


/*将字符串中单词结点的位置信息加入其中*/
void addPosition(Node &node, string buf) {
	char *pc = (char *)buf.c_str();
	int n = buf.size();

	int line = 0, offset = 0;
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
				node.pos.line = line;
				node.pos.offset = offset;
				node.pos.next = 0;
			}
			cout << line << "_" << offset << endl;
			if(cnt % 2 == 0&& cnt > 3){
				Position *pos = new Position;
				pos->line = line;
				pos->offset = offset;
				pos->next = node.pos.next;
				node.pos.next = pos;
			}
		}
		i++;
	}
	cnt++;
	if(cnt == 2){
		node.pos.line = line;
		node.pos.offset = offset;
		node.pos.next = 0;
	}
	cout << line << "_" << offset << endl;
	if(cnt % 2 == 0&& cnt > 3){
		Position *pos = new Position;
		pos->line = line;
		pos->offset = offset;
		pos->next = node.pos.next;
		node.pos.next = pos;
	}
	
}





