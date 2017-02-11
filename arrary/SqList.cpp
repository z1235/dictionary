#include <iostream>
#include <fstream>
#include "SqList.h"
using namespace std;

void createSqList(Table &T){
	T.capacity = INIT_SIZE;
	T.cnt = 0;
	T.data = new Node[INIT_SIZE];
}

void expand(Table &T){
	Node *p = T.data;
	T.data = new Node[T.capacity <<= 1];
	for(int i = 0; i < T.cnt; ++i){
		T.data[i] = p[i];
	}
}

/*向表中添加一个单词结点*/
void addSqList(Table &T, string word, unsigned int line, unsigned int offset){
	int i;
	if(T.capacity <= T.cnt) expand(T);
	for(i = 0; i < T.cnt; ++i){
		if(word == T.data[i].word) break;
	}
	if(i == T.cnt){
		T.data[i].word = word;
		T.data[i].pos.line = line;
		T.data[i].pos.offset = offset;
		T.data[i].pos.next = NULL;
		T.cnt++;
	}
	else{
		Position *p = new Position;
		p->line = line;
		p->offset = offset;
		p->next = T.data[i].pos.next;
		T.data[i].pos.next = p;
	}
}
/*重载 向表中添加单词结点，只是此时不能确定单词的行数和行内偏移*/
void addSqList(Table &T, string word){
	if(T.capacity <= T.cnt) expand(T);
	T.data[T.cnt].word = word;
	T.cnt++;
}

void traverse(Table &T){
	for(int i = 0; i < T.cnt; ++i){
		cout << T.data[i].word << endl;
	}
}

void display_result(Node *node){
	int cnt = 1;
	cout << "单词" << node->word << "位置信息(行数_行内偏移):\n";
	Position *p = (node->pos).next;
	while(p){
		cnt++;
		cout << p->line << "_" << p->offset << endl;
		p =  p->next;
	}
	cout << node->pos.line << "_" << node->pos.offset << endl;
	cout << "单词频度:" << cnt << endl;
}

/*暴力查找*/
Node * search_unsort(Table &T, string word){
	for(int i = 0; i < T.cnt; ++i){
		if(T.data[i].word == word) return &(T.data[i]);
	}
	return NULL;
}

/*二分查找*/
Node * bin_search(Table &T, string word){
	unsigned int lo = 0, hi = T.cnt;
	while(lo < hi){
		unsigned int mid =  lo + ((hi - lo) >> 1);
		if(T.data[mid].word == word) return &(T.data[mid]);
		else if(T.data[mid].word < word) lo = mid + 1;
		else hi = mid;
	}
	return NULL;
}

/*划分算法 pivot为第一个元素*/
int partiction(Node *data, int lo, int hi){
	int i = lo, j = hi - 1;
	Node key = data[lo];
	while(i < j){
		while(i < j && key.word <= data[j].word) j--;
		if(key.word > data[j].word){
			data[i] = data[j];
		}
		while(i < j && data[i].word <= key.word) i++;
		if(key.word < data[i].word){
			data[j] = data[i];
		}
	}
	data[i] = key;
	return i;
}

void quickSort(Node *data, int lo, int hi){
	if(hi - lo < 2) return;
	else{
		int pivot = partiction(data, lo, hi);
		quickSort(data, lo, pivot);
		quickSort(data, pivot + 1, hi);
	}
}

//快速排序算法
void sortTable(Table &T){
	quickSort(T.data, 0, T.cnt);
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

/*销毁单词结点*/
void destoryNode(Node &pNode){
	destoryPosition(pNode.pos);
}

/*销毁静态查找表*/
void destorySqList(Table &T){
	for(int i = 0; i < T.cnt; ++i){
		destoryNode(T.data[i]);
	}
	delete[] T.data;
}

/*保存到索引文件中*/
void saveIndexFile(Table &T, char *filename){
	ofstream fout(filename, ios::out);
	for(int i = 0; i < T.cnt; ++i){
		fout << T.data[i].word << endl;
		Position *p = (T.data[i].pos).next;
		while(p){
			fout << p->line << "_" << p->offset << "+";
			p =  p->next;
		}
		fout << T.data[i].pos.line << "_" << T.data[i].pos.offset << endl;
	}
	fout.close();
}
