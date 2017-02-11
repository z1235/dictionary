#ifndef HASH_H
#define HASH_H

#include <string>
using namespace std;

const int TABLE_SIZE = 19997;

/*位置链*/
typedef struct Position{
	unsigned int line;
	unsigned int offset;
	struct Position *next;
}Position;

/*单词结点*/
typedef struct Node{
	string word;
	Position pos;
	struct Node *next;//链地址解决冲突
}Node, *pNode;

/*hash表*/
typedef struct HashTable{
	int size;
	pNode *data;
}HashTable;

void initHashTable(HashTable &T);
unsigned int bkdrHash(string word);
pNode createNode(string word, unsigned int line, unsigned int offset);
void addPosition(pNode node, unsigned int line, unsigned int offset);
void insertHashTable(HashTable &T, string word, unsigned int line, unsigned int offset);

pNode searchHashTable(HashTable &T, string word);
void displayResult(Node *node);
void destoryPosition(Position &pos);
void destoryNodeList(Node *pNode);
void destoryHashTable(HashTable &T);
void deleteHash(HashTable &T, string word);

void saveIndexFile(HashTable &T, char *filename);


#endif
