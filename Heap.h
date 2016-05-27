#pragma once

#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;

//���Ը����Լ����趨�ƱȽ�����������Ҫ����
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
	Heap(const T* arr, int size)//����ʱ�临�Ӷ�  N*lg(N)
	{
		assert(arr);
		array.reserve(size); //����֪���ռ��С��ֱ�ӿ�����Ҫ�Ŀռ䣬����Ҫ�ڲ����������������

		for (int i = 0; i < size; i++)
		{
			array.push_back(arr[i]);
		}

		int begin = array.size() / 2 - 1;
		while (begin >= 0)
		{
			_AdjustDown(begin);
			begin--;          //�������ϵ�����������Ϳ��������С������
		}
	}

	Heap(vector<T>& arr)
	{
		array = arr;
		int begin = array.size() / 2 - 1;
		while (begin >= 0)
		{
			_AdjustDown(begin);
			begin--;            //�������ϵ�����������Ϳ��������С������
		}
	}

	Heap()
	{}

	void Insert(const T& x) //ʱ�临�Ӷ�  lg(N)
	{
		array.push_back(x);
		int begin = array.size();
		_AdjustUp(begin);
	}

	void Pop() //ʱ�临�Ӷ�lg(N)
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
	{ // �Ӹ��ڵ����µ���
		int size = array.size();
		int left = root * 2 + 1;
		int right = left + 1;
		int key = left;
		while (left < size)
		{//Container()�º���
			//if (right < size && array[left] > array[right])
			if (right < size && Container()(array[left], array[right]))  //�ұ�С
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
			else if (Container()(array[root], array[key])) //���С
			{
				swap(array[key], array[root]);
			}
			
			root = key;
			left = root * 2 + 1;
			right = left + 1;
		}
	}

	void _AdjustUp(int child)
	{  //��Ҷ�ӽڵ���ӽڵ����ϵ���
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

			root = (root - 1) / 2;  //root�޷�С��0������ѭ������Ϊ	root > 0
			left = root * 2 + 1;
			right = left + 1;
		}
	}

private:
	vector<T> array;  //����vector���������ݳ�Ա
};