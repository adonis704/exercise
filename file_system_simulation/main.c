#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "BTree.h"

int main() {
    BTree *btree = newBTree(100);
    btree->insertNode(btree, 10, 10);

    srand((unsigned)time(NULL));
    int data, choice;
    clock_t start, finish;
    start = clock();

    for(int i = 0; i < 10000000; i++){
        data = rand() % 100000;
        choice = data % 3;
        switch(choice){
            case 0:
                btree->insertNode(btree, data, data);
                break;
            case 1:
                btree->removeNode(btree, data);
                break;
            case 2:
                btree->searchNode(btree, data);
                if(btree->_height > 4)
                    printf("%d\n", btree->_height);
                break;
            default:
                break;
        }
    }
    finish = clock();
    printf("time cost: %d", finish-start);
    return 0;
}
