#include "x.h"

/*
    add_in:循环查找添加的位置，并在找到位置后添加该节点，添加后进行平衡操作
        @L：进行操作的树
        @p:本轮查找进行操作的节点
        @x：需要添加的节点
*/
void add_in(Tree *L, Shu *p, Shu *x)
{
    if (L->gen == NULL) //如果根节点为空，则添加节点为根节点
    {
        L->gen = x;
        L->nodes++;
        return;
    }

    //1.如果本轮递归查询的节点的值小于x，并且右子节点不为空
    if (p->Data < x->Data && p->rnext != NULL)
    {
        add_in(L, p->rnext, x);
    }
    //2.如果本轮递归查询的节点的值大于x，并且左子节点不为空
    else if (p->Data > x->Data && p->lnext != NULL)
    {
        add_in(L, p->lnext, x);
    }
    //3.如果本轮递归查询的节点的值小于x，并且右子节点为空
    else if (p->Data < x->Data && p->rnext == NULL)
    {
        p->rnext = x;
        L->nodes++;
    }
    //4.如果本轮递归查询的节点的值大于x，并且右子节点为空
    else if(p->Data > x->Data && p->lnext == NULL)
    {
        p->lnext = x;
        L->nodes++;
    }
    //5.如果本轮递归查询的节点的值等于x
    else if (p->Data == x->Data)
    {
        printf("已有此节点！其值为%d\n", p->Data);
        return;
    }
    //平衡操作
    find_gao(L, L->gen, L->gen);
    return;
}

/*
    not_in:循环查找删除的位置，并在找到位置后删除该节点，删除后进行平衡操作
        @L：进行操作的树
        @p:本轮查找进行操作的节点
        @x:需要删除的节点的值
*/
void not_in(Tree *L, Shu *p, int x)
{
    //未查询到此节点
    if (p->Data < x && p->rnext == NULL)
    {
        printf("不存在此节点\n");
        return;
    }
    //未查询到此节点
    else if (p->Data > x && p->lnext == NULL)
    {
        printf("不存在此节点\n");
        return;
    }
    //1.如果本轮递归查询的节点的值小于x，并且右子节点的值不是x
    else if (p->Data < x && p->rnext->Data != x)
    {
        not_in(L, p->rnext, x);
    }
    //2.如果本轮递归查询的节点的值大于x，并且左子节点的值不是x
    else if (p->Data > x && p->lnext->Data != x)
    {
        not_in(L, p->lnext, x);
    }
    //3.如果本轮递归查询的节点的值小于x，并且右子节点的值是x
    else if (p->Data < x && p->rnext->Data == x)
    {
        L->nodes--;
        Shu *z = p->rnext;

        //对节点左右子节点情况分类进行删除操作
        if (z->lnext == NULL && z->rnext == NULL)
        {
            p->rnext = NULL;
            free(z);
        }
        else if (z->lnext == NULL && z->rnext != NULL)
        {
            p->rnext = z->rnext;
            free(z);
        }
        else if (z->lnext != NULL && z->rnext == NULL)
        {
            p->rnext = z->lnext;
            free(z);
        }
        else
        {
            Shu *y = z->lnext;
            Shu *q = y;
            while (y->lnext)
            {
                q = y;
                y = y->rnext;
            }
            //用左子树最大的值来代替待删除节点，并且删除左子树的最大值节点
            z->Data = y->Data;
            if (y == z->lnext)
            {
                z->lnext = z->lnext->lnext;
            }
            else
            {
                q->rnext = NULL;
            }
            free(y);
        }
    }
    //4.如果本轮递归查询的节点的值大于x，并且右子节点的值是x
    else if (p->Data > x && p->lnext->Data == x)
    {
        L->nodes--;
        Shu *z = p->lnext;

        //对节点左右子节点情况分类进行删除操作
        if (z->lnext == NULL && z->rnext == NULL)
        {
            p->lnext = NULL;
            free(z);
        }
        else if (z->lnext == NULL && z->rnext != NULL)
        {
            p->lnext = z->rnext;
            free(z);
        }
        else if (z->lnext != NULL && z->rnext == NULL)
        {
            p->lnext = z->lnext;
            free(z);
        }
        else
        {
            Shu *y = z->lnext;
            Shu *q = z;
            while (y->rnext)
            {
                q = y;
                y = y->rnext;
            }
            //用左子树最大的值来代替待删除节点，并且删除左子树的最大值节点
            z->Data = y->Data;
            if (y == z->lnext)
            {
                z->lnext = z->lnext->lnext;
            }
            else
            {
                q->rnext = NULL;
            }
            free(y);
        }
    }
    find_gao(L, L->gen, L->gen);
}

