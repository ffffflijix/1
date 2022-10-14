#include <iostream>

using namespace std;

//��Tree����������Node��Ԫ��Ҫ�õ�
class Tree;

/*
    Node���������Ľڵ���
        @m_num���ڵ��д��ֵ
        @m_leftNode:���ӽڵ�
        @m_rightNode:���ӽڵ�
*/
class Node
{
    friend class Tree;

    private:
        int m_num;
        Node *m_leftNode;
        Node *m_rightNode;

    public:
        Node(int num) : m_num{num}
        {
            m_leftNode = nullptr;
            m_rightNode = nullptr;
            cout << "һ���µĽڵ��Ѿ�����,��ֵ���ڣ�" << num << endl;
        }

        ~Node()
        {
            m_leftNode = nullptr;
            m_rightNode = nullptr;
            cout << "��ɾ��ֵΪ" << m_num << "�Ľڵ�" << endl;
        }
};

/*
    Tree���������ĸ��ڵ�
        @m_Nodesnum:�������Ľڵ���
        @m_Nodesgen:�������ĸ��ڵ�
*/
class Tree
{
    private:
        int m_Nodesnum;
        Node *m_Nodesgen;

    public:
        Tree() : m_Nodesnum{0}
        {
            m_Nodesgen = nullptr;
            cout << "�������Ѵ���" << endl;
        }

        ~Tree()
        {
            Del_Tree();
            cout << "��������ɾ��" << endl;
        }

        /*
            delTree:�ݹ���������ڵ㣬������������
        */
        void Del_Tree()
        {
            //�����޲ε���
            Del_Tree(m_Nodesgen);
        }

        /*
            delTree:�ݹ���������ڵ㣬������������
                @p�������Ľڵ�
        */
        void Del_Tree(Node *p)
        {
            if (p == nullptr)
            {
                return;
            }
            Del_Tree(p->m_leftNode);
            Del_Tree(p->m_rightNode);
            delete p;
        }

        /*
            Get_Nodesnum:��ȡ�ڵ���
        */
        unsigned int Get_Nodesnum( )
        {
            return m_Nodesnum;
        }


        /*
            add_Node:���һ���ڵ㣬��ڵ�ֵΪnum
                @num����Ҫ��ӵĽڵ��ֵ
        */
        void Add_Node(int num)
        {
            //��һ���ڵ������ڵ�
            if (m_Nodesgen == nullptr)
            {
                m_Nodesgen = new Node(num);
                m_Nodesnum ++;
            }
            else
            {
                Node *p = m_Nodesgen;

                //Ѱ�Ҳ���λ��
                while (1)
                {
                    //һ��ֵֻ����һ���ڵ�
                    if( num == p->m_num )
                    {
                        cout << "�˽ڵ��Ѵ���, ��ֵΪ" << num << endl;
                            return;
                    }
                    else if (num < p->m_num)
                    {
                        if (p->m_leftNode == nullptr)
                        {
                            break;
                        }
                        p = p->m_leftNode;
                    }
                    else
                    {
                        if (p->m_rightNode == nullptr)
                        {
                            break;
                        }
                        p = p->m_rightNode;
                    }
                }

                //��ӽڵ�
                if (num < p->m_num)
                {
                    p->m_leftNode = new Node(num);
                    m_Nodesnum++;
                }
                else if (num > p->m_num)
                {
                    p->m_rightNode = new Node(num);
                    m_Nodesnum++;
                }

                //��������ƽ��
                Balanced( );
            }
        }

