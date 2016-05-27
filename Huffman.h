#pragma once

#include"Heap.h"
#include <iostream>
#include <assert.h>
using namespace std;

template<class T>
struct  HuffmanNode
{
	typedef HuffmanNode<T> Node;

	T		    _data;
	Node		*_left;
	Node		*_right;

	HuffmanNode(const T& x = T(), Node *left = NULL, Node *right = NULL)
		:_data(x)
		, _left(left)
		, _right(right)
	{}
};

template<class T>
class HuffmanTree
{
	typedef HuffmanNode<T> Node;
public:
	HuffmanTree()
		:_root(NULL)
	{}

	//ʹ����С�ѽ�������������Ȼ�󷵻ظ��ڵ�
	HuffmanTree(vector<T>& v)
	{  		
		_root = _Create(v, v.size());
	}

	HuffmanTree(const T* arr, size_t size)
	{
		_root = _Create(arr, size);
	}

	HuffmanTree(const T* arr, size_t size, const T filter)
	{ //�ṩ���˹��ܵĹ��캯��
		_root = _Create(arr, size, filter);
	}
	
	~HuffmanTree()
	{
		if (_root)
		{
			_Destroy(_root);
		}
	}

	Node* Root()
	{
		return _root;
	}

protected:
	Node* _Create(const T* arr, size_t size)
	{		
		struct compare
		{
			bool operator()(Node *left, Node *right)
			{
				return left->_data > right->_data;
			}
		};
		
		Heap<Node*, compare>  _heap;

		//���ν����ڵ㲢����vector��
		for (size_t i = 0; i < size; ++i)
		{
			Node *tmp = new Node(arr[i]);
			_heap.Insert(tmp);
		}
		//����ȡ��value��С�������ڵ㣬�����ǵ�value��Ϊ��value�������ǵĸ��ڵ�
		while (_heap.size() > 1)
		{
			Node *left = _heap.Top();
			_heap.Pop();
			Node*right = _heap.Top();
			_heap.Pop();

			Node *parent = new Node(left->_data + right->_data);
			parent->_left = left;
			parent->_right = right;

			_heap.Insert(parent);
		}

		//������ֻʣһ��Ԫ���ˣ�����ǹ��������ĸ��ڵ���
		return _heap.Top();
	}

	Node* _Create(const T* arr, size_t size,const T filter)
	{ //�ṩ�˹��˹��ܵĹ��캯����������Ĺ��캯���߼����
		struct compare
		{
			bool operator ()(const Node *left,const Node *right)
			{
				return left->_data > right->_data;
			}
		};

		Heap<Node*, compare>  _heap;

		for (size_t i = 0; i < size; ++i)
		{
			if (arr[i] != filter)
			{
				Node *tmp = new Node(arr[i]);
				_heap.Insert(tmp);
			}
		}

		while (_heap.size() > 1)
		{
			Node *left = _heap.Top();
			_heap.Pop();
			Node*right = _heap.Top();
			_heap.Pop();

			Node *parent = new Node(left->_data + right->_data);
			parent->_left = left;
			parent->_right = right;

			_heap.Insert(parent);
		}

		return _heap.Top();
	}

	void _Destroy(Node* root)
	{ // �ͷ����Ľڵ㣬�ṩ�������������õ�
		if (root->_left == NULL && root->_right == NULL)
		{
			delete root;
			root = NULL;
		}
		else
		{
			_Destroy(root->_left);
			_Destroy(root->_right);			
		}
	}

private:
	Node    *_root;
};


void Test()
{
	int arr[5] = { 2, 4, 5, 7 };
	HuffmanTree<int> Ht(arr, 4);
}