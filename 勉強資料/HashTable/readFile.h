#include <fstream>
#include "hashTable.h"

#define V_STATION "v_station.csv"

class readFile
{
public:
	readFile(int file);
	~readFile();

	enum FILENAME{
		STATION = 1,
		COMPANY,
		LINE,
	};

public:
	void Find(unsigned long stCode);
	void Traverse();
	
private:
	const char* filename;
	HashTable  stTable;
};