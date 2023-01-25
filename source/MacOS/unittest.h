#pragma once
#include "tool.h"

class UnitTest
{
public:
	UnitTest();
	~UnitTest();
	void TestFile(string& s, long long lines);
	void TestSmallFile();
	void TestFewLines();
	void TestBorders();
private:
};