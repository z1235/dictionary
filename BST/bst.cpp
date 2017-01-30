#include "bst.h"
#include <string>
#include <iostream>
using namespace std;

void initBST(Bstree &T){
	T = 0; 
}

/*新建一个结点*/
Node * createNode(string word, unsigned int line, unsigned int offset){
	Node *p = new Node;
	p->word = word;
	p->lchild = p->rchild = p->parent = 0;
	(p->pos).line = line;
	(p->pos).offset = offset;
	(p->pos).next = 0;
	return p;
	
}

/*给一个结点添加其余位置信息*/
void addPosition(Node *pNode, unsigned int line, unsigned int offset){
	Position *p = new Position;
	p->line = line;
	p->offset = offset;
	p->next = (pNode->pos).next;
	(pNode->pos).next = p;
}

/*将一个数据项加入到binary search tree中*/
void insertBST(Bstree &T, string word, unsigned int line, unsigned int offset){
	if(T){
		if(T->word < word){
			if(T->rchild){
				insertBST(T->rchild, word, line, offset);
			}
			else{//右子树为空
				Node *pNode = createNode(word, line, offset);
				T->rchild = pNode;
			}
		}
		else if(word < (T->word)){
			if(T->lchild){
				insertBST(T->lchild, word, line, offset);
			}
			else{//左子树为空
				Node *pNode = createNode(word, line, offset);
				T->lchild = pNode;
			}
		}
		else{//更新结点添加位置信息
			addPosition(T, line, offset);
		}
	}
	else{ //空树
		Node *pNode = createNode(word, line, offset);
		T = pNode;
	}
}

void inOrderBST(Bstree &T){
	if(T){
		inOrderBST(T->lchild);
		cout << T->word << "\n";
		inOrderBST(T->rchild);
	}
}

Node * searchBST(Bstree &T, string word){
	if(T){
		if(T->word == word) return T;
		else if(T->word < word) return searchBST(T->rchild, word);
		else return searchBST(T->lchild, word);
	}
	else
		return 0;
}

void display_result(Node *node){
	cout << "单词" << node->word << "位置信息(行数_行内偏移)\n";
	Position *p = (node->pos).next;
	while(p){
		cout << p->line << "_" << p->offset << endl;
		p = p->next;
	}
	cout << node->pos.line << "_" << node->pos.offset << endl;
}
