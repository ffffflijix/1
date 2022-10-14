#include "x.c"
#include "x.h"

void main()
{
    Tree *L = malloc(sizeof(Tree));
    L->gen = NULL;
    L->nodes = 0;

    int a;

    //添加节点，以0未结尾
    while (1)
    {
        scanf("%d", &a);
        if (a == 0)
        {
            break;
        }
        Shu *x = malloc(sizeof(Shu));
        x->Data = a;

        x->lnext = NULL;
        x->rnext = NULL;
        add_in(L, L->gen, x);
    }

    //三种顺序输出
    print(L->gen, 1);
    putchar('\n');
    print(L->gen, 2);
    putchar('\n');
    print(L->gen, 3);
    putchar('\n');

    //二叉树的信息
    printf("二叉树的高度为%d , 节点共有%d个\n", howgao(L->gen), L->nodes);

    //删除一个节点
    scanf("%d", &a);
    not_in(L, L->gen, a);

    //三种顺序输出
    print(L->gen, 1);
    putchar('\n');
    print(L->gen, 2);
    putchar('\n');
    print(L->gen, 3);
    putchar('\n');

    //二叉树信息
    printf("二叉树的高度为%d , 节点共有%d个\n", howgao(L->gen), L->nodes);
}