#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <cstdlib>
//#include <ctime>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iomanip>
#include "heap.h"
#include <thread>
#include <random>
using namespace std;
extern int topX;
extern int coresCPU;
//How many files we split a large file to
extern int fileCounts;
#define MAXVALUE 99999
#define UUIDLEN 16
#define VALUELEN 5

#define LINEPERCPU 500000

#define LINE4GB 180000000
#define LINE2GB 90000000
#define LINE1GB 45000000
#define LINE512MB 22400000
#define LINE256MB 11200000
#define LINE128MB 5600000
#define LINE64MB 2800000
#define LINE32MB 1400000
#define LINE16MB 700000
#define LINE8MB 350000
#define LINE4MB 175000
#define LINE2MB 88000
#define LINE2 2


#ifdef _WIN32
//Windows \r\n
#define LINEMAXLEN 24
#else
//Others \r
#define LINEMAXLEN 23
#endif

void ShowProgress(float percent);



class WriteFile
{
public:
	WriteFile();
	WriteFile(const char* path);
	~WriteFile();
	void CreateLargeFile();
	void CreateFile(int lines = LINE2MB);
	void GetFileSize(const char* fileName);
	int GetRandomNum(uniform_int_distribution<int>& distribution, mt19937& gen);
	void SplitLargeFiles();
	void WriteToFile(ofstream& wf, const char* s, bool flag);
private:
	string filePath;
	int uuidLength;
	int valueLength;
};


class ReadFile
{
public:
	ReadFile();
	ReadFile(const char* path);
	~ReadFile();
	void ReadSingleFile();
	void ReadSingleFile(string& s);
	void ReadLargeFile();
	void ParseLine(string& s);
	void Split(const string& s, vector<string>& v, const string& c);
	void FormatOutput();
	Heap& GetHeap();
	int threadID;

private:
	string filePath;
	Heap heap;
	
};
