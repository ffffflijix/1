#include "x.c"
#include "x.h"

void main()
{
    Tree *L = malloc(sizeof(Tree));
    L->gen = NULL;
    L->nodes = 0;

    int a;

    //��ӽڵ㣬��0δ��β
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

    //����˳�����
    print(L->gen, 1);
    putchar('\n');
    print(L->gen, 2);
    putchar('\n');
    print(L->gen, 3);
    putchar('\n');

    //����������Ϣ
    printf("�������ĸ߶�Ϊ%d , �ڵ㹲��%d��\n", howgao(L->gen), L->nodes);

    //ɾ��һ���ڵ�
    scanf("%d", &a);
    not_in(L, L->gen, a);

    //����˳�����
    print(L->gen, 1);
    putchar('\n');
    print(L->gen, 2);
    putchar('\n');
    print(L->gen, 3);
    putchar('\n');

    //��������Ϣ
    printf("�������ĸ߶�Ϊ%d , �ڵ㹲��%d��\n", howgao(L->gen), L->nodes);
}