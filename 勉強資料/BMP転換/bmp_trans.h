//
// Author  : HYRON Yang
// Created : 2017/03/14
//
#ifndef _omronsoft_bmp_trans_h_
#define _omronsoft_bmp_trans_h_

#include <stdio.h>
#include <windows.h>
#include <string>
#include <fstream>
#include <iomanip>
#include "stdlib.h"
#include "math.h"
#include "../../include/omronsoft/log.h"

using namespace std;
using namespace omronsoft;

namespace bmptrans {

  bool MakeYokoBmp(const char* fromFileName, const char* toFileName, int rotateAngle = -90);

}

#endif  // _omronsoft_bmp_trans_h_

