// Dll2.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Dll2.h"
#include <iostream>
#include <fstream>
#include <sstream> 
using namespace std;
int calc(int a, int b)
{
	return a + b;
}
ofstream logg;
char * testStr()
{
	logg.open("E:\\log.txt", ofstream::app);
	logg << "测试" << endl;
	char *p;
	char result[] = "abc";
	p = result;
	logg << "p=" << p << endl;
	//strcpy_s(result,3, "abc");
	logg << p << endl;
	logg.close();
	return p;
}

