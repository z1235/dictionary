#include "bst.h"
#include <fstream>
#include <string>
#include <queue>
#include <iostream>
using namespace std;

void initBST(Bstree &T){
	T = 0; 
}

extern unsigned int word_cnt;
/*新建一个结点*/
Node * createNode(string word, unsigned int line, unsigned int offset){
	word_cnt++;
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
				pNode->parent = T;
				T->rchild = pNode;
			}
		}
		else if(word < (T->word)){
			if(T->lchild){
				insertBST(T->lchild, word, line, offset);
			}
			else{//左子树为空
				Node *pNode = createNode(word, line, offset);
				pNode->parent = T;
				T->lchild = pNode;
			}
		}
		else{//更新结点添加位置信息
			addPosition(T, line, offset);
		}
	}
	else{ //空树
		Node *pNode = createNode(word, line, offset);
		pNode->parent = 0;
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


/*销毁位置链*/
void destoryPosition(Position &pos){
	Position *p = pos.next, *q;
	while(p){
		q = p;
		p = p->next;
		delete q;
	}
}

/*销毁结点*/
void destoryNode(Node *pNode){
	destoryPosition(pNode->pos);
	delete pNode; pNode = 0;
}


void destoryBST(Bstree &T){
	if(T){
		destoryBST(T->lchild);
		destoryBST(T->rchild);
		destoryNode(T);
	}
}

Node * findMinInTree(Node *p){
	if(!p) return 0; //空树直接返回
	
	if(p->lchild){
		return findMinInTree(p->lchild);
	}
	else{
		return p;
	}
}


//删除结点
void deleteNode(Node *pNode){
	if(pNode->lchild){
		if(pNode->rchild){
			Node *p = findMinInTree(pNode->rchild);//寻找直接后继
			pNode->word = p->word;
			pNode->pos = p->pos;
			deleteNode(p);
		}
		else{//左孩子不空，右孩子空
			pNode->word = (pNode->lchild)->word;
			pNode->pos = (pNode->lchild)->pos;
			delete pNode->lchild; pNode->lchild = 0;
		}
	}
	else{
		if(pNode->rchild){//左孩子空，右孩子不空
			pNode->word = (pNode->rchild)->word;
			pNode->pos = (pNode->rchild)->pos;
			delete pNode->rchild; pNode->rchild = 0;
		}
		else{//左右孩子空
			if(pNode->parent->lchild == pNode) pNode->parent->lchild = 0;
			else pNode->parent->rchild = 0;
			delete pNode; pNode = 0;
		}
	}
}

void deleteBST(Bstree &T, string word){
	Node *p = searchBST(T, word);
	if(p){
		deleteNode(p);//删除结点
	}
}

/*将查找树保存到文件中，便于恢复, 采用先序遍历*/
extern ofstream fout_index;
void saveWordNode(Node *pNode){
	fout_index << pNode->word << endl;
	Position *p = (pNode->pos).next;
	while(p){
		fout_index << p->line << "_" << p->offset;
		p = p->next;
	}
	fout_index << (pNode->pos).line << "_" << (pNode->pos).offset << endl;
}
void saveIndexFile(Bstree &T){
	if(!T) return;
	Node *p = 0;
	queue<Bstree> Q;
	Q.push(T);
	while(!Q.empty()){
		p = Q.front(); Q.pop();
		saveWordNode(p);
		if(p->lchild) Q.push(p->lchild);
		if(p->rchild) Q.push(p->rchild);
	}
}
