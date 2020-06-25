#include "BTree.h"

BTree* newBTree(int m){
    BTree *this = (BTree*)malloc(sizeof(BTree));
    this->_m = m;
    this->_min = (m-1)/2 +1;    //向上取整
    this->_height = 0;
    this->minNode = NULL;
    this->root = newBaseNode(m, ROOT, NULL, NULL);

    this->searchNode = searchNode;
    this->insertNode = insertNode;
    this->removeNode = removeNode;
    this->solveOverFlow = solveOverFlow;
    this->replace = replace;
    this->solveUnderFlow = solveUnderFlow;

    return this;
}

KeyNode* insertNode(struct BTree *this, int key, int data){
    if(this->_height == 0)  {   //一个节点也没有，则直接在根节点插入
        this->_height = 1;
        this->minNode = this->root->insertKeyNode(this->root, key, LEAF, data, NULL);
        return this->minNode;
    }
    if(this->searchNode(this, key) != -1)   return this->root->trailer; //存在则不插入

    //从根节点开始遍历
    int cnt = 1;
    BaseNode *current = this->root;
    KeyNode *tmp = NULL;
    while(cnt++ < this->_height){   //找到相应的叶子节点
        tmp = current->searchKeyNode(current, key);
        current = tmp->child;
    }

    KeyNode *rst = current->insertKeyNode(current, key, LEAF, data, NULL);
    if(key < this->minNode->key)    this->minNode = rst;
    this->solveOverFlow(this, current);
    return  rst;
}
void solveOverFlow(struct BTree *this, BaseNode* target){
    if(target->_size <= this->_m )  return; //没有发生上溢

    Type type;
    if(target->type != ROOT)type = target->type;
    else {
        if(this->_height == 1)  type = LEAF;
        else    type = INDEX;
    }
    int pos = target->_size / 2;    //向下取整
    BaseNode *lNode = newBaseNode(this->_m, type, target->parent, NULL);    //左孩子
    BaseNode *rNode = newBaseNode(this->_m, type, target->parent, NULL);    //右孩子
    if(type == LEAF){
        lNode->brother = rNode;
        rNode->brother = target->brother;
    }

    //从第一个节点开始依次附加到左孩子或右孩子中
    KeyNode *tmp = target->header->succ;
    KeyNode *tmpNext = NULL;
    KeyNode *tmpIndex = NULL;   //中间节点的索引
    for(int i = 0; i <pos; i++){
        tmpNext = tmp->succ;
        lNode->append(lNode, tmp);
        tmp = tmpNext;
    }
    //左孩子附加完毕，tmp现在是应该附加到右孩子的第一个节点，需要在父节点建立其前一个孩子的索引
    if(target->type == ROOT){   //如果是根节点发生上溢，则新建一个根节点
        BaseNode *newRoot = newBaseNode(this->_m, ROOT, NULL, NULL);
        tmpIndex = newRoot->insertKeyNode(newRoot, tmp->pred->key, INDEX, -1, lNode);
        this->root = newRoot;
        lNode->parent = newRoot;
        this->_height++;
    } else  tmpIndex = target->parent->insertKeyNode(target->parent, tmp->pred->key, INDEX, -1, lNode);

    //开始附加到右孩子
    for(int i = pos; i < target->_size; i++){
        tmpNext = tmp->succ;
        rNode->append(rNode, tmp);
        tmp = tmpNext;
    }
    //判断是否需要更新右孩子的索引节点的值
    if(target->type == ROOT){   //根节点需要插入新的索引节点
        this->root->insertKeyNode(this->root, tmp->pred->key, INDEX, -1, rNode);
        rNode->parent = this->root;
    }
    if(tmpIndex->succ->key != rNode->trailer->pred->key){   //更新key值
        tmpIndex->succ->key = rNode->trailer->pred->key;
    }
    tmpIndex->succ->child = rNode;
    free(target);
    target = NULL;

    //左右孩子节点不为叶子节点，则需要更新其子节点的父母
    if(type == INDEX){
        for(KeyNode *current = lNode->header->succ;current->type != NILL; current = current->succ)
            current->child->parent = lNode;
        for(KeyNode *current = rNode->header->succ;current->type != NILL; current = current->succ)
            current->child->parent = rNode;
    }
    this->solveOverFlow(this, lNode->parent);
}
int searchNode(struct BTree *this, int key){
    //从根节点开始遍历
    int cnt = 1;
    BaseNode *current = this->root;
    KeyNode *tmp = NULL;
    while(cnt++ < this->_height){
        tmp = current->searchKeyNode(current, key);
        current = tmp->child;
    }
    tmp = current->searchKeyNode(current, key);
    if(tmp->key == key) return tmp->data;
    return -1;
}
void removeNode(struct BTree *this, int key){
    if(this->root->_size == 0) return;

    //从根节点到叶子节点
    int cnt = 1;
    BaseNode *current = this->root;
    KeyNode *tmp = NULL;
    while(cnt++ < this->_height){
        tmp = current->searchKeyNode(current, key);
        current = tmp->child;
    }

    int keyMax = current->trailer->pred->key;   //当前叶节点的关键码最大值
    current->removeKeyNode(current, key);
    if(current->type == ROOT) return;
    if(keyMax != key){
        this->solveUnderFlow(this, current);
        return;
    }
    this->replace(this, current->parent, keyMax, current->trailer->pred->key);
    this->solveUnderFlow(this, current);
}

