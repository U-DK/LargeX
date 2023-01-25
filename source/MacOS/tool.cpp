#include "tool.h"
int topX = 0;
int coresCPU = 1;
float tempPercent = 0;
int fileCounts = 0;

//Show split process
void ShowProgress(float percent)
{
	//control progress display every 0.2% not too fast not too slow
	if (percent < 1 && percent - tempPercent < 0.002f)
	{
		return;
	}

	tempPercent = percent;
	int bar = percent * 50;
	fflush(stdout);
	cout.flush();
	putchar('\r');
	//cout << "\33[1A";
	cout << "[" + string(bar, '=') + ">" + string(50 - bar, ' ') + "] " << fixed << setprecision(1) << 100 * percent << '%';

}


WriteFile::WriteFile() :filePath("../defaultinfo.txt"), uuidLength(UUIDLEN), valueLength(VALUELEN)
{

}

WriteFile::WriteFile(const char* path): filePath(path), uuidLength(UUIDLEN), valueLength(VALUELEN)
{

}

WriteFile::~WriteFile()
{

}

void WriteFile::CreateLargeFile()
{
	if (filePath.empty())
	{
		cout << "File path is not legal" << endl;
		return;
	}
	string largePath = "../TargetFiles/largeinfo.txt";
	ofstream wf;
	wf.open(largePath, ios::out);
	long temp = 0;
	if (wf.is_open())
	{
		long counts = LINE2GB;
		float percent = 0;
		//srand(time(0));
		tempPercent = 0;
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<int> distribution(1, MAXVALUE);
		cout <<"Generating the large file, please wait..."<< endl;
		for (int i=0;i!=counts;++i)
		{
			wf << setw(uuidLength) << setfill('0') << i << " " << GetRandomNum(distribution,gen) << endl;
			percent = (float)i / counts;

			if (i>=counts-1)
			{
				percent = 1;
			}
			ShowProgress(percent);
		}

		cout << endl;

	}
	else
	{
		cout << "write failed!" << endl;
	}
	wf.close();

}

void WriteFile::CreateFile(int lines)
{
	ofstream wf;
	wf.open(filePath, ios::out);
	if (wf.is_open())
	{
		char enter = 10;
		float percent = 0;
		tempPercent = 0;

		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<int> distribution(1, MAXVALUE);

		for (int i=0;i!=lines;++i)
		{
			wf << setw(uuidLength) << setfill('0') << i << " " << GetRandomNum(distribution, gen) << enter;
			percent = (float)i / lines;

			if (i >= lines - 1)
			{
				percent = 1;
			}
			ShowProgress(percent);
		}
		cout << endl;

	}
	else
	{
		cout << "write failed!" << endl;
	}
	wf.close();
}

void WriteFile::GetFileSize(const char* fileName)
{
	// if (NULL == fileName)
	// {
	// 	return;
	// }

	// struct stat statbuff;
	// stat(fileName, &statbuff);
	// size_t fileSize = statbuff.st_size;
	// cout << "file size: " << fileSize << endl;
}

int WriteFile::GetRandomNum(uniform_int_distribution<int>& distribution, mt19937& gen)
{

	int a = distribution(gen);

	return a;
}

