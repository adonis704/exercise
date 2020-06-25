#include "BTreeNode.h"

struct KeyNode* insertAsPred(struct KeyNode *this, int key, Type type, int data, BaseNode *child){
    KeyNode *newNode = newKeyNode(key, type, data, child, this->pred, this);
    this->pred->succ = newNode;
    this->pred = newNode;

    return newNode;
}
struct KeyNode* insertAsSucc(struct KeyNode *this, int key, Type type, int data, BaseNode *child){
    KeyNode *newNode = newKeyNode(key, type, data, child, this, this->succ);
    this->succ->pred = newNode;
    this->succ = newNode;

    return newNode;
}

KeyNode* searchKeyNode(struct BaseNode *this, int key){
    if(this->_size == 0)    return this->trailer;   //若节点大小为0，则直接返回

    //从第一个节点开始，依次遍历
    KeyNode* current = this->header->succ;
    while(current->key > -1){  //到尾节点退出
        if(current->key >= key) return current; //找到第一个大于或等于关键码值的关键码并返回
        current = current->succ;
    }
    return current->pred;   //返回最后一个节点
}
struct KeyNode* insertKeyNode(struct BaseNode *this, int key, Type type, int data, BaseNode* child){
    if(this->_size == 0) {
        this->_size = 1;
        return this->header->insertAsSucc(this->header, key, type, data, child);
    }  //若节点大小为0，则简单插入即可

    KeyNode *current = this->header->succ;
    while(current->key >-1){
        if(current->key == key) return this->trailer;   //存在则不插入
        if(current->key > key) break;   //找到第一个大于关键码值的关键码
        current = current->succ;
    }
    this->_size++;
    return current->insertAsPred(current, key, type, data, child);  //将关键码作为前继插入
}
void append(struct BaseNode *this, struct KeyNode *target){
    this->_size++;
    target->pred = this->trailer->pred;
    target->succ = this->trailer;
    this->trailer->pred->succ = target;
    this->trailer->pred = target;
}
void appendAhead(struct BaseNode *this, struct KeyNode *target){
    this->_size++;
    target->pred = this->header;
    target->succ = this->header->succ;
    this->header->succ->pred = target;
    this->header->succ = target;
}
void removeKeyNode(struct BaseNode *this, int key){
    KeyNode *target = this->searchKeyNode(this, key);
    if(target->key != key)  return;  //不存在则无法移除

    target->pred->succ = target->succ;
    target->succ->pred = target->pred;
    this->_size--;
    free(target);
    target = NULL;
}

KeyNode* newKeyNode(int key, Type type, int data, BaseNode *child, KeyNode *pred, KeyNode *succ){
    KeyNode *this = NULL;
    this = (KeyNode*)malloc(sizeof(KeyNode));
    this->key = key;
    this->type = type;
    this->data = data;
    this->child = child;
    this->pred = pred;
    this->succ = succ;

    this->insertAsPred = insertAsPred;
    this->insertAsSucc = insertAsSucc;
    return this;
}
BaseNode* newBaseNode(int m, Type type, BaseNode *parent, BaseNode *brother){
    BaseNode *this = NULL;
    this = (BaseNode*)malloc(m * sizeof(KeyNode));

    KeyNode *header = newKeyNode(-1, NILL, -1,  NULL, NULL, NULL);
    KeyNode *trailer = newKeyNode(-1, NILL, -1, NULL, NULL, NULL);
    header->succ = trailer;
    trailer->pred = header;

    this->header = header;
    this->trailer = trailer;
    this->parent = parent;
    this->brother = brother;
    this->type = type;
    this->_size = 0;

    this->searchKeyNode = searchKeyNode;
    this->insertKeyNode = insertKeyNode;
    this->removeKeyNode = removeKeyNode;
    this->append = append;
    this->appendAhead = appendAhead;

    return this;
}