        /*
            delNode:ɾ��һ��ֵΪnum�Ľڵ�
                @num����Ҫɾ���Ľڵ��ֵ
        */
        void Del_Node(int num)
        {
            //ɾ�����Ǹ��ڵ�
            if( m_Nodesgen->m_num == num )
            {
                Node *temp = m_Nodesgen;

                if( temp->m_leftNode != nullptr && temp->m_leftNode->m_rightNode != nullptr )
                {
                    //���ڵ����ӽڵ������ӽڵ�
                    Node *p = m_Nodesgen->m_leftNode;
                    Node *pre = p;

                    //Ѱ��������ڵ��ֵ
                    while( p->m_rightNode != nullptr )
                    {
                        pre = p;
                        p = p->m_rightNode;
                    }

                    //�滻���ڵ��ֵ��������
                    temp->m_num = p->m_num;
                    pre->m_rightNode = nullptr;
                    delete p;
                }
                else if( temp->m_leftNode != nullptr && temp->m_leftNode->m_rightNode == nullptr )
                {
                    //���ӽڵ�û�����ӽڵ�
                    Node *p = temp->m_leftNode;
                    
                    //�����ӽڵ��ֵ������ڵ�
                    //ֱ�ӽ����ӽڵ���ӽڵ���Ϊ���ӽڵ�
                    temp->m_num = p->m_num;
                    temp->m_leftNode = temp->m_leftNode->m_leftNode;
                    delete p;
                }
                else if( temp->m_leftNode == nullptr )
                {
                    //���ڵ�û�����ӽڵ�
                    //���ӽڵ��ɸ��ڵ㣬����ԭ���ڵ�
                    m_Nodesgen = temp->m_rightNode;
                    delete temp;
                }
            }
            else if (m_Nodesgen == nullptr)
            {
                //��������
                cout << "������Ϊ��" << endl;
            }
            else
            {
                Node *pre = m_Nodesgen;
                Node *p = m_Nodesgen;

                //Ѱ��ɾ���ڵ�
                //��p->m_num != num��p != nullptr֮ǰ�ж�ʱ����pΪnullptrʱ����Ϊ������nullptr��m_num��δ���
                while (p != nullptr && p->m_num != num)
                {
                    if (num < p->m_num)
                    {
                        pre = p;
                        p = p->m_leftNode;
                    }
                    else
                    {
                        pre = p;
                        p = p->m_rightNode;
                    }
                }

                //��δѰ�ҵ�����ڵ�ʱ
                if (p == nullptr)
                {
                    cout << "���޴�ֵ��" << num << endl;
                    return;
                }
                //��ɾ���ڵ�����ӽڵ㲻Ϊ��
                else if (p->m_leftNode != nullptr)
                {
                    Node *temp = p->m_leftNode;
                    Node *temppre = temp;

                    while (temp->m_rightNode != nullptr)
                    {
                        temppre = temp;
                        temp = temp->m_rightNode;
                    }

                    p->m_num = temp->m_num;

                    //���ӽڵ������ӽڵ�
                    if (temp == p->m_leftNode)
                    {
                        p->m_leftNode = p->m_leftNode->m_leftNode;
                        delete temp;
                    }
                    //���ӽڵ������ӽڵ�
                    else
                    {
                        temppre->m_rightNode = nullptr;
                        delete temp;
                    }
                }
                //��ɾ���ڵ�����ӽڵ�Ϊ��
                else
                {
                    //��ɾ���ڵ�����ӽڵ�Ϊ��
                    if (p->m_rightNode == nullptr)
                    {
                        //ֱ��ɾ��
                        if (p->m_num < pre->m_num)
                        {
                            pre->m_leftNode = nullptr;
                        }
                        else
                        {
                            pre->m_rightNode = nullptr;
                        }
                    }
                    //��ɾ���ڵ�����ӽڵ㲻Ϊ��
                    else
                    {
                        //�����ӽڵ�������
                        if (p->m_num < pre->m_num)
                        {
                            pre->m_leftNode = p->m_rightNode;
                        }
                        else
                        {
                            pre->m_rightNode = p->m_rightNode;
                        }
                    }

                    delete p;

                    m_Nodesnum--;
                }
            }

            //��������ƽ��
            Balanced( );
        }

        /*
            Data_print:����������
                @n:ѡ������ķ�ʽ
                    n==1->ǰ��
                    n==2->����
                    n==3->����
        */
        void Data_print(int n)
        {
            //�޲������ýڵ�ı�������Ϊ�������'\n'
            Data_print(m_Nodesgen, n);
            cout << endl;
        }