/*
    print:选择性递归方式输出输出
        @p：本轮输出的节点
        @n:选择输出的方式
            1.先序输出
            2.中序输出
            3.后序输出
*/
void print(Shu *p, int n)
{
    if (p == NULL)
    {
        return;
    }

    //通过n决定a的值，然后a%10的值选择先输出左右节点和本节点，以此达成选择输出顺序的目的
    int a = 0; 
    switch (n)
    {
    case 1:
        a = 321;
        break;
    case 2:
        a = 312;
        break;
    case 3:
        a = 132;
        break;
    }
    for (int i = 0; i < 3; i++)
    {
        switch (a % 10)
        {
        case 1:
            printf("%d ", p->Data);
            break;
        case 2:
            print(p->lnext, n);
            break;
        case 3:
            print(p->rnext, n);
            break;
        }
        a = a / 10;
    }
}

/*
    howgao：递归得到节点p的高度（深度）
        @p:需要得到高度的节点
*/
int howgao(Shu *p)
{
    if (p == NULL)
        return 0;

    int suml = 0;
    int sumr = 0;

    suml = howgao(p->lnext) + 1;
    sumr = howgao(p->rnext) + 1;

    return suml > sumr ? suml : sumr;
}

/*
    find_gao：并对不平衡的节点进行左旋平衡操作
        @L：进行操作的树
        @p:本轮查找进行操作的节点
        @pre:p的前一个节点，当p为根节点时，pre也是根节点
*/
void *find_gao(Tree *L, Shu *p, Shu *pre)
{
    if (p == NULL)
        return NULL;

    find_gao(L, p->lnext, p);
    find_gao(L, p->rnext, p);
    
    if (howgao(p->rnext) - howgao(p->lnext) > 1 || howgao(p->lnext) - howgao(p->rnext) > 1)
    {
        if (howgao(p->rnext) - howgao(p->lnext) > 1)
        {
            if (howgao(p->rnext->lnext) > howgao(p->rnext->rnext))
            {
                //先右后左
                youxuan(p, p->rnext, L);
                zuoxuan(pre, p, L);
            }
            else
            {
                //简单左旋
                zuoxuan(pre, p, L);
            }
        }
        else
        {
            if (howgao(p->lnext->rnext) > howgao(p->lnext->lnext))
            {
                //先左后右
                zuoxuan(p, p->lnext, L);
                youxuan(pre, p, L);
            }
            else
            {
                //简单右旋
                youxuan(pre, p, L);
            }
        }
        printf("节点%d不平衡\n", p->Data);
        return p;
    }

    return NULL;
}

/*
    find_gao：并对不平衡的节点进行左旋平衡操作
        @L：进行操作的树
        @p:本轮查找进行操作的节点
        @pre:p的前一个节点，当p为根节点时，pre也是根节点
*/
void zuoxuan(Shu *pre, Shu *p, Tree *L)
{
    if (p == pre)
    {
        L->gen = p->rnext;
    }
    else
    {
        if (p->Data > pre->Data)
        {
            pre->rnext = p->rnext;
        }
        else
        {
            pre->lnext = p->rnext;
        }
    }

    Shu *R_lnext = p->rnext->lnext;

    p->rnext->lnext = p;
    p->rnext = R_lnext;
}

/*
    find_gao：并对不平衡的节点进行右旋平衡操作
        @L：进行操作的树
        @p:本轮查找进行操作的节点
        @pre:p的前一个节点，当p为根节点时，pre也是根节点
*/
void youxuan(Shu *pre, Shu *p, Tree *L)
{
    if (p == pre)
    {
        L->gen = p->lnext;
    }
    else
    {
        if (p->Data > pre->Data)
        {
            pre->rnext = p->lnext;
        }
        else
        {
            pre->lnext = p->lnext;
        }
    }

    Shu *L_rnext = p->lnext->rnext;
    
    p->lnext->rnext = p;
    p->lnext = L_rnext;
}
