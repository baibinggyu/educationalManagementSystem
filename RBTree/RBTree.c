#include "RBTree.h"
RBTree RBTree_CREATENODE(void* data,enum Color color){
    RBTree node = (RBTree)malloc(sizeof(struct RBTree));
    if(node == NULL){
        perror("内存分配错误");
        return NULL;
    }
    node->data = data;
    node->color = RED;
    node->left = node->right = node->parent = NULL;
    return node;
}

static bool RBTree_LEFT_ROTATE(RBTree* root,RBTree x){
    if(!x->right) return false;
    RBTree y = x->right;
    if(y->left)
        RBTree_SETPARENT(y->left,x);
    RBTree_SETRIGHT(x,y->left);
    RBTree_SETLEFT(y,x);
    if(*root == x){
        *root = y;
    }else{
        if(RBTree_GET_PARENT(x)->left == x){
            RBTree_GET_PARENT(x)->left = y;
        }else{
            RBTree_GET_PARENT(x)->right = y;
        }
    }
    RBTree_SETPARENT(y,RBTree_GET_PARENT(x));
    RBTree_SETPARENT(x,y);
    return true;
}
static bool RBTree_RIGHT_ROTATE(RBTree* root,RBTree y){
    if(!y->left) return false;
    RBTree x = y->left;
    if(x->right)
        RBTree_SETPARENT(x->right,y);
    RBTree_SETLEFT(y,x->right);
    RBTree_SETRIGHT(x,y);
    if(*root == y){
        *root = x;
    }else{
        if(RBTree_GET_PARENT(y)->left == y){
            RBTree_GET_PARENT(y)->left = x;
        }else{
            RBTree_GET_PARENT(y)->right = x;
        }
    }
    RBTree_SETPARENT(x,RBTree_GET_PARENT(y));
    RBTree_SETPARENT(y,x);

    return true;
}
bool RBTree_PREODER(RBTree root,RBTree_Print Print){
    if(root == NULL) return true;
    Print(root->data);
    RBTree_PREODER(root->left,Print);
    RBTree_PREODER(root->right,Print);
    return true;

}
bool RBTree_INORDER(RBTree root,RBTree_Print Print){
    if(root == NULL) return true;
    RBTree_INORDER(root->left,Print);
    Print(root->data);
    RBTree_INORDER(root->right,Print);
    return true;
}
bool RBTree_POSTORDER(RBTree root,RBTree_Print Print){
    if(root == NULL) return true;
    RBTree_POSTORDER(root->left,Print);
    RBTree_POSTORDER(root->right,Print);
    Print(root->data);
    return true;
}

RBTree RBTree_SEARCH(RBTree* root,void* data,CompareFunction cmp){
    RBTree cur = *root;
    while(cur){
        int flag = cmp(data,cur->data);
        if(flag == 0) return cur;
        else if (flag < 0) cur = cur->left;
        else if(flag > 0) cur = cur->right;
        else return cur;
    }
    return NULL;
}
bool RBTree_UPDATE(RBTree* root,void* data,CompareFunction cmp,UpdateFunction update){
    RBTree cur = RBTree_SEARCH(root,data,cmp);
    if(!cur) return false;
    return update(cur,data);
}
bool RBTree_DESTROY(RBTree root, RBTree_FreeData free_func){
    if(root == NULL) return true;
    RBTree_DESTROY(root->left,free_func);
    RBTree_DESTROY(root->right,free_func);
    if(free_func) if(!free_func(root->data)) return false;
    free(root);
    return true;
}

static bool RBTree_INSERT_FIXUP(RBTree* root,RBTree node){
    RBTree parent = NULL,grandParent = NULL;
    while((parent  = RBTree_GET_PARENT(node))&& RBTree_ISRED(parent)){
        grandParent = RBTree_GET_PARENT(parent);
        if(grandParent->left == parent && RBTree_ISRED(grandParent->right)){
            RBTree_SETRED(grandParent);
            RBTree_SETBLACK(parent);
            RBTree_SETBLACK(grandParent->right);
            node = grandParent;
        }else if(grandParent->right == parent && RBTree_ISRED(grandParent->left)){
            RBTree_SETRED(grandParent);
            RBTree_SETBLACK(parent);
            RBTree_SETBLACK(grandParent->left);
            node = grandParent;
        }
        else {
            // LL
            if(parent->left == node && grandParent->left == parent){
                RBTree_SETBLACK(parent);
                RBTree_SETRED(grandParent);
                RBTree_RIGHT_ROTATE(root,grandParent);
            // LR
            }else if (parent->right == node && grandParent->left == parent){
                RBTree_SETBLACK(node);
                RBTree_SETRED(grandParent);
                RBTree_LEFT_ROTATE(root,parent);
                RBTree_RIGHT_ROTATE(root,grandParent);
                node = parent;

            // RR
            }else if (parent->right == node && grandParent->right == parent){
                RBTree_SETBLACK(parent);
                RBTree_SETRED(grandParent);
                RBTree_LEFT_ROTATE(root,grandParent);
            // RL
            }else if (parent->left == node && grandParent->right == parent){
                RBTree_SETBLACK(node);
                RBTree_SETRED(grandParent);
                RBTree_RIGHT_ROTATE(root,parent);
                RBTree_LEFT_ROTATE(root,grandParent);
                node = parent;
            }
        }

    }
    RBTree_SETBLACK(*root);
    return true;
}

