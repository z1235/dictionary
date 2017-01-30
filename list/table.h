#ifndef TABLE_H
#define TABLE_H

typedef struct Position{
	unsigned int line;	//行号
	unsigned int offset;
	struct Position *next;	//下一个位置
}Position;

typedef struct Node{
	char word[32];		//单词	
	Position pos;		//位置链表
	struct Node *next;	//指向下一个表项
}Node, *List;

void initList(List *T);		
void addword(Node *p, char word, unsigned int line, unsigned int offset);
void addpos(Node *p, unsigned int line, unsigned int offset);
void addList(List T, char *word, unsigned int line, unsigned offset);
void traverse(List T, char *filename, void (*visit)(Node *, char *));
void search_word(List T, void(*display)(Node *));
void reback(List T, char *filename);

char * change_to_lower(char *str);
void word_token(List L, char *str, unsigned line);
void build_dictionary(List L, char *srcfile);

#endif
