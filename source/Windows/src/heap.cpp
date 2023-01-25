#include "heap.h"
#include <iostream>

Heap::Heap(int max) :capacity(max), size(0)
{
	for (int i = 0; i != capacity; ++i)
	{
		DataInfo f = { 0,0 };
		vec.push_back(f);
		
	}
}

Heap::~Heap()
{

}

DataInfo Heap::Peek()
{
	return GetData(0);
}

void Heap::Pop()
{


}

void Heap::DownHeap()
{
	int bg = 0;
	int ed = capacity - 1;
	int parent = bg;
	int child = 2 * parent + 1;
	while (parent <= ed && child <= ed)
	{
		int temp = child + 1;
		if (temp <= ed && vec[child].value > vec[temp].value)
		{
			++child;
		}
		if (vec[parent].value > vec[child].value)
		{
			DataInfo info = vec[parent];
			vec[parent] = vec[child];
			vec[child] = info;
			parent = (parent - 1) / 2;
			child = 2 * parent + 1;
			continue;
		}
		parent = child;
		child = 2 * parent + 1;

	}

}

void Heap::PushBack(DataInfo info)
{
	if (size >= capacity && info.value > vec[0].value)
	{
		vec[0] = info;
		//DownHeap();
		make_heap(vec.begin(), vec.end(), DataCompare());
		return;
	}
	if (size < capacity)
	{
		vec[size] = info;
		++size;
	}
}

void Heap::FormatOutput()
{
	for (int i = 0; i != capacity; ++i)
	{
		cout << vec[i].value << endl;
	}
}

DataInfo Heap::GetData(int i)
{
	if (i >= size)
	{
		return { -1,-1 };
	}
	return vec[i];
}

int Heap::GetSize()
{
	return size;
}

void Heap::Resize(int i)
{
	vec.resize(i);
	capacity = i;
}
