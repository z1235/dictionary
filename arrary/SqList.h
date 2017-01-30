#ifndef ARRARY_H
#define ARRARY_H

#include <string>
using namespace std;

const int INIT_SIZE = 1024;

//位置链表
typedef struct Position{
	int line;
	int offset;
	struct Position *next;
}Position;

typedef struct Node{
	string word;
	Position pos;
}Node;

//静态查找表
typedef struct Table{
	Node *data;
	int capacity;
	int cnt;
}Table;

void createSqList(Table &T);
void expand(Table &T);
void addSqList(Table &T, string word, unsigned int line, unsigned int offset);
void traverse(Table &T);
void search_unsort(Table &T, string word);
int partiction(Node *data, int lo, int hi);
void quickSort(Node *data, int lo, int hi);
void sort(Table &T);

Node * bin_search(Table &T, string word);
void display_result(Node *node);





#endif
