#include<iostream>
#include<cmath>
#include<set>
#include<cstring>
#include<vector>
#include"base.h"
using namespace std;
#include<fstream>
#define M 1023
struct Set {
	int x;
	int y;
};
struct CityID
{
	string name;  //城市名字
	Set coordinate;//坐标
};   
class CityBaseTree
{
private:
	struct CNode
	{
		CityID cityid;   //城市信息
		CNode* left;
		CNode* right;
		CNode()
		{
			left = NULL;
			right = NULL;
		}
		CNode(CityID id)
		{
			cityid = id;
			left = NULL;
			right = NULL;
		}
		CNode(CityID id, CNode* P1, CNode* P2) :cityid(id), left(P1), right(P2) {};
		int Compare(const CityID& id) const
		{
			return cityid.name.compare(id.name);
			//cityid<id 返回-1
			//cityid=id 返回0
			//cityid>id 返回1
		}
	};
	typedef CNode* CNodePointer;

public:
	CNodePointer root;
	//根结点指针

	CityBaseTree() :root(NULL) {};
	explicit CityBaseTree(int file)
	{
		root = NULL;
		if (file != 0) Input();
	}
	~CityBaseTree()
	{//析构函数，删除new出来的root
		makeEmpty(root);
	}

	void makeEmpty(CNodePointer t)
	{
		if (t != nullptr)
		{
			makeEmpty(t->left);
			makeEmpty(t->right);
			delete t;
		}
		t = nullptr;
	}
	bool IsEmpty(CNodePointer T)
	{//判断结点T是否为空，若空则返回1，反之则返回0
		if (T == NULL) return 1;
		else return 0;
	}
	CNodePointer Root()
	{
		return root;
	}
	void Insert(const CityID& CID, CNodePointer& node)
	{
		if (node == NULL)
		{
			node = new CNode;  //录入城市信息
			node->cityid = CID;
		}
		else if (node->Compare(CID) == -1) Insert(CID, node->left);
		else if (node->Compare(CID) == 1) Insert(CID, node->right);
		else ;

	}
	CNodePointer FindMax(const CNodePointer node) const
	{
		if (node != NULL)
		{
			if (node->right == NULL) return node;
			else return FindMax(node->right);
		}
		else
		{
			return NULL;
		}
	}
	CNodePointer FindMin(const CNodePointer node) const
	{
		if (node != NULL)
		{
			if (node->left == NULL) return node;
			else return FindMin(node->left);
		}
		else
		{
			return NULL;
		}
	}
	CNodePointer Remove(const CityID& id, CNodePointer& node)
	{
		if (node == NULL) return NULL;
		else if (node->Compare(id) == -1) Remove(id, node->left); //city<id
		else if (node->Compare(id) == 1) Remove(id, node->right); //city>id
		else if (node->left != NULL && node->right != NULL)              //city=id 
		{//有两个孩子
			CNodePointer p = FindMax(node->left);
			node->cityid = p->cityid;
			delete p;
		}
		else                                                      //city=id 
		{//只有一个，或没有孩子
			if (node->left != NULL)
			{//有左孩子
				CNodePointer q = node;  //用于保存删除结点位置，有利于删除
				node = node->left;
				delete q;
			}
			else
			{//有右孩子，或左，右孩子为空
				CNodePointer q = node;  //用于保存删除结点位置，有利于删除
				node = node->right;
				delete q;
			}
		}
		return node;
	}
	void Print(const CNodePointer& T)
	{
		cout << "CITY:" << T->cityid.name << "  " 
			<< "Codinate: (" << T->cityid.coordinate.x <<
			", " << T->cityid.coordinate.y << ") " << endl;
	}
	void PrefixTranverse(const CNodePointer& T)
	{// 前序遍历
		if (T != NULL)  //T非空
		{
			Print(T);
			PrefixTranverse(T->left);
			PrefixTranverse(T->right);
		}
		else return;
	}
	void InfixTranverse(const CNodePointer& T)
	{// 中序遍历
		if (T != NULL)  //T非空
		{
			PrefixTranverse(T->left);
			Print(T);		
			PrefixTranverse(T->right);
		}
		else return;
	}
	void PostfixTranverse(const CNodePointer& T)
	{// 后序遍历
		if (T != NULL)  //T非空
		{
			PrefixTranverse(T->left);
			PrefixTranverse(T->right);
			Print(T);
		}
		else return;
	}
	double Distance(CityID CityIDA, CityID CityIDB)
	{
		double dis = 0;
		dis = sqrt(pow(CityIDA.coordinate.x - CityIDB.coordinate.x, 2) + pow(CityIDA.coordinate.y - CityIDB.coordinate.y, 2));
		return dis;
	}
	void Distance_Recursion(const CNodePointer& Node, const CNodePointer& CITY, const double& X)
	{                                                 //Node根结点开始查找
		double dis = -1;                              //初始化距离为负数，
		if (Node != NULL && CITY != NULL && X > 0)
		{                                             //递归终止：任意一结点为空，或X<=0
			if (Node != CITY)                         //减少碰到自己的情况
				dis = Distance(Node->cityid, CITY->cityid);
			if (dis < X && dis>0)
			{
				Print(Node);                //在区域范围内，打印城市信息
			}
			Distance_Recursion(Node->left, CITY, X);  //递归左子树
			Distance_Recursion(Node->right, CITY, X); //递归右子树
		}
		else return;
	}
	void Input()
	{
		fstream f1;
		f1.open("..\\City_Data_Base\\City_ID.txt", ios::in);                 //以写方式打开分数线文件
		if (!f1) {                                        //文理科标志
			cerr << "File could not be open." << endl;
			abort();
		}
		else
		{
			for (int i = 0; i < M; i++)
			{
				CityID tempID;
				f1 >> tempID.name;
				f1 >> tempID.coordinate.x;
				f1 >> tempID.coordinate.y;;
				Insert(tempID, root);
			}
		}
	}


};
int main()
{
	CityBaseTree T(1);
	T.InfixTranverse(T.root);
	cout << endl << "符合要求的城市：" << endl;
	T.Distance_Recursion(T.root, T.root, 200.0);

	CityID ID;
	ID.name = "city1";
	Set s;
	s.x = 443;
	s.y = -933;
	ID.coordinate = s;

	cout << endl;
	T.Remove(ID, T.root);
	T.PrefixTranverse(T.root);

	T.Insert(ID, T.root);
	cout << endl;
	T.PrefixTranverse(T.root);
	return 0;
}
