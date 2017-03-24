// PointTest.h: PointTest クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINTTEST_H__AEF1C528_BA6F_4412_989F_5C055366FC90__INCLUDED_)
#define AFX_POINTTEST_H__AEF1C528_BA6F_4412_989F_5C055366FC90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PointTest  
{
public:
	PointTest();
	virtual ~PointTest();
	void (*FunPoint)(int);
	int print();

private:
	int h;
	void myFun(int a);

};
#endif // !defined(AFX_POINTTEST_H__AEF1C528_BA6F_4412_989F_5C055366FC90__INCLUDED_)
