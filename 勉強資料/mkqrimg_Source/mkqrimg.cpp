// mkqrimg.cpp : mkqrimg コンソール アプリケーション用のエントリ ポイントの定義
// Date 2006/05/17	Ver. 1.12	Psytec Inc.

#include "stdafx.h"
#include "mkqrimg.h"
#include "QR_Encode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// エラーコード
#define ERROR_PARAMETER		1
#define ERROR_CAPACITY		2
#define ERROR_FILEWRITE		3

// パラメータ
static int nLevel = 1; // M
static int nVersion = 0; // 自動
static BOOL bAutoExtent = TRUE;
static int nMaskingNo = -1; // 自動
static int nModuleSize = 4;
static LPSTR lpsSourceData = NULL;
static int ncSourceData = 0;
static char szSavePath[MAX_PATH];

// ファンクション
void CheckCommandlineParam(LPCSTR lpszParam);
void DecodeCommandlineText(LPCSTR lpszParam);
void LoadSourceFile(LPCSTR lpszPath);
int SaveImage();

char hctoi(char c);
void ShowUsage();

/////////////////////////////////////////////////////////////////////////////
// 唯一のアプリケーション オブジェクト

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nReturnCode = NOERROR;
	int i;

	szSavePath[0] = '\0';

	// MFC の初期化および初期化失敗時のエラーの出力
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		return 1;
	}
	else
	{
		if (argc <= 1)
		{
			ShowUsage();
			nReturnCode = ERROR_PARAMETER;
		}
		else
		{
			// コマンドラインパラメータ取り込み
			for (i = 1; i < argc; ++i)
			{
				CheckCommandlineParam(argv[i]);
			}

			if (lpsSourceData == NULL || lstrlen(szSavePath) == 0)
			{
				nReturnCode = ERROR_PARAMETER;
			}
			else
			{
				// エンコード、及びファイル保存
				nReturnCode = SaveImage();
			}

			// バッファ開放
			if (lpsSourceData != NULL)
				HeapFree(GetProcessHeap(), 0, lpsSourceData);

			switch (nReturnCode)
			{
			case ERROR_PARAMETER:
				cerr << _T("パラメータの指定に誤りがあります。") << endl;
				break;

			case ERROR_CAPACITY:
				cerr << _T("ＱＲコードの容量をオーバーしています。") << endl;
				break;

			case ERROR_FILEWRITE:
				cerr << _T("ファイル保存に失敗しました。") << endl;
				break;

			default:
				break;
			}
		}
	}

	return nReturnCode;
}


/////////////////////////////////////////////////////////////////////////////
// CheckCommandlineParam
// 用  途：コマンドライン文字列解析
// 引  数：コマンドライン文字列ブロック

void CheckCommandlineParam(LPCSTR lpszParam)
{
	if (lpszParam[0] != '/')
		return;

	switch (lpszParam[1])
	{
	case 'O': case 'o':
		// 保存ファイル名
		lstrcpyn(szSavePath, lpszParam + 2, MAX_PATH - 1);
		break;

	case 'T': case 't':
		// テキストデータ
		if (ncSourceData == 0)
			DecodeCommandlineText(lpszParam + 2);

		break;

	case 'I': case 'i':
		// ファイル入力
		if (ncSourceData == 0)
			LoadSourceFile(lpszParam + 2);

		break;

	case 'S': case 's':
		// モジュールサイズ
		nModuleSize = atoi(lpszParam + 2);

		if (nModuleSize < 1 || nModuleSize > 20)
			nModuleSize = 4;

		break;

	case 'L': case 'l':
		// 誤り訂正レベル
		nLevel = atoi(lpszParam + 2);

		if (nLevel < 0 || nLevel > 3)
			nLevel = 1;

		break;

	case 'V': case 'v':
		// 型番
		nVersion = atoi(lpszParam + 2);

		if (nVersion < 1 || nVersion > 40)
			nVersion = 0;

		break;

	case 'E': case 'e':
		// 型番
		if (lpszParam[2] == 'N' || lpszParam[2] == 'n')
			bAutoExtent = FALSE;

		break;

	case 'M': case 'm':
		// マスキングパターン番号
		nMaskingNo = atoi(lpszParam + 2);

		if (nMaskingNo < 0 || nMaskingNo > 7)
			nMaskingNo = -1;

		break;

	default:
		break;
	}
}


