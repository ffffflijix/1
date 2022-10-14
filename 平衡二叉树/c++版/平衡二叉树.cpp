#include <iostream>

using namespace std;

//类Tree的声明，类Node友元需要用到
class Tree;

/*
    Node：二叉树的节点类
        @m_num：节点中存的值
        @m_leftNode:左子节点
        @m_rightNode:右子节点
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
            cout << "一个新的节点已经创建,其值等于：" << num << endl;
        }

        ~Node()
        {
            m_leftNode = nullptr;
            m_rightNode = nullptr;
            cout << "已删除值为" << m_num << "的节点" << endl;
        }
};

/*
    Tree：二叉树的根节点
        @m_Nodesnum:二叉树的节点数
        @m_Nodesgen:二叉树的根节点
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
            cout << "二叉树已创建" << endl;
        }

        ~Tree()
        {
            Del_Tree();
            cout << "二叉树已删除" << endl;
        }

        /*
            delTree:递归调用析构节点，用于析构函数
        */
        void Del_Tree()
        {
            //用于无参调用
            Del_Tree(m_Nodesgen);
        }

        /*
            delTree:递归调用析构节点，用于析构函数
                @p：析构的节点
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
            Get_Nodesnum:获取节点数
        */
        unsigned int Get_Nodesnum( )
        {
            return m_Nodesnum;
        }


        /*
            add_Node:添加一个节点，其节点值为num
                @num：需要添加的节点的值
        */
        void Add_Node(int num)
        {
            //第一个节点作根节点
            if (m_Nodesgen == nullptr)
            {
                m_Nodesgen = new Node(num);
                m_Nodesnum ++;
            }
            else
            {
                Node *p = m_Nodesgen;

                //寻找插入位置
                while (1)
                {
                    //一个值只存在一个节点
                    if( num == p->m_num )
                    {
                        cout << "此节点已存在, 其值为" << num << endl;
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

                //添加节点
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

                //将二叉树平衡
                Balanced( );
            }
        }

        /*
            delNode:删除一个值为num的节点
                @num：需要删除的节点的值
        */
        void Del_Node(int num)
        {
            //删除的是根节点
            if( m_Nodesgen->m_num == num )
            {
                Node *temp = m_Nodesgen;

                if( temp->m_leftNode != nullptr && temp->m_leftNode->m_rightNode != nullptr )
                {
                    //根节点左子节点有右子节点
                    Node *p = m_Nodesgen->m_leftNode;
                    Node *pre = p;

                    //寻找替代根节点的值
                    while( p->m_rightNode != nullptr )
                    {
                        pre = p;
                        p = p->m_rightNode;
                    }

                    //替换根节点的值后将其析构
                    temp->m_num = p->m_num;
                    pre->m_rightNode = nullptr;
                    delete p;
                }
                else if( temp->m_leftNode != nullptr && temp->m_leftNode->m_rightNode == nullptr )
                {
                    //左子节点没有右子节点
                    Node *p = temp->m_leftNode;
                    
                    //用左子节点的值代替根节点
                    //直接将左子节点的子节点提为左子节点
                    temp->m_num = p->m_num;
                    temp->m_leftNode = temp->m_leftNode->m_leftNode;
                    delete p;
                }
                else if( temp->m_leftNode == nullptr )
                {
                    //根节点没有左子节点
                    //右子节点变成根节点，析构原根节点
                    m_Nodesgen = temp->m_rightNode;
                    delete temp;
                }
            }
            else if (m_Nodesgen == nullptr)
            {
                //二叉树空
                cout << "二叉树为空" << endl;
            }
            else
            {
                Node *pre = m_Nodesgen;
                Node *p = m_Nodesgen;

                //寻找删除节点
                //当p->m_num != num在p != nullptr之前判断时，当p为nullptr时，因为访问了nullptr的m_num会段错误
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

                //当未寻找到所需节点时
                if (p == nullptr)
                {
                    cout << "查无此值：" << num << endl;
                    return;
                }
                //需删除节点的左子节点不为空
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

                    //左子节点无右子节点
                    if (temp == p->m_leftNode)
                    {
                        p->m_leftNode = p->m_leftNode->m_leftNode;
                        delete temp;
                    }
                    //左子节点有右子节点
                    else
                    {
                        temppre->m_rightNode = nullptr;
                        delete temp;
                    }
                }
                //需删除节点的左子节点为空
                else
                {
                    //需删除节点的右子节点为空
                    if (p->m_rightNode == nullptr)
                    {
                        //直接删除
                        if (p->m_num < pre->m_num)
                        {
                            pre->m_leftNode = nullptr;
                        }
                        else
                        {
                            pre->m_rightNode = nullptr;
                        }
                    }
                    //需删除节点的右子节点不为空
                    else
                    {
                        //将右子节点提上来
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

            //将二叉树平衡
            Balanced( );
        }

        /*
            Data_print:二叉树遍历
                @n:选择遍历的方式
                    n==1->前序
                    n==2->中序
                    n==3->后序
        */
        void Data_print(int n)
        {
            //无参数调用节点的遍历，并为遍历输出'\n'
            Data_print(m_Nodesgen, n);
            cout << endl;
        }

        /*
            Data_print:二叉树遍历，递归调用
                @p：需要输出的节点
                @n:选择遍历的方式
                    n==1->前序
                    n==2->中序
                    n==3->后序
        */
        void Data_print(Node *p, int n)
        {
            if (p == nullptr)
                return;

            int a = 0;
            
            switch (n)
            {
            case 1:
                a = 321;    //先序
                break;
            case 2:
                a = 312;    //中序
                break;
            case 3:
                a = 132;    //后序
                break;
            }

            /*
                a的值每次%10，并将a/10
                当a%10 = 1 时，输出自己的值
                当a%10 = 2 时，递归调用输出左子节点
                当a%10 = 3 时，递归调用输出右子节点
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
            Get_height:获取二叉树的深度
        */
        int Get_height()
        {
            return Get_height(m_Nodesgen);
        }

        /*
            Get_height:获取节点p的深度
                @p：需要获取深度的节点
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
            lest_xuan：对不平衡的节点进行左旋平衡操作
                @p:本轮查找进行操作的节点
                @pre:p的前一个节点，当p为根节点时，pre也是根节点
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
            //将需要进行操作的节点的右子节点与自身交换，包括了值和子节点
            Node *R_lNode = p->m_rightNode->m_leftNode;

            p->m_rightNode->m_leftNode = p;
            p->m_rightNode = R_lNode;
        }

        /*
            right_xuan：对不平衡的节点进行右旋平衡操作
                @p:本轮查找进行操作的节点
                @pre:p的前一个节点，当p为根节点时，pre也是根节点
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
            //将需要进行操作的节点的左子节点与自身交换，包括了值和子节点
            Node *L_rNode = p->m_leftNode->m_rightNode;

            p->m_leftNode->m_rightNode = p;
            p->m_leftNode = L_rNode;
        }


        /*
            Balanced:二叉树的平衡方法选择，当有不平衡的节点时，分类选择平衡的方式
                无参：为了方便平衡操作的无参调用
        */
        void Balanced()
        {
            Balanced(m_Nodesgen, m_Nodesgen);
        }

        /*
            Balanced:二叉树的平衡方法选择，当有不平衡的节点时，分类选择平衡的方式
                @p:需要判断是否是不平衡节点的节点
                @pre：p的前一个节点，当p为根节点时，pre也是根节点
        */
        void Balanced(Node *p, Node *pre)
        {
            if (p == nullptr)
            {
                return;
            }
            //先进行子树的平衡( 递归调用 )
            Balanced(p->m_rightNode, p);
            Balanced(p->m_leftNode, p);

            //判断节点是否不平衡
            if (Get_height(p->m_rightNode) - Get_height(p->m_leftNode) > 1 || Get_height(p->m_leftNode) - Get_height(p->m_rightNode) > 1 )
            {
                cout << "节点" << p->m_num << "不平衡" << endl;

                if (Get_height(p->m_rightNode) - Get_height(p->m_leftNode) > 1)
                {
                    if (Get_height(p->m_rightNode->m_leftNode) > Get_height(p->m_rightNode->m_leftNode))
                    {
                        //右子树造成不平衡，且是右子树的左子树造成不平衡
                        //先右旋后左旋
                        Right_xuan(p->m_rightNode, p);
                        Left_xuan(p, pre);
                    }
                    else
                    {
                        //右子树造成不平衡，且是右子树的右子树造成不平衡
                        //简单左旋
                        Left_xuan(p, pre);
                    }
                }
                else if (Get_height(p->m_leftNode) - Get_height(p->m_rightNode) > 1)
                {
                    if (Get_height(p->m_leftNode->m_leftNode) > Get_height(p->m_leftNode->m_rightNode))
                    {
                        //左子树造成不平衡，且是左子树的左子树造成不平衡
                        //简单右旋
                        Right_xuan(p, pre);
                    }
                    else
                    {
                        //左子树造成不平衡，且是左子树的右子树造成不平衡
                        //先左旋再右旋
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

    //循环进行输入与删除
    while (1)
    {
        //多次选择添加，以0为结尾
        while (x != 0)
        {
            cin >> x;
            if (x != 0)
                a.Add_Node(x);
        }
        x = 1;

        //遍历
        cout << "前序遍历：" << endl
             << "-------------------" << endl;
        a.Data_print(1);
        cout << "中序遍历：" << endl
             << "-------------------" << endl;
        a.Data_print(2);
        cout << "后序遍历：" << endl
             << "-------------------" << endl;
        a.Data_print(3);

        //二叉树的信息
        cout << "二叉树的深度为：" << a.Get_height( ) << " , 节点数为：" << a.Get_Nodesnum( ) << endl; 

        //多次选择删除，以0为结尾
        while (x != 0)
        {
            cin >> x;
            if (x != 0)
                a.Del_Node(x);
        }

        //遍历
        cout << "前序遍历：" << endl
             << "-------------------" << endl;
        a.Data_print(1);
        cout << "中序遍历：" << endl
             << "-------------------" << endl;
        a.Data_print(2);
        cout << "后序遍历：" << endl
             << "-------------------" << endl;
        a.Data_print(3);

        //二叉树的信息
        cout << "二叉树的深度为：" << a.Get_height( ) << " , 节点数为：" << a.Get_Nodesnum( ) << endl; 

        //输入0结束，输入其他继续
        cout << "您想要退出吗（ yes -> 0 ) ?" << endl;
        cin >> x;
        if (x == 0)
        {
            break;
        }
    }
}