        /*
            Data_print:�������������ݹ����
                @p����Ҫ����Ľڵ�
                @n:ѡ������ķ�ʽ
                    n==1->ǰ��
                    n==2->����
                    n==3->����
        */
        void Data_print(Node *p, int n)
        {
            if (p == nullptr)
                return;

            int a = 0;
            
            switch (n)
            {
            case 1:
                a = 321;    //����
                break;
            case 2:
                a = 312;    //����
                break;
            case 3:
                a = 132;    //����
                break;
            }

            /*
                a��ֵÿ��%10������a/10
                ��a%10 = 1 ʱ������Լ���ֵ
                ��a%10 = 2 ʱ���ݹ����������ӽڵ�
                ��a%10 = 3 ʱ���ݹ����������ӽڵ�
            */
            for (int i = 0; i < 3; i++)
            {
                switch (a % 10)
                {
                case 1:
                    cout << p->m_num << "  ";
                    break;
                case 2:
                    Data_print(p->m_leftNode, n);
                    break;
                case 3:
                    Data_print(p->m_rightNode, n);
                    break;
                }
                a = a / 10;
            }
        }


        /*
            Get_height:��ȡ�����������
        */
        int Get_height()
        {
            return Get_height(m_Nodesgen);
        }

        /*
            Get_height:��ȡ�ڵ�p�����
                @p����Ҫ��ȡ��ȵĽڵ�
        */
        int Get_height(Node *p)
        {
            if (p == nullptr)
            {
                return 0;
            }
            unsigned int l_height = Get_height(p->m_leftNode);
            unsigned int r_height = Get_height(p->m_rightNode);

            return ( l_height > r_height ? l_height : r_height ) + 1;
        }

        /*
            lest_xuan���Բ�ƽ��Ľڵ��������ƽ�����
                @p:���ֲ��ҽ��в����Ľڵ�
                @pre:p��ǰһ���ڵ㣬��pΪ���ڵ�ʱ��preҲ�Ǹ��ڵ�
        */
        void Left_xuan(Node *p, Node *pre)
        {
            if (p == pre)
            {
                m_Nodesgen = p->m_rightNode;
            }
            else
            {
                if (p->m_num > pre->m_num)
                {
                    pre->m_rightNode = p->m_rightNode;
                }
                else
                {
                    pre->m_leftNode = p->m_rightNode;
                }
            }
            //����Ҫ���в����Ľڵ�����ӽڵ�����������������ֵ���ӽڵ�
            Node *R_lNode = p->m_rightNode->m_leftNode;

            p->m_rightNode->m_leftNode = p;
            p->m_rightNode = R_lNode;
        }

        /*
            right_xuan���Բ�ƽ��Ľڵ��������ƽ�����
                @p:���ֲ��ҽ��в����Ľڵ�
                @pre:p��ǰһ���ڵ㣬��pΪ���ڵ�ʱ��preҲ�Ǹ��ڵ�
        */
        void Right_xuan(Node *p, Node *pre)
        {
            if (p == pre)
            {
                m_Nodesgen = p->m_leftNode;
            }
            else
            {
                if (p->m_num > pre->m_num)
                {
                    pre->m_rightNode = p->m_leftNode;
                }
                else
                {
                    pre->m_leftNode = p->m_leftNode;
                }
            }
            //����Ҫ���в����Ľڵ�����ӽڵ�����������������ֵ���ӽڵ�
            Node *L_rNode = p->m_leftNode->m_rightNode;

            p->m_leftNode->m_rightNode = p;
            p->m_leftNode = L_rNode;
        }


        /*
            Balanced:��������ƽ�ⷽ��ѡ�񣬵��в�ƽ��Ľڵ�ʱ������ѡ��ƽ��ķ�ʽ
                �޲Σ�Ϊ�˷���ƽ��������޲ε���
        */
        void Balanced()
        {
            Balanced(m_Nodesgen, m_Nodesgen);
        }

