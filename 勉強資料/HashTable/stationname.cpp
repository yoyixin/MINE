// stationname.cpp : コンソール アプリケーション用のエントリ ポイントの定義
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
// 唯一のアプリケーション オブジェクト

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// MFC の初期化および初期化失敗時のエラーの出力
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 必要に応じてエラー コードを変更してください。
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		// TODO: この位置にアプリケーションの動作を記述してください。
		cout << "駅名検索へよこそ" << endl;
	}

//	const StationRecord	 station_tbl[] = {
//		{127001,"草津"},
//		{127002,"大津"},
//		{127003,"京都"},
//		{128001,"名古屋"},
//		{128002,"春日井"},
//		{128003,"神領"},
//		{129001,"清水寺"},
//		{129002,"金閣寺"},
//		{129003,"銀閣寺"},
//		{129004,"四条通"},
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


