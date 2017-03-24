#include <string>

#define HASH_TABLE_MAX 256

typedef struct T_STATION_RECORD{
	unsigned long station_code;
	const char*	  station_name;
} StationRecord;

class Node{
public:
	Node():next(NULL){}
	//ptr
	Node* next;
	StationRecord sr;
};

//	class LinkList{
//	public:
//		LinkList();
//		~LinkList();
//
//	private:
//		Node* head;
//	};

class HashTable{
public:
	HashTable();
	~HashTable();

	void Insert(unsigned long stCode, StationRecord record);
	Node* Find(unsigned long stCode);
	void traverse();

private:
//	list<StationRecord> ht[256];
	Node *head[HASH_TABLE_MAX];

private:
	int HashFunction(unsigned long stCode);   //hash function

};