#pragma once
#define ICONV_STATIC
#define MECAB_STATIC

#ifndef UTF8PROC_STATIC
#define UTF8PROC_STATIC
#endif

#include <unordered_map>
#include <cassert>
#include <iconv.h>
#include <utf8proc.h>
#include <mecab.h> // jpn
#include <string>
#include <cstring>
#include <list>
#include <algorithm>
#include <iostream>
#include <cctype>
#include <sstream>
#include <array>
#include <regex>

#include "ftts.h"
#include "Processor.h"
#include "JSProcessor.h"
#include "LJSProcessor.h"