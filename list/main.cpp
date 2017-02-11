#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include "table.h"
using namespace std;
char f_bible[] = "bible";
char f_test[] = "test";
char f_reback[] = "index";
List word_list;	//单词链表

//显示查找结果使用的函数
void display_search(Node *n){
	unsigned int m_line, page;
	unsigned int cnt = 1;
	if(n){
		cout << n->word << "出现位置:\n";
		cout << "====================\n";
		Position *p = (n->pos).next;
		while(p){
			m_line = (p->line) % 128;//页内行号
			page = (p->line) / 128;//页号
			cout << setw(12) << "页号: " << setw(6) << page 
				<< setw(12) << " 行号: " << setw(6) << m_line 
				<< setw(12) << "  行内偏移: " << setw(6) << p->offset << endl;
			cnt++;
			p = p->next;
		}
		m_line = (n->pos).line % 128;
		page = (n->pos).offset / 128;
		cout << setw(12) << "页号: " << setw(6) << page 
			<< setw(12) << " 行号: " << setw(6) << m_line 
			<< setw(12) << "  行内偏移: " << setw(6) << (n->pos).offset << endl;
		cout << "单词频度: " << cnt << endl;
	}
	else{
		cout << "抱歉，没有找到这个单词！" << endl;
	}
}

//遍历使用的访问函数, 保存倒排索引文件
void visit(Node *p, char *filename){
	ofstream fout(filename, ios::app | ios::out);
	fout << p->word << ":";
	Position *pos = (p->pos).next;
	while(pos){
		fout << pos->line << "_" << pos->offset << " ";
		pos = pos->next;
	}
	fout << (p->pos).line << "_" << (p->pos).line << " \n";
	fout.close();
}


//---主函数---
int main(){
	initList(&word_list);
	build_dictionary(word_list, f_bible);
	traverse(word_list, f_reback, visit);
	search_word(word_list, display_search);
	return 0;
}
