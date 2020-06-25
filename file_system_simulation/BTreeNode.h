#ifndef BTREE_BTREENODE_H
#define BTREE_BTREENODE_H

#endif //BTREE_BTREENODE_H
#ifndef NULL
#define NULL 0
#endif

#include <malloc.h>

typedef enum Type{
    ROOT = 0,   //节点为根节点
    INDEX = 1,  //节点为索引节点，或关键码为索引
    LEAF = 2,   //节点为叶子节点，或关键码为叶子
    NILL = 3,   //表示不存在
} Type;

//用链表串联关键码结构，因此关键码是链表的一个节点
typedef struct KeyNode{
    int key;    //关键码id
    Type type;  //关键码类型
    int data;   //若为叶子关键码则存放数据块在磁盘上的地址，否则为-1
    struct BaseNode *child; //若为索引关键码，则指向下一级I/O
    struct KeyNode *pred;   //前继
    struct KeyNode *succ;   //后继

    struct KeyNode* (*insertAsPred)(struct KeyNode *this, int key, Type type, int data, struct BaseNode *child);
    struct KeyNode* (*insertAsSucc)(struct KeyNode *this, int key, Type type, int data, struct BaseNode *child);
} KeyNode;

//用链表实现节点结构
typedef struct BaseNode{
    int _size;  //链表大小
    Type type;  //节点的类型，是根节点、索引节点、还是叶子节点
    struct BaseNode *parent;    //节点的父亲
    struct BaseNode *brother;   //节点的兄弟，只在叶子节点中有效
    struct KeyNode *header; //头节点
    struct KeyNode* trailer;    //尾节点

    struct KeyNode* (*searchKeyNode)(struct BaseNode *this, int key);
    struct KeyNode* (*insertKeyNode)(struct BaseNode *this, int key,Type type, int data, struct BaseNode* child);
    void (*append)(struct BaseNode *this, struct KeyNode *target);
    void (*appendAhead)(struct BaseNode *this, struct KeyNode *target);
    void (*removeKeyNode)(struct BaseNode *this, int key);
} BaseNode;

//构造函数
KeyNode* newKeyNode(int key, Type type, int data, BaseNode *child, KeyNode *pred, KeyNode *succ);
BaseNode* newBaseNode(int m, Type type, BaseNode *parent, BaseNode *brother);
//在前面或后面插入关键码
struct KeyNode* insertAsPred(struct KeyNode *this, int key, Type type, int data, struct BaseNode *child);
struct KeyNode* insertAsSucc(struct KeyNode *this, int key, Type type, int data, struct BaseNode *child);

//输入：关键码值，返回最大的但不大于该关键码值的关键码结构
KeyNode* searchKeyNode(struct BaseNode *this, int key);
//输入：关键码值、类型、数据、孩子，将关键码插入到对应节点，并返回
struct KeyNode* insertKeyNode(struct BaseNode *this, int key, Type type, int data, BaseNode* child);
//将关键码附到节点末尾或首部
void append(struct BaseNode *this, struct KeyNode *target);
void appendAhead(struct BaseNode *this, struct KeyNode *target);
//输入：关键码值，删除该关键码
void removeKeyNode(struct BaseNode *this, int key);
