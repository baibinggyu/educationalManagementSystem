#ifndef _RBTREE_C
#define _RBTREE_C
#include "stdlib.h"
#include "stdbool.h"
#include <stdio.h>

enum Color{
    BLACK = 0,
    RED
};

#define RBTree_GET_PARENT(x) ((x)->parent)
#define RBTree_GET_COLOR(x) ((x)->color)
#define RBTree_ISBLACK(x) ((x) == NULL || (x)->color == BLACK)
#define RBTree_ISRED(x) ((x) && ((x)->color == RED))
#define RBTree_SETPARENT(x,y) (((x)->parent) = y)
#define RBTree_SETLEFT(x,y) (((x)->left) = y)
#define RBTree_SETRIGHT(x,y) (((x)->right) = y)
#define RBTree_SETBLACK(x) (((x)->color) = BLACK)
#define RBTree_SETRED(x) (((x)->color) = RED)

// 小于 1 等于 0 大于 -1
typedef int (*CompareFunction)(const void* node_data, const void* input_data);
typedef bool (*UpdateFunction)(const void* old_data,const void* new_data); 
typedef void (*RBTree_Print)(const void* data);
typedef bool (*RBTree_FreeData)(void* data); 

typedef struct RBTree{
    void* data;
    enum Color color;
    struct RBTree * left,* right,* parent;
}* RBTree;

RBTree RBTree_CREATENODE(void* data,enum Color color);
bool RBTree_INSERT(RBTree* root,void* data,CompareFunction cmp);
bool RBTree_PREODER(RBTree root,RBTree_Print Print);
bool RBTree_INORDER(RBTree root,RBTree_Print Print);
bool RBTree_POSTORDER(RBTree root,RBTree_Print Print);
RBTree RBTree_SEARCH(RBTree* root,void* data,CompareFunction cmp);
bool RBTree_UPDATE(RBTree* root,void* data,CompareFunction cmp,UpdateFunction update);
bool RBTree_DELETE(RBTree* root,void* data,CompareFunction cmp,RBTree_FreeData free_func);
bool RBTree_DESTROY(RBTree root, RBTree_FreeData free_func);


bool RBTree_STATICK_DESTROY(RBTree root);

#endif