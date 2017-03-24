#include "StdAfx.h"
#include "hashTable.h"

using namespace std;

/**************************************************

**************************************************/
HashTable::HashTable()
{
	for(int i = 0; i < HASH_TABLE_MAX; i++){
		head[i] = 0;
	}
}

/**************************************************

**************************************************/
HashTable::~HashTable()
{
	delete [] *head;
}

/**************************************************

**************************************************/
int HashTable::HashFunction(unsigned long stCode)
{
	int ret = stCode % 1000;
	if(ret >= HASH_TABLE_MAX)
	{
		ret = 0;
	}
	return ret;
}

void HashTable::Insert(unsigned long stCode, StationRecord record)
{
	int hash = HashFunction(stCode);
	if(0 == head[hash])
	{
		head[hash] = new Node();
		head[hash]->sr = record;
	} else {
		Node* ptr = new Node();
		ptr->next = head[hash];
		ptr->sr = record;
		head[hash] = ptr;
	}
}

Node* HashTable::Find(unsigned long stCode)
{
	Node* ret;
	int hash = HashFunction(stCode);
	if(0 == head[hash])
	{
		ret = new Node();
	} else {
		Node* ptr;
		ptr = head[hash];
		while(ptr->next)
		{
			if(ptr->sr.station_code == stCode) break;
			ptr = ptr->next;
		}
		ret = ptr;
	}
	return ret;
}

void HashTable::traverse()
{
	Node* ptr;
	for(int i = 0; i < HASH_TABLE_MAX; i++)
	{
		if(0 == head[i])
		{
			cout << i << "  ";
		} else {
			ptr = head[i];
			cout << i << "  ";
			cout << ptr->sr.station_name << "  ";
			while(ptr->next)
			{
				ptr = ptr->next;
				cout << ptr->sr.station_name << "  ";
			}
		}
		cout << "\n";
	}
}