void WriteFile::SplitLargeFiles()
{
	const char* largeFilePath = "../TargetFiles/largeinfo.txt";
	struct stat sb;
	if (stat(largeFilePath, &sb) != 0)
	{
		cout << "Not valid path" << endl;
		return;
	}

	struct stat statbuff;
	stat(largeFilePath, &statbuff);
	size_t fileSize = statbuff.st_size;
	fileCounts = fileSize / (LINEMAXLEN * LINEPERCPU);
	if (0!=fileSize % (LINEMAXLEN * LINEPERCPU))
	{
		++fileCounts;
	}

	cout << "File size: "<<fileSize<<" bytes"<< endl;
	cout << "Every thread will deal with "<<(LINEMAXLEN * LINEPERCPU)<<" bytes" << endl;
	cout << "Small files amount: "<<fileCounts<< endl;
	cout << "Now split file, please wait..." << endl;

	ifstream file(largeFilePath, ios::in);
	if (!file.good())
	{
		cout << "failed to open file" << endl;
	}

	char* line = new char[LINEMAXLEN];
	int len = LINEMAXLEN;
	int currentLines = 0;
	int index = 0;
	string tmpName;
	ofstream wf;
	float percent;
	tempPercent = 0;
	while (file.getline(line, len))
	{


		if (0 == currentLines % LINEPERCPU)
		{


			currentLines = 1;
			++index;
			tmpName = "../TargetFiles/largeinfo_" + to_string(index) + ".txt";
			wf.close();
			wf.open(tmpName, ios::out);
			//cout << " " << index << "/" << fileCounts << endl;;
			//cout << "\33[1A";
			percent = (float)index / fileCounts;
			if (index >= fileCounts)
			{
				percent = 1;
			}
			ShowProgress(percent);
		}
		else
			++currentLines;
		WriteToFile(wf, line, currentLines == LINEPERCPU);


	}
	wf.close();
	file.close();
	delete[] line;
	cout << endl;
	
}

void WriteFile::WriteToFile(ofstream& wf, const char* s, bool flag)
{
	if (wf.is_open())
	{
		wf << s;
		if (!flag)
		{
			wf << (char)10;
		}
		return;
	}
	cout << "write failed!" << endl;
}

ReadFile::ReadFile() :filePath("../defaultinfo.txt"), heap(topX), threadID(-1)
{

}

ReadFile::ReadFile(const char* path):filePath(path), heap(topX), threadID(-1)
{

}

ReadFile::~ReadFile()
{
	   
}

void ReadFile::ReadSingleFile()
{
	ReadSingleFile(filePath);

}

void ReadFile::ReadSingleFile(string& s)
{
	ifstream file(s.c_str(), ios::in);
	if (!file.good())
	{
		cout << "failed to open file" << endl;
		return;
	}

	char* line = new char[LINEMAXLEN];
	int len = LINEMAXLEN;
	while (file.getline(line, len))
	{
		//cout << "char*: " << line << endl;
		string s = line;
		ParseLine(s);
	}
	delete[] line;
	line = NULL;
	file.close();
}

void ReadFile::ReadLargeFile()
{
	//ReadSingleFile(filePath);
	string tmpName;
	int index = 0;
	int fileID = threadID + 1;
	string info;
	while (fileID <= fileCounts)
	{
		tmpName = "../TargetFiles/largeinfo_" + to_string(fileID) + ".txt";
		// tmpName = format("../TargetFiles/largeinfo_{}.txt", fileID);
		info = "thread " + to_string(threadID) + " is processing: " + tmpName + "\n";
		// info = format("thread {} is processing {} \n", threadID, tmpName);
		//cout <<"thread "<<threadID<< " is processing: " << tmpName << endl;
		cout << info;
		ReadSingleFile(tmpName);
		++index;
		fileID = threadID + 1 + index * coresCPU;
	}
}

void ReadFile::ParseLine(string& s)
{
	vector<string> vec;
	Split(s, vec, " ");
	DataInfo info = { stol(vec[0]),stoi(vec[1]) };
	heap.PushBack(info);
}

//Split a string based on another string
void ReadFile::Split(const string& s, vector<string>& v, const string& c)
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (pos2 != string::npos)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));
		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);

	}

	if (pos1!=s.length())
	{
		v.push_back(s.substr(pos1));
	}
}

void ReadFile::FormatOutput()
{
	for (int i=0;i!=heap.GetSize();++i)
	{
		long uuid = heap.GetData(i).uuid;
		int value = heap.GetData(i).value;
		cout << "uuid: " << uuid << " value: " << value << endl;
	}
}

Heap& ReadFile::GetHeap()
{
	return heap;
}
