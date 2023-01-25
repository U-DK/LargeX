#include "unittest.h"

UnitTest::UnitTest()
{

}

UnitTest::~UnitTest()
{

}

void UnitTest::TestFile(string& s, long long lines)
{
	const char* filePath = s.c_str();
	WriteFile wf(filePath);
	wf.CreateFile(lines);
	ReadFile rf(filePath);
	rf.ReadSingleFile();
	rf.FormatOutput();
}

void UnitTest::TestSmallFile()
{
	string current = "../TargetFiles/info2MB.txt";
	cout << "Writing and reading info2MB.txt..." << endl;
	TestFile(current, LINE2MB);
}

void UnitTest::TestFewLines()
{
	string current = "../TargetFiles/info2.txt";
	cout << "Writing and reading info2.txt..." << endl;
	TestFile(current, LINE2);
}
