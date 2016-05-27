#pragma once

#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;

//可以根据自己所需定制比较器，但是需要合理
template<class T>
class LESS
{       
public:
	bool operator() (const T& left, const T& right)
	{
		return left > right;
	}
};

template<class T>
class BIG
{
public:
	bool operator() (const T& left, const T& right)
	{
		return left < right;
	}
};

template<class T, class Container = LESS<T> >
class Heap
{
public:
	Heap(const T* arr, int size)//建堆时间复杂度  N*lg(N)
	{
		assert(arr);
		array.reserve(size); //事先知道空间大小后，直接开辟需要的空间，不需要在插入过程中扩增容量

		for (int i = 0; i < size; i++)
		{
			array.push_back(arr[i]);
		}

		int begin = array.size() / 2 - 1;
		while (begin >= 0)
		{
			_AdjustDown(begin);
			begin--;          //从下往上调用向下排序就可以完成最小堆排序
		}
	}

	Heap(vector<T>& arr)
	{
		array = arr;
		int begin = array.size() / 2 - 1;
		while (begin >= 0)
		{
			_AdjustDown(begin);
			begin--;            //从下往上调用向下排序就可以完成最小堆排序
		}
	}

	Heap()
	{}

	void Insert(const T& x) //时间复杂度  lg(N)
	{
		array.push_back(x);
		int begin = array.size();
		_AdjustUp(begin);
	}

	void Pop() //时间复杂度lg(N)
	{
		array[0] = array[array.size() - 1];
		array.pop_back();
		_AdjustDown(0);
	}

	void Clear()
	{
		array.clear();
	}

	bool Empty()
	{
		return array.empty();
	}

	const T& Top()
	{
		if (!Empty())
		{
			return array[0];
		}
		exit(1);
	}

	size_t size()
	{
		return array.size();
	}

protected:
	void _AdjustDown(int root)
	{ // 从根节点向下调整
		int size = array.size();
		int left = root * 2 + 1;
		int right = left + 1;
		int key = left;
		while (left < size)
		{//Container()仿函数
			//if (right < size && array[left] > array[right])
			if (right < size && Container()(array[left], array[right]))  //右边小
			{	
				key = right;	
			}
			else
			{	
				key = left;	
			}
			//if ((array[key] > array[root]))
			if (Container()(array[key], array[root]))
			{	
				break;	
			}
			//else if (array[root] > array[key])
			else if (Container()(array[root], array[key])) //左边小
			{
				swap(array[key], array[root]);
			}
			
			root = key;
			left = root * 2 + 1;
			right = left + 1;
		}
	}

	void _AdjustUp(int child)
	{  //从叶子节点或子节点向上调整
		int size = array.size();
		if (size == 0 || size == 1)
		{
			return;
		}

		int root = (child - 2) / 2;
		int left = root * 2 + 1;
		int right = left + 1;
		int key = 0;
		while (root >= 0)
		{
			//if (right < size && array[left] > array[right])
			if (right < size && Container()(array[left], array[right]))
			{
				key = right;
			}
			else
			{
				key = left;
			}
			//if (array[root] > array[key])
			if (Container()(array[root], array[key]))
			{
				swap(array[root], array[key]);
			}
			else
			{
				break;
			}

			if (root == 0)
			{
				break;
			}

			root = (root - 1) / 2;  //root无法小于0，所以循环条件为	root > 0
			left = root * 2 + 1;
			right = left + 1;
		}
	}

private:
	vector<T> array;  //借助vector来当作数据成员
};