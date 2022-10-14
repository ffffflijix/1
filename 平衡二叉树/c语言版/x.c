#include "x.h"

/*
    add_in:ѭ��������ӵ�λ�ã������ҵ�λ�ú���Ӹýڵ㣬��Ӻ����ƽ�����
        @L�����в�������
        @p:���ֲ��ҽ��в����Ľڵ�
        @x����Ҫ��ӵĽڵ�
*/
void add_in(Tree *L, Shu *p, Shu *x)
{
    if (L->gen == NULL) //������ڵ�Ϊ�գ�����ӽڵ�Ϊ���ڵ�
    {
        L->gen = x;
        L->nodes++;
        return;
    }

    //1.������ֵݹ��ѯ�Ľڵ��ֵС��x���������ӽڵ㲻Ϊ��
    if (p->Data < x->Data && p->rnext != NULL)
    {
        add_in(L, p->rnext, x);
    }
    //2.������ֵݹ��ѯ�Ľڵ��ֵ����x���������ӽڵ㲻Ϊ��
    else if (p->Data > x->Data && p->lnext != NULL)
    {
        add_in(L, p->lnext, x);
    }
    //3.������ֵݹ��ѯ�Ľڵ��ֵС��x���������ӽڵ�Ϊ��
    else if (p->Data < x->Data && p->rnext == NULL)
    {
        p->rnext = x;
        L->nodes++;
    }
    //4.������ֵݹ��ѯ�Ľڵ��ֵ����x���������ӽڵ�Ϊ��
    else if(p->Data > x->Data && p->lnext == NULL)
    {
        p->lnext = x;
        L->nodes++;
    }
    //5.������ֵݹ��ѯ�Ľڵ��ֵ����x
    else if (p->Data == x->Data)
    {
        printf("���д˽ڵ㣡��ֵΪ%d\n", p->Data);
        return;
    }
    //ƽ�����
    find_gao(L, L->gen, L->gen);
    return;
}

/*
    not_in:ѭ������ɾ����λ�ã������ҵ�λ�ú�ɾ���ýڵ㣬ɾ�������ƽ�����
        @L�����в�������
        @p:���ֲ��ҽ��в����Ľڵ�
        @x:��Ҫɾ���Ľڵ��ֵ
*/
void not_in(Tree *L, Shu *p, int x)
{
    //δ��ѯ���˽ڵ�
    if (p->Data < x && p->rnext == NULL)
    {
        printf("�����ڴ˽ڵ�\n");
        return;
    }
    //δ��ѯ���˽ڵ�
    else if (p->Data > x && p->lnext == NULL)
    {
        printf("�����ڴ˽ڵ�\n");
        return;
    }
    //1.������ֵݹ��ѯ�Ľڵ��ֵС��x���������ӽڵ��ֵ����x
    else if (p->Data < x && p->rnext->Data != x)
    {
        not_in(L, p->rnext, x);
    }
    //2.������ֵݹ��ѯ�Ľڵ��ֵ����x���������ӽڵ��ֵ����x
    else if (p->Data > x && p->lnext->Data != x)
    {
        not_in(L, p->lnext, x);
    }
    //3.������ֵݹ��ѯ�Ľڵ��ֵС��x���������ӽڵ��ֵ��x
    else if (p->Data < x && p->rnext->Data == x)
    {
        L->nodes--;
        Shu *z = p->rnext;

        //�Խڵ������ӽڵ�����������ɾ������
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
            //������������ֵ�������ɾ���ڵ㣬����ɾ�������������ֵ�ڵ�
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
    //4.������ֵݹ��ѯ�Ľڵ��ֵ����x���������ӽڵ��ֵ��x
    else if (p->Data > x && p->lnext->Data == x)
    {
        L->nodes--;
        Shu *z = p->lnext;

        //�Խڵ������ӽڵ�����������ɾ������
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
            //������������ֵ�������ɾ���ڵ㣬����ɾ�������������ֵ�ڵ�
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
    print:ѡ���Եݹ鷽ʽ������
        @p����������Ľڵ�
        @n:ѡ������ķ�ʽ
            1.�������
            2.�������
            3.�������
*/
void print(Shu *p, int n)
{
    if (p == NULL)
    {
        return;
    }

    //ͨ��n����a��ֵ��Ȼ��a%10��ֵѡ����������ҽڵ�ͱ��ڵ㣬�Դ˴��ѡ�����˳���Ŀ��
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
    howgao���ݹ�õ��ڵ�p�ĸ߶ȣ���ȣ�
        @p:��Ҫ�õ��߶ȵĽڵ�
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
    find_gao�����Բ�ƽ��Ľڵ��������ƽ�����
        @L�����в�������
        @p:���ֲ��ҽ��в����Ľڵ�
        @pre:p��ǰһ���ڵ㣬��pΪ���ڵ�ʱ��preҲ�Ǹ��ڵ�
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
                //���Һ���
                youxuan(p, p->rnext, L);
                zuoxuan(pre, p, L);
            }
            else
            {
                //������
                zuoxuan(pre, p, L);
            }
        }
        else
        {
            if (howgao(p->lnext->rnext) > howgao(p->lnext->lnext))
            {
                //�������
                zuoxuan(p, p->lnext, L);
                youxuan(pre, p, L);
            }
            else
            {
                //������
                youxuan(pre, p, L);
            }
        }
        printf("�ڵ�%d��ƽ��\n", p->Data);
        return p;
    }

    return NULL;
}

/*
    find_gao�����Բ�ƽ��Ľڵ��������ƽ�����
        @L�����в�������
        @p:���ֲ��ҽ��в����Ľڵ�
        @pre:p��ǰһ���ڵ㣬��pΪ���ڵ�ʱ��preҲ�Ǹ��ڵ�
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
    find_gao�����Բ�ƽ��Ľڵ��������ƽ�����
        @L�����в�������
        @p:���ֲ��ҽ��в����Ľڵ�
        @pre:p��ǰһ���ڵ㣬��pΪ���ڵ�ʱ��preҲ�Ǹ��ڵ�
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
