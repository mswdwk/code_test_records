#include <vector>
#include <iostream>
#include <stack>
#include <queue>
using namespace std;

struct BitNode
{
    int data;
    char ch;
    BitNode *left, *right;
    BitNode(int x) :data(x), left(0), right(0){}
    BitNode(char x) :ch(x), left(0), right(0){}
};
#if 0
void Create(BitNode *&root)
{
    int key;
    cin >> key;
    if (key == -1)
        root = NULL;
    else
    {
        root = new BitNode(key);
        Create(root->left);
        Create(root->right);
    }
}
#else
/*
 	   a
	 /  \
	b    c
      /	 \    \
    d	  e    f

*/

void Create(BitNode*&root)
{
	root = new BitNode('a');
	BitNode*b = new BitNode ('b');
	BitNode*c = new BitNode ('c');
	BitNode*d = new BitNode ('d');
	BitNode*e = new BitNode ('e');
	BitNode*f = new BitNode ('f');
	
	root->left = b;
	root->right = c;
	
	b->left = d;
	b->right = e;
	
	//c.left = 0;
	c->right = f;
	
}
#endif

void PreOrderTraversal(BitNode *root)
{
    if (root)
    {
        //cout << root->data << " ";
        cout << root->ch << " ";
        PreOrderTraversal(root->left);
        PreOrderTraversal(root->right);
    }
}

void destory_tree(BitNode *root)
{
    stack<BitNode*> nodeStack;
    nodeStack.push(root);
    while (!nodeStack.empty())
    {
        BitNode *node = nodeStack.top();
        cout << node->ch << ' ';
        nodeStack.pop();
        if (node->right)
        {
            nodeStack.push(node->right);
        }
        if (node->left)
        {
            nodeStack.push(node->left);
        }
        delete node;
    }
    cout<<endl;
}
//深度优先搜索
//利用栈，现将右子树压栈再将左子树压栈
void DepthFirstSearch(BitNode *root)
{
    stack<BitNode*> nodeStack;
    nodeStack.push(root);
    while (!nodeStack.empty())
    {
        BitNode *node = nodeStack.top();
        cout << node->ch << ' ';
        nodeStack.pop();
        if (node->right)
        {
            nodeStack.push(node->right);
        }
        if (node->left)
        {
            nodeStack.push(node->left);
        }
    }
    cout<<endl;
}

//广度优先搜索
void BreadthFirstSearch(BitNode *root)
{
    queue<BitNode*> nodeQueue;
    nodeQueue.push(root);
    while (!nodeQueue.empty())
    {
        BitNode *node = nodeQueue.front();
        //cout << node->data << ' ';
        cout << node->ch << ' ';
        nodeQueue.pop();
        if (node->left)
        {
            nodeQueue.push(node->left);
        }
        if (node->right)
        {
            nodeQueue.push(node->right);
        }
    }
    cout<<endl;
}

int  main()
{
    BitNode *root = NULL;
    Create(root);
    //前序遍历
    cout<<"front treverse"<<endl;
    PreOrderTraversal(root);
    //深度优先遍历
    cout << endl << "dfs" << endl;
    DepthFirstSearch(root);
    //广度优先搜索
    cout << endl << "bfs" << endl;
    BreadthFirstSearch(root);
    cout << endl << "destroy tree by depth" << endl;
    destory_tree(root);
    return 0;
}
