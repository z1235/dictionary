#include <iostream>
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

void traverse(Table &T){
	for(int i = 0; i < T.cnt; ++i){
		cout << T.data[i].word << endl;
	}
}

void display_result(Node *node){
	cout << "单词" << node->word << "位置信息(行数_行内偏移):\n";
	Position *p = (node->pos).next;
	while(p){
		cout << p->line << "_" << p->offset << endl;
		p =  p->next;
	}
	cout << node->pos.line << "_" << node->pos.offset << endl;
}

//遍历搜索
void search_unsort(Table &T, string word){
	int i;
	for(i = 0; i < T.cnt; ++i){
		if(T.data[i].word == word) break;
	}
	if(i == T.cnt){
		cout << "抱歉，没有找到!\n";
	}
	else{
		display_result(&T.data[i]);
	}
}

//二分搜索
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
void sort(Table &T){
	quickSort(T.data, 0, T.cnt);
}
