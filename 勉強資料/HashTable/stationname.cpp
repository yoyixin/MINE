// stationname.cpp : �R���\�[�� �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
//

#include "stdafx.h"
#include "stationname.h"
#include "readFile.h"
//#include "hashTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// �B��̃A�v���P�[�V���� �I�u�W�F�N�g

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// MFC �̏���������я��������s���̃G���[�̏o��
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: �K�v�ɉ����ăG���[ �R�[�h��ύX���Ă��������B
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		// TODO: ���̈ʒu�ɃA�v���P�[�V�����̓�����L�q���Ă��������B
		cout << "�w�������ւ悱��" << endl;
	}

//	const StationRecord	 station_tbl[] = {
//		{127001,"����"},
//		{127002,"���"},
//		{127003,"���s"},
//		{128001,"���É�"},
//		{128002,"�t����"},
//		{128003,"�_��"},
//		{129001,"������"},
//		{129002,"���t��"},
//		{129003,"��t��"},
//		{129004,"�l���"},
//		{-1,    NULL}
//	};
//
//	HashTable ht;
//	for (int i = 0; station_tbl[i].station_code != -1; i++)
//	{
//		ht.Insert(station_tbl[i].station_code, station_tbl[i]);
//	}
//
//	unsigned long put;
//	cin >> put;
//	Node* ptr = ht.Find(put);
//	cout << ptr->sr.station_code << "  " << ptr->sr.station_name << endl;
//
//	ht.traverse();

	readFile st(readFile::FILENAME::STATION);
//	st.Traverse();


	return nRetCode;
}