void replace(struct BTree *this, struct BaseNode *target, int keyOld, int keyNew){
    KeyNode *targetKeyNode = target->searchKeyNode(target, keyOld);
    if(targetKeyNode->key != keyOld) return;    //递归基
    targetKeyNode->key = keyNew;
    if(target->type == ROOT)    return;
    replace(this, target->parent, keyOld, keyNew);
}
void solveUnderFlow(struct BTree *this, struct BaseNode *target){
    if(target->_size >= this->_min) return;
    if(target->type == ROOT) return;    //根节点的情况？

    int maxKey = target->trailer->pred->key;
    KeyNode *borrowKeyNode = NULL;
    BaseNode *rNeighbor = target->brother;
    BaseNode *lNeighbor = (target->parent->searchKeyNode(target->parent, maxKey))->pred->child;
    //从左兄弟拿一个过来
    if(lNeighbor != NULL && lNeighbor->_size > this->_min){
        borrowKeyNode = lNeighbor->trailer->pred;   //拿最大的过来
        //把节点从左兄弟中摘除并放到目标节点
        borrowKeyNode->succ->pred = borrowKeyNode->pred;
        borrowKeyNode->pred->succ = borrowKeyNode->succ;
        target->appendAhead(target, borrowKeyNode);
        //替换索引节点的关键码值
        this->replace(this, lNeighbor->parent, borrowKeyNode->key, lNeighbor->trailer->pred->key);
        return;
    }
    //从右兄弟拿一个过来
    if(rNeighbor != NULL && rNeighbor->_size > this->_min){
        borrowKeyNode = rNeighbor->header->succ;    //拿最小的过来
        borrowKeyNode->pred->succ = borrowKeyNode->succ;
        borrowKeyNode->succ->pred = borrowKeyNode->pred;
        target->append(target, borrowKeyNode);
        this->replace(this, target->parent, maxKey, borrowKeyNode->key);
        return;
    }
    //合并
    KeyNode *current = lNeighbor->trailer->pred;
    KeyNode *pred = NULL;
    int keyOld = current->key;
    while(current->type != NILL){
        pred = current->pred;
        target->appendAhead(target, current);
        current = pred;
    }
    free(lNeighbor);
    lNeighbor = NULL;
    target->parent->removeKeyNode(target->parent, keyOld);
    this->solveUnderFlow(this, target->parent);
}