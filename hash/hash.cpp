#include <iostream>
#include <fstream>
#include "hash.h"
using namespace std;

/*初始化hash表*/
void initHashTable(HashTable &T){
	T.size = TABLE_SIZE;
	T.data = new pNode[TABLE_SIZE];
	for(int i = 0; i < TABLE_SIZE; ++i){
		T.data[i] = 0;// all pNode = 0，means empty slot
	}
}

//BKDRHash- Kernighan and Dennis
unsigned int bkdrHash(string word){
	if(word.size() == 0) return 0;//空字符串返回0
	const int seed = 131;
	unsigned int hash = 0;
	for(int i = 0; i < word.length(); ++i){
		hash = hash * seed + word[i];
	}
	return hash % TABLE_SIZE;
	//这里的TABLE_SIZE不使用宏，使用const常量
	//否则可能会因为类型混乱，部分0位没有被清理掉，造成hash出错
}

unsigned int hash(string word, int select){
	switch(select){
	case 1: return bkdrHash(word);
	}
}

/*创建结点*/
extern int word_cnt;
pNode createNode(string word, unsigned int line, unsigned int offset){
	word_cnt++;
	Node *p = new Node;
	p->word = word;
	p->next = 0;
	(p->pos).line = line;
	(p->pos).offset = offset;
	(p->pos).next = 0;
	return p;
}

/*加入位置信息*/
void addPosition(pNode node, unsigned int line, unsigned int offset){
	Position *new_pos = new Position;
	new_pos->line = line;
	new_pos->offset = offset;
	new_pos->next = (node->pos).next;
	(node->pos).next = new_pos;
}

/*插入单词数据*/
void insertHashTable(HashTable &T, string word, unsigned int line, unsigned int offset){
	unsigned int hash = bkdrHash(word);
	if(!T.data[hash]){//empty slot
		T.data[hash] = createNode(word, line, offset);
	}
	else{
		pNode p = T.data[hash];
		while(p && p->word != word){
			p = p->next;
		}
		if(p){//同名单词
			addPosition(p, line, offset);
		}
		else{//解决冲突问题
			pNode q = createNode(word, line, offset);
			q->next = T.data[hash]->next;
			T.data[hash]->next = q;
		}
	}
}

/*销毁位置链*/
void destoryPosition(Position &pos){
	Position *p = pos.next, *q;
	while(p){
		q = p;
		p = p->next;
		delete q;
	}
}

/*销毁单词结点链*/
void destoryNodeList(Node *pNode){
	Node *p = pNode, *q;
	while(p){
		q = p;
		p = p->next;
		delete q;
	}
}

/*销毁HashTable*/
void destoryHashTable(HashTable &T){
	for(unsigned int i = 0; i < TABLE_SIZE; ++i){
		destoryNodeList(T.data[i]);
	}
	delete[] T.data;
}

/*将某个hash slot清空*/
void deleteHash(HashTable &T, string word){
	unsigned int hash = bkdrHash(word);
	if(T.data[hash]){
		destoryNodeList(T.data[hash]);
		T.data[hash] = 0;
	}
}

/*搜索单词项*/
pNode searchHashTable(HashTable &T, string word){
	unsigned int hash = bkdrHash(word);
	if(!T.data[hash]){
		return 0;
	}
	else{
		pNode p = T.data[hash];
		while(p && p->word != word){
			p = p->next;
		}
		return p;
	}
}

/*打印出搜索结果*/
void displayResult(Node *node){
	int cnt = 1;
	cout << "单词" << node->word << "位置信息(行数_行内偏移)\n";
	Position *p = (node->pos).next;
	while(p){
		cout << p->line << "_" << p->offset << endl;
		p = p->next;
		cnt++;
	}
	cout << node->pos.line << "_" << node->pos.offset << endl;
	cout << "总出现次数：" << cnt << endl;
}


/*使用文件保存hash表*/
void saveIndexFile(HashTable &T, char *filename){
	ofstream fout(filename, ios::out);
	for(int i = 0; i < TABLE_SIZE; ++i){
		if(T.data[i]){//当存在这个坑的时候
			fout << i << ":";    //保存hash值
			fout << T.data[i]->word << endl;//保存单词结点
			Position *p = ((T.data[i])->pos).next;
			while(p){
				fout << p->line << "_" << p->offset << "+";
				p = p->next;
			}
			fout << ((T.data[i])->pos).line << "_" << ((T.data[i])->pos.offset) << endl;
		}
	}
	fout.close();
}
