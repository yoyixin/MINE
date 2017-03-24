// ポイント編.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include "PointTest.h"
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
	string str("Jonh Smith");
	char name[10];
	strcpy(name, str.c_str());

//	printf("%d", pt.print());
	return 0;
}