        /*
            Balanced:��������ƽ�ⷽ��ѡ�񣬵��в�ƽ��Ľڵ�ʱ������ѡ��ƽ��ķ�ʽ
                @p:��Ҫ�ж��Ƿ��ǲ�ƽ��ڵ�Ľڵ�
                @pre��p��ǰһ���ڵ㣬��pΪ���ڵ�ʱ��preҲ�Ǹ��ڵ�
        */
        void Balanced(Node *p, Node *pre)
        {
            if (p == nullptr)
            {
                return;
            }
            //�Ƚ���������ƽ��( �ݹ���� )
            Balanced(p->m_rightNode, p);
            Balanced(p->m_leftNode, p);

            //�жϽڵ��Ƿ�ƽ��
            if (Get_height(p->m_rightNode) - Get_height(p->m_leftNode) > 1 || Get_height(p->m_leftNode) - Get_height(p->m_rightNode) > 1 )
            {
                cout << "�ڵ�" << p->m_num << "��ƽ��" << endl;

                if (Get_height(p->m_rightNode) - Get_height(p->m_leftNode) > 1)
                {
                    if (Get_height(p->m_rightNode->m_leftNode) > Get_height(p->m_rightNode->m_leftNode))
                    {
                        //��������ɲ�ƽ�⣬��������������������ɲ�ƽ��
                        //������������
                        Right_xuan(p->m_rightNode, p);
                        Left_xuan(p, pre);
                    }
                    else
                    {
                        //��������ɲ�ƽ�⣬��������������������ɲ�ƽ��
                        //������
                        Left_xuan(p, pre);
                    }
                }
                else if (Get_height(p->m_leftNode) - Get_height(p->m_rightNode) > 1)
                {
                    if (Get_height(p->m_leftNode->m_leftNode) > Get_height(p->m_leftNode->m_rightNode))
                    {
                        //��������ɲ�ƽ�⣬��������������������ɲ�ƽ��
                        //������
                        Right_xuan(p, pre);
                    }
                    else
                    {
                        //��������ɲ�ƽ�⣬��������������������ɲ�ƽ��
                        //������������
                        Left_xuan(p->m_leftNode, p);
                        Right_xuan(p, pre);
                    }
                }
            }
        }
};


int main()
{
    Tree a;
    int x = 1;

    //ѭ������������ɾ��
    while (1)
    {
        //���ѡ����ӣ���0Ϊ��β
        while (x != 0)
        {
            cin >> x;
            if (x != 0)
                a.Add_Node(x);
        }
        x = 1;

        //����
        cout << "ǰ�������" << endl
             << "-------------------" << endl;
        a.Data_print(1);
        cout << "���������" << endl
             << "-------------------" << endl;
        a.Data_print(2);
        cout << "���������" << endl
             << "-------------------" << endl;
        a.Data_print(3);

        //����������Ϣ
        cout << "�����������Ϊ��" << a.Get_height( ) << " , �ڵ���Ϊ��" << a.Get_Nodesnum( ) << endl; 

        //���ѡ��ɾ������0Ϊ��β
        while (x != 0)
        {
            cin >> x;
            if (x != 0)
                a.Del_Node(x);
        }

        //����
        cout << "ǰ�������" << endl
             << "-------------------" << endl;
        a.Data_print(1);
        cout << "���������" << endl
             << "-------------------" << endl;
        a.Data_print(2);
        cout << "���������" << endl
             << "-------------------" << endl;
        a.Data_print(3);

        //����������Ϣ
        cout << "�����������Ϊ��" << a.Get_height( ) << " , �ڵ���Ϊ��" << a.Get_Nodesnum( ) << endl; 

        //����0������������������
        cout << "����Ҫ�˳��� yes -> 0 ) ?" << endl;
        cin >> x;
        if (x == 0)
        {
            break;
        }
    }
}