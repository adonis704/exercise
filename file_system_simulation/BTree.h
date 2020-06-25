#ifndef BTREE_BTREE_H
#define BTREE_BTREE_H

#endif //BTREE_BTREE_H

#include "BTreeNode.h"

typedef struct BTree{
    int _m; //阶数
    int _min;   //除根节点外每个节点应具有的最小节点数
    int _height;    //树高

    KeyNode *minNode;   //最小的叶子节点关键码
    BaseNode *root; //根节点

    KeyNode* (*insertNode)(struct BTree *this, int key, int data);
    int (*searchNode)(struct BTree *this, int key);
    void (*removeNode)(struct BTree *this, int key);
    void (*solveOverFlow)(struct BTree *this, BaseNode* target);
    void (*replace)(struct BTree *this, struct BaseNode *target, int keyOld, int keyNew);
    void (*solveUnderFlow)(struct BTree *this, struct BaseNode *target);
} BTree;
//构造函数
BTree* newBTree(int m);
//在树中插入叶子节点，输入：关键码值、数据，插入并返回
KeyNode* insertNode(struct BTree *this, int key, int data);
//在树中找到关键码值所存储的数据，输入：关键码值，返回：数据
int searchNode(struct BTree *this, int key);
//在树中移除关键码及其数据
void removeNode(struct BTree *this, int key);
//解决上溢问题，输入可能发生上溢的节点
void solveOverFlow(struct BTree *this, BaseNode* target);
//替换关键码，输入索引节点，待替换的关键码和新关键码
void replace(struct BTree *this, struct BaseNode *target, int keyOld, int keyNew);
//解决下溢问题，输入可能发生下溢的节点
void solveUnderFlow(struct BTree *this, struct BaseNode *target);