/////////////////////////////////////////////////////////////////////////////
// DecodeCommandlineText
// 用  途：テキストデータ(16進数表記)のデコード
// 引  数：テキストデータ文字列

void DecodeCommandlineText(LPCSTR lpszParam)
{
	int i;

	// 文字数をカウント
	for (i = 0; i < lstrlen(lpszParam); ++i)
	{
		++ncSourceData;

		if (lpszParam[i] == '%')
			i += 2;
	}

	// バッファ取得
	lpsSourceData = (LPSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ncSourceData);

	int nIndex = 0;

	for (i = 0; i < lstrlen(lpszParam); ++i)
	{
		if (lpszParam[i] != '%')
			lpsSourceData[nIndex++] = lpszParam[i];
		else
		{
			lpsSourceData[nIndex++] = (char)(hctoi(lpszParam[i + 1]) * 16 + hctoi(lpszParam[i + 2]));
			i += 2;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// LoadSourceFile
// 用  途：データファイルの読み込み
// 引  数：ファイルパス

void LoadSourceFile(LPCSTR lpszPath)
{
	// メモリーマップドファイル
	HANDLE hFile = CreateFile(lpszPath, GENERIC_READ, FILE_SHARE_READ, NULL,
							  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return;

	DWORD dwFileSize = GetFileSize(hFile, NULL);

	if (dwFileSize > 0 && dwFileSize != 0xFFFFFFFF)
	{
		// バッファ取得
		lpsSourceData = (LPSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwFileSize);

		DWORD dwRead = 0;

		if (ReadFile(hFile, lpsSourceData, dwFileSize, &dwRead, NULL))
		{
			ncSourceData = dwRead;
		}
		else
		{
			HeapFree(GetProcessHeap(), 0, lpsSourceData);
			lpsSourceData = NULL;
		}
	}

	CloseHandle(hFile);

	return;
}


/////////////////////////////////////////////////////////////////////////////
// SaveImage
// 用  途：ＱＲシンボルイメージの保存
// 戻り値：処理結果

int SaveImage()
{
	int i, j;

	CQR_Encode* pQR_Encode = new CQR_Encode;

	if (! pQR_Encode->EncodeData(nLevel, nVersion, bAutoExtent, nMaskingNo, lpsSourceData, ncSourceData))
	{
		delete pQR_Encode;
		return ERROR_CAPACITY;
	}

	// メモリ上ビットマップにシンボルを描画
	int nSymbleSize = pQR_Encode->m_nSymbleSize + (QR_MARGIN * 2);

	CBitmap* pSymbleBitmap = new CBitmap;
	pSymbleBitmap->CreateBitmap(nSymbleSize, nSymbleSize, 1, 1, NULL);

	CDC* pSymbleDC = new CDC;
	pSymbleDC->CreateCompatibleDC(NULL);

	CBitmap* pOldBitmap1 = pSymbleDC->SelectObject(pSymbleBitmap);

	// 画面消去
	pSymbleDC->PatBlt(0, 0, nSymbleSize, nSymbleSize, WHITENESS);

	// ドット描画
	for (i = 0; i < pQR_Encode->m_nSymbleSize; ++i)
	{
		for (j = 0; j < pQR_Encode->m_nSymbleSize; ++j)
		{
			if (pQR_Encode->m_byModuleData[i][j])
			{
				pSymbleDC->SetPixel(i + QR_MARGIN, j + QR_MARGIN, RGB(0, 0, 0));
			}
		}
	}

	delete pQR_Encode;

	// ファイルに保存
	int nPixelSize = nSymbleSize * nModuleSize;

	// ビットデータサイズ取得
	int nBmpSize = ((nPixelSize + 31) / 32) * 32 / 8;
	nBmpSize *= nPixelSize;

	// メモリーマップドファイル作成
	HANDLE hFile = CreateFile(szSavePath, GENERIC_READ | GENERIC_WRITE, 0, NULL,
							  CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return ERROR_FILEWRITE;
	}

	HANDLE hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE | SEC_COMMIT, 0, 
						  sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2 + nBmpSize, NULL);
	LPBYTE lpbyMapView = (LPBYTE)MapViewOfFile(hFileMapping, FILE_MAP_WRITE, 0, 0, 0);

	ZeroMemory(lpbyMapView, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2 + nBmpSize);

	// 構造体アドレス割付
	LPBITMAPFILEHEADER pbfh = (LPBITMAPFILEHEADER)lpbyMapView;
	LPBITMAPINFO       pbmi = (LPBITMAPINFO)(lpbyMapView + sizeof(BITMAPFILEHEADER));
	LPVOID             pbdt = lpbyMapView + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2;

	// BITMAPFILEHEADER
	pbfh->bfType      = (WORD) (('M' << 8) | 'B'); // "BM"
	pbfh->bfSize      = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2 + nBmpSize;
	pbfh->bfReserved1 = 0;
	pbfh->bfReserved2 = 0;
	pbfh->bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2;

	// LPBITMAPINFO->BITMAPINFOHEADER
	pbmi->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth			= nPixelSize;
	pbmi->bmiHeader.biHeight		= nPixelSize;
	pbmi->bmiHeader.biPlanes		= 1;
	pbmi->bmiHeader.biBitCount		= 1;
	pbmi->bmiHeader.biCompression	= BI_RGB;
	pbmi->bmiHeader.biSizeImage		= nBmpSize;
	pbmi->bmiHeader.biXPelsPerMeter = 3780;
	pbmi->bmiHeader.biYPelsPerMeter = 3780;
	pbmi->bmiHeader.biClrUsed		= 0;
	pbmi->bmiHeader.biClrImportant	= 0;

	CDC* pWorkDC = new CDC;
	pWorkDC->CreateCompatibleDC(NULL);

	CBitmap* pWorkBitmap = new CBitmap;
	pWorkBitmap->CreateBitmap(nPixelSize, nPixelSize, 1, 1, NULL);

	CBitmap* pOldBitmap2 = pWorkDC->SelectObject(pWorkBitmap);
	pWorkDC->StretchBlt(0, 0, nPixelSize, nPixelSize, pSymbleDC, 0, 0, nSymbleSize, nSymbleSize, SRCCOPY);

	GetDIBits(pWorkDC->m_hDC, (HBITMAP)*pWorkBitmap, 0, nPixelSize, pbdt, pbmi, DIB_RGB_COLORS);

	pWorkDC->SelectObject(pOldBitmap2);
	delete pWorkBitmap;
	delete pWorkDC;

	UnmapViewOfFile(lpbyMapView);
	CloseHandle(hFileMapping);
	CloseHandle(hFile);

	pSymbleDC->SelectObject(pOldBitmap1);
	delete pSymbleBitmap;
	delete pSymbleDC;

	return NOERROR;
}


/////////////////////////////////////////////////////////////////////////////
// htoi
// 用  途：16進数文字列の整数化
// 引  数：対象文字
// 戻り値：整数化数値

char hctoi(char c)
{
	if (c >= '0' && c <= '9')
	{
		return (char)(c - '0');
	}

	else if (c >= 'A' && c <= 'F')
	{
		return (char)(c - 'A' + 10);
	}

	else if (c >= 'a' && c <= 'f')
	{
		return (char)(c - 'a' + 10);
	}

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// ShowUsage
// 用  途：使用方法表示

void ShowUsage()
{
	cout << 
	"QRコードシンボルのビットマップイメージを作成します。\r\n"
	"\r\n"
	"MKQRBMP /O\"出力ファイル\" /T\"テキストデータ\" [/I\"入力データファイル\"] [/Sn] [/Ln] [/Vn] [/E(y/n)] [/Mn]\r\n"
	"\r\n"
	"  /O      出力ビットマップファイル名を指定します。\r\n"
	"  /T      エンコードするテキストを指定します。\r\n"
	"          不可視文字および'%'、ダブルクォーテーションは'%'により\r\n"
	"          16進表記してください。\r\n"
	"  /I      テキスト指定の代わりにファイルを指定します。\r\n"
	"  /Sn     モジュールサイズを指定します。n=1～20、デフォルト=4\r\n"
	"  /Ln     誤り訂正レベルを指定します。n=0(L), 1(M). 2(Q), 3(H)、デフォルト=1\r\n"
	"  /Vn     型番を指定します。n=1～40、省略時は自動選択されます。\r\n"
	"  /E(y/n) 指定型番に収まらなかった場合に大きな型番を自動選択するかどうかを指定\r\n"
	"          します。デフォルト=y\r\n"
	"  /Mn     マスキングパターン番号を指定します。n=0～7、省略時は自動選択されます。\r\n"
	"  Copyright (C) 2006 Psytec Inc."
	<< endl;
}
