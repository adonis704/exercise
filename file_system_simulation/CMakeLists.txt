cmake_minimum_required(VERSION 3.15)
project(BTree C)

set(CMAKE_C_STANDARD 99)

add_executable(BTree main.c BTreeNode.c BTreeNode.h BTree.c BTree.h)