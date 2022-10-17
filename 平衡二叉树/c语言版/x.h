#ifndef __SHU_H__
#define __SHU_H__

#include <stdio.h>  //printf
#include <stdlib.h> //malloc
#include <string.h> //strlen

typedef int da;

/*
    Shu:二叉树的树枝，作节点
        @Data：节点上存放的数据
        @lnext:左子节点
        @rnext:右左节点
*/
typedef struct shu
{
    da Data;
    struct shu *lnext;
    struct shu *rnext;
} Shu;

/*
    Tree：二叉树的头节点
        @nodes:二叉树的节点的数量
        @gen:二叉树的根节点

*/
typedef struct tree
{
    da nodes;
    Shu *gen;
} Tree;


/*
    Destroy:销毁二叉树
        @node:递归调用删除的节点
*/
void Destroy( Shu *node );


/*
    add_in:循环查找添加的位置，并在找到位置后添加该节点，添加后进行平衡操作
        @L：进行操作的树
        @p:本轮查找进行操作的节点
        @x：需要添加的节点
*/
void add_in(Tree *L, Shu *p, Shu *x);

/*
    not_in:循环查找删除的位置，并在找到位置后删除该节点，删除后进行平衡操作
        @L：进行操作的树
        @p:本轮查找进行操作的节点
        @x:需要删除的节点的值
*/
void not_in(Tree *L, Shu *p, int x);

/*
    print:选择性递归方式输出输出
        @p：本轮输出的节点
        @n:选择输出的方式
            1.先序输出
            2.中序输出
            3.后序输出
*/
void print(Shu *p, int n);


/*
    howgao：递归得到节点p的高度（深度）
        @p:需要得到高度的节点
*/
int howgao(Shu *p);


/*
    find_gao：通过比对节点的左右子节点的高度查看节点是否平衡，并对不平衡的节点进行平衡操作
        @L：进行操作的树
        @p:本轮查找进行操作的节点
        @pre:p的前一个节点，当p为根节点时，pre也是根节点
*/
void *find_gao(Tree *L, Shu *p, Shu *pre);

/*
    find_gao：并对不平衡的节点进行左旋平衡操作
        @L：进行操作的树
        @p:本轮查找进行操作的节点
        @pre:p的前一个节点，当p为根节点时，pre也是根节点
*/
void zuoxuan(Shu *pre, Shu *p, Tree *L);

/*
    find_gao：并对不平衡的节点进行右旋平衡操作
        @L：进行操作的树
        @p:本轮查找进行操作的节点
        @pre:p的前一个节点，当p为根节点时，pre也是根节点
*/
void youxuan(Shu *pre, Shu *p, Tree *L);

#endif
