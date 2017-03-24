#include "StdAfx.h"
#include "common.h"
#include "readFile.h"

using namespace std;

readFile::readFile(int file)
{
	switch(file)
	{
	case FILENAME::STATION:
		filename = V_STATION;
	default:
		filename = V_STATION;
	}

	ifstream f(V_STATION);
	ofstream log("log.txt");
	if(!f){
		cout << "FILE OPEN ERROR" << endl;
	}else{
		string l;
		while(getline(f,l))
		{
			if(l.size() == 0 || l.at(0) == '#') {continue;}
			StrIter siter(l);
			string s_st,s_line,s_st_name;
			siter.next_one(s_st,',');
			siter.next_one(s_line,',');
			siter.next_one(s_st_name,',');
			ostreamstream oss;
			s_st += s_line;
			unsigned long l_st = atol(s_st.c_str());
			StationRecord sr;
			sr.station_code=l_st;
			sr.station_name=s_st_name.c_str();
			stTable.Insert(l_st,sr);

			log << l_st << "," << s_st_name << "\n";
		}
	}
	f.close();
	log.close();
}

readFile::~readFile(){}

void readFile::Find(unsigned long stCode){}

void readFile::Traverse()
{
	stTable.traverse();
}