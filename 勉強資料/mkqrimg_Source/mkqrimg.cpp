// mkqrimg.cpp : mkqrimg �R���\�[�� �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
// Date 2006/05/17	Ver. 1.12	Psytec Inc.

#include "stdafx.h"
#include "mkqrimg.h"
#include "QR_Encode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// �G���[�R�[�h
#define ERROR_PARAMETER		1
#define ERROR_CAPACITY		2
#define ERROR_FILEWRITE		3

// �p�����[�^
static int nLevel = 1; // M
static int nVersion = 0; // ����
static BOOL bAutoExtent = TRUE;
static int nMaskingNo = -1; // ����
static int nModuleSize = 4;
static LPSTR lpsSourceData = NULL;
static int ncSourceData = 0;
static char szSavePath[MAX_PATH];

// �t�@���N�V����
void CheckCommandlineParam(LPCSTR lpszParam);
void DecodeCommandlineText(LPCSTR lpszParam);
void LoadSourceFile(LPCSTR lpszPath);
int SaveImage();

char hctoi(char c);
void ShowUsage();

/////////////////////////////////////////////////////////////////////////////
// �B��̃A�v���P�[�V���� �I�u�W�F�N�g

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nReturnCode = NOERROR;
	int i;

	szSavePath[0] = '\0';

	// MFC �̏���������я��������s���̃G���[�̏o��
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
			// �R�}���h���C���p�����[�^��荞��
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
				// �G���R�[�h�A�y�уt�@�C���ۑ�
				nReturnCode = SaveImage();
			}

			// �o�b�t�@�J��
			if (lpsSourceData != NULL)
				HeapFree(GetProcessHeap(), 0, lpsSourceData);

			switch (nReturnCode)
			{
			case ERROR_PARAMETER:
				cerr << _T("�p�����[�^�̎w��Ɍ�肪����܂��B") << endl;
				break;

			case ERROR_CAPACITY:
				cerr << _T("�p�q�R�[�h�̗e�ʂ��I�[�o�[���Ă��܂��B") << endl;
				break;

			case ERROR_FILEWRITE:
				cerr << _T("�t�@�C���ۑ��Ɏ��s���܂����B") << endl;
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
// �p  �r�F�R�}���h���C����������
// ��  ���F�R�}���h���C��������u���b�N

void CheckCommandlineParam(LPCSTR lpszParam)
{
	if (lpszParam[0] != '/')
		return;

	switch (lpszParam[1])
	{
	case 'O': case 'o':
		// �ۑ��t�@�C����
		lstrcpyn(szSavePath, lpszParam + 2, MAX_PATH - 1);
		break;

	case 'T': case 't':
		// �e�L�X�g�f�[�^
		if (ncSourceData == 0)
			DecodeCommandlineText(lpszParam + 2);

		break;

	case 'I': case 'i':
		// �t�@�C������
		if (ncSourceData == 0)
			LoadSourceFile(lpszParam + 2);

		break;

	case 'S': case 's':
		// ���W���[���T�C�Y
		nModuleSize = atoi(lpszParam + 2);

		if (nModuleSize < 1 || nModuleSize > 20)
			nModuleSize = 4;

		break;

	case 'L': case 'l':
		// ���������x��
		nLevel = atoi(lpszParam + 2);

		if (nLevel < 0 || nLevel > 3)
			nLevel = 1;

		break;

	case 'V': case 'v':
		// �^��
		nVersion = atoi(lpszParam + 2);

		if (nVersion < 1 || nVersion > 40)
			nVersion = 0;

		break;

	case 'E': case 'e':
		// �^��
		if (lpszParam[2] == 'N' || lpszParam[2] == 'n')
			bAutoExtent = FALSE;

		break;

	case 'M': case 'm':
		// �}�X�L���O�p�^�[���ԍ�
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
// �p  �r�F�e�L�X�g�f�[�^(16�i���\�L)�̃f�R�[�h
// ��  ���F�e�L�X�g�f�[�^������

void DecodeCommandlineText(LPCSTR lpszParam)
{
	int i;

	// ���������J�E���g
	for (i = 0; i < lstrlen(lpszParam); ++i)
	{
		++ncSourceData;

		if (lpszParam[i] == '%')
			i += 2;
	}

	// �o�b�t�@�擾
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
// �p  �r�F�f�[�^�t�@�C���̓ǂݍ���
// ��  ���F�t�@�C���p�X

void LoadSourceFile(LPCSTR lpszPath)
{
	// �������[�}�b�v�h�t�@�C��
	HANDLE hFile = CreateFile(lpszPath, GENERIC_READ, FILE_SHARE_READ, NULL,
							  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return;

	DWORD dwFileSize = GetFileSize(hFile, NULL);

	if (dwFileSize > 0 && dwFileSize != 0xFFFFFFFF)
	{
		// �o�b�t�@�擾
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
// �p  �r�F�p�q�V���{���C���[�W�̕ۑ�
// �߂�l�F��������

int SaveImage()
{
	int i, j;

	CQR_Encode* pQR_Encode = new CQR_Encode;

	if (! pQR_Encode->EncodeData(nLevel, nVersion, bAutoExtent, nMaskingNo, lpsSourceData, ncSourceData))
	{
		delete pQR_Encode;
		return ERROR_CAPACITY;
	}

	// ��������r�b�g�}�b�v�ɃV���{����`��
	int nSymbleSize = pQR_Encode->m_nSymbleSize + (QR_MARGIN * 2);

	CBitmap* pSymbleBitmap = new CBitmap;
	pSymbleBitmap->CreateBitmap(nSymbleSize, nSymbleSize, 1, 1, NULL);

	CDC* pSymbleDC = new CDC;
	pSymbleDC->CreateCompatibleDC(NULL);

	CBitmap* pOldBitmap1 = pSymbleDC->SelectObject(pSymbleBitmap);

	// ��ʏ���
	pSymbleDC->PatBlt(0, 0, nSymbleSize, nSymbleSize, WHITENESS);

	// �h�b�g�`��
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

	// �t�@�C���ɕۑ�
	int nPixelSize = nSymbleSize * nModuleSize;

	// �r�b�g�f�[�^�T�C�Y�擾
	int nBmpSize = ((nPixelSize + 31) / 32) * 32 / 8;
	nBmpSize *= nPixelSize;

	// �������[�}�b�v�h�t�@�C���쐬
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

	// �\���̃A�h���X���t
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
// �p  �r�F16�i��������̐�����
// ��  ���F�Ώە���
// �߂�l�F���������l

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
// �p  �r�F�g�p���@�\��

void ShowUsage()
{
	cout << 
	"QR�R�[�h�V���{���̃r�b�g�}�b�v�C���[�W���쐬���܂��B\r\n"
	"\r\n"
	"MKQRBMP /O\"�o�̓t�@�C��\" /T\"�e�L�X�g�f�[�^\" [/I\"���̓f�[�^�t�@�C��\"] [/Sn] [/Ln] [/Vn] [/E(y/n)] [/Mn]\r\n"
	"\r\n"
	"  /O      �o�̓r�b�g�}�b�v�t�@�C�������w�肵�܂��B\r\n"
	"  /T      �G���R�[�h����e�L�X�g���w�肵�܂��B\r\n"
	"          �s�����������'%'�A�_�u���N�H�[�e�[�V������'%'�ɂ��\r\n"
	"          16�i�\�L���Ă��������B\r\n"
	"  /I      �e�L�X�g�w��̑���Ƀt�@�C�����w�肵�܂��B\r\n"
	"  /Sn     ���W���[���T�C�Y���w�肵�܂��Bn=1�`20�A�f�t�H���g=4\r\n"
	"  /Ln     ���������x�����w�肵�܂��Bn=0(L), 1(M). 2(Q), 3(H)�A�f�t�H���g=1\r\n"
	"  /Vn     �^�Ԃ��w�肵�܂��Bn=1�`40�A�ȗ����͎����I������܂��B\r\n"
	"  /E(y/n) �w��^�ԂɎ��܂�Ȃ������ꍇ�ɑ傫�Ȍ^�Ԃ������I�����邩�ǂ������w��\r\n"
	"          ���܂��B�f�t�H���g=y\r\n"
	"  /Mn     �}�X�L���O�p�^�[���ԍ����w�肵�܂��Bn=0�`7�A�ȗ����͎����I������܂��B\r\n"
	"  Copyright (C) 2006 Psytec Inc."
	<< endl;
}