bool RBTree_INSERT(RBTree* root,void* data,CompareFunction cmp){
    RBTree node = RBTree_CREATENODE(data,RED);
    if(node == NULL) return false;
    RBTree preNode = NULL,curNode = * root;
    int flag;
    while(curNode){
        preNode = curNode;
        flag = cmp(data,curNode->data);
        if(flag < 0 ) curNode = curNode->left;
        else  curNode  = curNode->right;
    }
    RBTree_SETPARENT(node,preNode);
    if(preNode == NULL){
        *root = node;
    }else{
        if(flag < 0) preNode->left = node;
        else preNode->right = node;
    }
    return RBTree_INSERT_FIXUP(root,node);
}

static RBTree RBTree_MINIMUM(RBTree node){
    if(!node) return NULL;
    while(node->left) node = node->left;
    return node;
}

// 节点替换--->移植
// 用v替换u 不处理子节点
static bool RBTree_TRANSPLANT(RBTree* root,RBTree u,RBTree v){
    if(!u) return false;
    if(!RBTree_GET_PARENT(u)){
        *root = v;
    }else if(u == RBTree_GET_PARENT(u)->left){
        RBTree_GET_PARENT(u)->left = v;
    }else{
        RBTree_GET_PARENT(u)->right = v;
    }
    if(v){
        RBTree_GET_PARENT(v) = RBTree_GET_PARENT(u);
    }
    return true;
}
static bool RBTree_DELETE_FIXUP(RBTree* root,RBTree node,RBTree parent){
    while (node != *root && RBTree_ISBLACK(node)) {
        if (node == parent->left) {
            RBTree sibling = parent->right;
            // 兄弟是红色 -> 变色 + 左旋
            if (RBTree_ISRED(sibling)) {
                RBTree_SETBLACK(sibling);
                RBTree_SETRED(parent);	
                RBTree_LEFT_ROTATE(root, parent);
                sibling = parent->right; 
            }
            // 兄弟是黑色，两个子节点也都是黑色
            if (RBTree_ISBLACK(sibling->left) && RBTree_ISBLACK(sibling->right)) {
                RBTree_SETRED(sibling); // 把兄弟当作补偿
                node = parent;	          // 向上传递“双重黑”
                parent = RBTree_GET_PARENT(node);
            } else {
                // 兄弟是黑色，左红右黑
                if (RBTree_ISBLACK(sibling->right)) {
                    RBTree_SETBLACK(sibling->left);
                    RBTree_SETRED(sibling);
                    RBTree_RIGHT_ROTATE(root, sibling);
                    sibling = parent->right; // 更新兄弟
                }
                // 兄弟是黑色，右孩子为红
                sibling->color = parent->color;
                RBTree_SETBLACK(parent);
                RBTree_SETBLACK(sibling->right);
                RBTree_LEFT_ROTATE(root, parent);
                node = *root; // 修复结束
                break;
            }
        } else {
            // 对称情况：node 是 parent 的右孩子
            RBTree sibling = parent->left;
            // 兄弟是红色 -> 变色 + 右旋
            if (RBTree_ISRED(sibling)) {
                RBTree_SETBLACK(sibling);
                RBTree_SETRED(parent);
                RBTree_RIGHT_ROTATE(root, parent);
                sibling = parent->left;
            }
            // 兄弟是黑色，两个子节点也都是黑色
            if (RBTree_ISBLACK(sibling->left) && RBTree_ISBLACK(sibling->right)) {
                RBTree_SETRED(sibling);
                node = parent;
                parent = RBTree_GET_PARENT(node);
            } else {
                // 兄弟是黑色，右红左黑
                if (RBTree_ISBLACK(sibling->left)) {
                    RBTree_SETBLACK(sibling->right);
                    RBTree_SETRED(sibling);
                    RBTree_LEFT_ROTATE(root, sibling);
                    sibling = parent->left;
                }

                // 兄弟是黑色，左孩子为红
                sibling->color = parent->color;
                RBTree_SETBLACK(parent);
                RBTree_SETBLACK(sibling->left);
                RBTree_RIGHT_ROTATE(root, parent);
                node = *root;
                break;
            }
        }
    }

    if (node != NULL) {
        RBTree_SETBLACK(node); // 保证最终颜色合法
    }

    return true;
}
bool RBTree_DELETE(RBTree* root,void* data,CompareFunction cmp,RBTree_FreeData free_func){ 
    RBTree node = RBTree_SEARCH(root,data,cmp);
    if(!node) return false;
    enum Color color = RBTree_GET_COLOR(node);
    RBTree child = NULL;
    RBTree parent = NULL;
    // bst删除
    if(node->left == NULL){
        child = node->right;
        parent = RBTree_GET_PARENT(node);
        RBTree_TRANSPLANT(root,node,node->right);
    }else if (node->right == NULL){
        child = node->left;
        parent = RBTree_GET_PARENT(node);
        RBTree_TRANSPLANT(root,node,node->left);
    }else{
        RBTree successor = RBTree_MINIMUM(node->right);
        child = successor->right;
        if(RBTree_GET_PARENT(successor) != node){
            parent = successor->parent;
            RBTree_TRANSPLANT(root,successor,successor->right);
            successor->right = node->right;
            if(successor->right){
                successor->right->parent = successor;
            }
        }else 
            parent = successor;
        RBTree_TRANSPLANT(root,node,successor);
        successor->left = node->left;
        if(successor->left) successor->left->parent = successor;
        successor->color = RBTree_GET_COLOR(node);
    }
    if(free_func) if(node->data) free_func(node->data);
    free(node);
    if(color == BLACK){
        if(child == NULL){
            if(parent != NULL){
                RBTree_DELETE_FIXUP(root,NULL,parent);
            }
        }else {
            RBTree_DELETE_FIXUP(root,child,RBTree_GET_PARENT(child));
        }
    }
    return true;
}
