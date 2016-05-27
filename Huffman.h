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

	//使用最小堆建立哈弗曼树，然后返回根节点
	HuffmanTree(vector<T>& v)
	{  		
		_root = _Create(v, v.size());
	}

	HuffmanTree(const T* arr, size_t size)
	{
		_root = _Create(arr, size);
	}

	HuffmanTree(const T* arr, size_t size, const T filter)
	{ //提供过滤功能的构造函数
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

		//依次建立节点并放入vector中
		for (size_t i = 0; i < size; ++i)
		{
			Node *tmp = new Node(arr[i]);
			_heap.Insert(tmp);
		}
		//依次取出value最小的两个节点，以他们的value和为新value建立他们的父节点
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

		//最后堆中只剩一个元素了，这就是哈弗曼树的根节点了
		return _heap.Top();
	}

	Node* _Create(const T* arr, size_t size,const T filter)
	{ //提供了过滤功能的构造函数，和上面的构造函数逻辑差不多
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
	{ // 释放树的节点，提供给析构函数调用的
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