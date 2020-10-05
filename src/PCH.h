#pragma once
#define LIBICONV_STATIC
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

#define b1(a) {(char)a}
#define b2(a, b) {(char)a, (char)b}
#define b3(a, b, c) {(char)a, (char)b, (char)c}

#include "ftts.h"
#include "Processor.h"
#include "JSProcessor.h"
#include "LJSProcessor.h"