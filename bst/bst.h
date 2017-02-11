#ifndef BST_H
#define BST_H

#include <string>
using namespace std;

typedef struct Position{
	unsigned int line;
	unsigned int offset;
	Position *next;
}Position;

typedef struct Node{
	string word;
	Position pos;
	struct Node *lchild;
	struct Node *rchild;	
	struct Node *parent;
}Node, *Bstree;

void initBST(Bstree &T);
void insertBST(Bstree &T, string word, unsigned int line, unsigned int offset);
void inOrderBST(Bstree &T);/*中序遍历binary search tree*/
Node * searchBST(Bstree &T, string word);
Node * createNode(string word, unsigned int line, unsigned int offset);
void addPosition(Node *pNode, unsigned int line, unsigned int offset);
Node * searchBST(Bstree &T, string word);
void display_result(Node *node);

void destoryPosition(Position &pos);
void destoryNode(Node *pNode);
void destoryBST(Bstree &T);

Node * findMinInTree(Node *p);
void deleteNode(Node *pNode);
void deleteBST(Bstree &T, string word);

void saveIndexFile(Bstree &T);
#endif
