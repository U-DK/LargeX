#pragma once
#include <vector>
#include <algorithm>
using namespace std;



struct DataInfo
{
	long uuid;
	int value;
};

struct DataCompare {
	bool operator()(const DataInfo& a, const DataInfo& b) {
		return a.value > b.value;
	}
};

class Heap
{
public:
	//Heap();
	Heap(int max);
	~Heap();
	DataInfo Peek();
	void Pop();
	void DownHeap();
	void PushBack(DataInfo info);
	void FormatOutput();
	DataInfo GetData(int i);
	int GetSize();
	void Resize(int i);
	
private:
	vector<DataInfo> vec;
	int capacity;
	int size;
};

//Heap::Heap() :capacity(5), size(0)
//{	
//	for (int i = 0; i != capacity; ++i)
//	{
//		DataInfo f = { 0,0 };
//		vec.push_back(f);
//	}
//	
//
//}

