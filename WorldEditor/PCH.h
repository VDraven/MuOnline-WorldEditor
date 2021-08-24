#ifndef PCH_H
#define PCH_H

#define _CRT_SECURE_NO_WARNINGS

#include "VS2008_string.h"
#include "VS2008_map.h"

#include <Windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <gl\GL.h>
#include "Util.h"
#include "ZzzTexture.h"

typedef __int64 int64;
typedef int int32;
typedef short int16;
typedef char int8;
typedef unsigned __int64 uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

#endif //PCH_H
