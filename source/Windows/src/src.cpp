
#include "tool.h"
#include "unittest.h"
#include <format>
#include <intrin.h>
#include <chrono>


//#ifndef _WIN32
//#include <unistd.h>
//#include <sys/sysinfo.h>
//#else
//#include <windowsx.h>
//#endif

void CheckDirectory();
void GenerateAndSplitLargeFile();
void SingleThreadRead(int threadID);
void CombineAllResult();
void ShowTimeConsuming(clock_t& startTime, clock_t& endTime);

//void GetCPUInfo();
//void cpuID(unsigned i, unsigned regs[4]);
vector<ReadFile*> vecRead;
int main(int argc, char* argv[])
{
    cout << "Hello World!\n";//Salute!
	CheckDirectory();
	clock_t startTime;
	clock_t endTime;

	int x = 0;
	cout << "Please enter x (for your convenience, entering a number less than 1000 would be better): " << endl;
	cin >> x;
	if (x<=0)
	{
		cout << "x is less than 0, process ends... " << endl;
		return 0;
		system("pause");
	}
	if (x>10000)
	{
		cout << "x is more than 10000, process ends... " << endl;
		return 0;
		system("pause");
	}

	topX = x;
	string cmd1;
	string cmd2;

	if (argc > 1)
	{
		cmd1 = argv[1];
	}
	if (argc > 2)
	{
		cmd2 = argv[2];
	}
	if (0 == cmd1.compare("unittest"))
	{
		if (cmd2.empty())
		{
			cout << "No UnitTest found!" << endl;
			system("pause");
			return 0;
		}
		startTime = clock();
		UnitTest ut;
		if (0 == cmd2.compare("1"))
		{
			ut.TestSmallFile();
		}
		if (0 == cmd2.compare("2"))
		{
			ut.TestFewLines();
		}

		endTime = clock();
		ShowTimeConsuming(startTime, endTime);
		system("pause");
		return 0;
	}

	startTime = clock();
	GenerateAndSplitLargeFile();
	endTime = clock();
	ShowTimeConsuming(startTime, endTime);

	coresCPU = thread::hardware_concurrency();
	cout << "This device has CPU thread count: " << coresCPU << endl;
	coresCPU -= 2;//Not using them all, save resources for other program.
	if (coresCPU <= 1)
	{
		coresCPU = 1;
	}
	cout << "We are using: " << coresCPU << endl;

	int optimization = min(coresCPU, fileCounts);//If just few files are there, we don't need to create all the threads.
	vector<thread> vecThread;

	cout << "Next is processing from all of small files, please wait..."<< endl;
	cout << endl;

	startTime = clock();
	for (int i = 0; i !=optimization; ++i)
	{
		//void (*fp)(int) = &SingleThreadRead;
		//thread th(SingleThreadRead, i);
		vecThread.push_back(thread(SingleThreadRead, i));
	}

	for (auto& th : vecThread)
	{
		th.join();
	}

	CombineAllResult();
	endTime = clock();
	ShowTimeConsuming(startTime, endTime);

	for (int i = 0; i != vecRead.size(); ++i)
	{
		delete vecRead[i];
		vecRead[i] = NULL;
	}

	system("pause");
}

void CombineAllResult()
{
	Heap heap(topX);
	for (int i = 0; i != vecRead.size(); ++i)
	{
		auto temp = (*vecRead[i]).GetHeap();
		for (int j = 0; j != temp.GetSize(); ++j)
		{
			heap.PushBack(temp.GetData(j));
		}
	}

	for (int i = 0; i != topX; ++i)
	{
		long uuid = heap.GetData(i).uuid;
		int value = heap.GetData(i).value;
		cout << "uuid: " << uuid << " value: " << value << endl;
	}
}

void SingleThreadRead(int threadID)
{

	ReadFile* rf=new ReadFile();
	rf->threadID = threadID;
	rf->ReadLargeFile();
	vecRead.push_back(rf);
	string idmeg = format("thread {} is done", threadID);
	cout << idmeg << endl;

}

void GenerateAndSplitLargeFile()
{
	WriteFile wf;
	wf.CreateLargeFile();
	wf.SplitLargeFiles();
}


void CheckDirectory()
{
	string current = "../TargetFiles";

	const char* dirPath = current.c_str();
	struct _stat64 sb;
	if (_stat64(dirPath, &sb) != 0)
	{
		cout << "Not valid path" << endl;
		return;
	}
	for (const auto& entry : filesystem::directory_iterator(dirPath))
	{
		filesystem::remove_all(entry.path());
	}
	cout << "Directory cleared!" << endl;

}

void ShowTimeConsuming(clock_t& startTime, clock_t& endTime)
{
	auto interval = endTime - startTime;
	auto span = (double)interval;
	double sec = span / CLOCKS_PER_SEC;
	string timeString = format("{:<4.2f}s", sec);//to_string(sec) + 's';
	if (sec>200.0f)
	{
		int s = floor(sec);
		int m = s / 60;
		s = s % 60;
		
		timeString = format("{}m{}s", m, s);
	}

	cout << "time used: " << timeString << endl;
}



//void GetCPUInfo()
//{
//	unsigned regs[4];
//
//	// Get vendor
//	char vendor[12];
//	cpuID(0, regs);
//	((unsigned*)vendor)[0] = regs[1]; // EBX
//	((unsigned*)vendor)[1] = regs[3]; // EDX
//	((unsigned*)vendor)[2] = regs[2]; // ECX
//	string cpuVendor = string(vendor, 12);
//
//	// Get CPU features
//	cpuID(1, regs);
//	unsigned cpuFeatures = regs[3]; // EDX
//
//	// Logical core count per CPU
//	cpuID(1, regs);
//	unsigned logical = (regs[1] >> 16) & 0xff; // EBX[23:16]
//	cout << " logical cpus: " << logical << endl;
//	unsigned cores = logical;
//
//	if (cpuVendor == "GenuineIntel") {
//		// Get DCP cache info
//		cpuID(4, regs);
//		cores = ((regs[0] >> 26) & 0x3f) + 1; // EAX[31:26] + 1
//
//	}
//	else if (cpuVendor == "AuthenticAMD") {
//		// Get NC: Number of CPU cores - 1
//		cpuID(0x80000008, regs);
//		cores = ((unsigned)(regs[2] & 0xff)) + 1; // ECX[7:0] + 1
//	}
//
//	cout << "    cpu cores: " << cores << endl;
//
//	// Detect hyper-threads  
//	bool hyperThreads = cpuFeatures & (1 << 28) && cores < logical;
//
//	cout << "hyper-threads: " << (hyperThreads ? "true" : "false") << endl;
//}
//
//void cpuID(unsigned i, unsigned regs[4]) 
//{
//#ifdef _WIN32
//	__cpuid((int*)regs, (int)i);
//
//#else
//	asm volatile
//		("cpuid" : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
//			: "a" (i), "c" (0));
//	// ECX is set to zero for CPUID function 4
//#endif
//}