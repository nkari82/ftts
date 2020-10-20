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
#include <array>

#define b1(a) {(char)a}
#define b2(a, b) {(char)a, (char)b}
#define b3(a, b, c) {(char)a, (char)b, (char)c}
#define b4(a, b, c, d) {(char)a, (char)b, (char)c, (char)d}
#define b5(a, b, c, d, e) {(char)a, (char)b, (char)c, (char)d, (char)e}
#define b6(a, b, c, d, e, f) {(char)a, (char)b, (char)c, (char)d, (char)e, (char)f}
#define b7(a, b, c, d, e, f, g) {(char)a, (char)b, (char)c, (char)d, (char)e, (char)f, (char)g}
#define b8(a, b, c, d, e, f, g, h) {(char)a, (char)b, (char)c, (char)d, (char)e, (char)f, (char)g, (char)h}
#define b9(a, b, c, d, e, f, g, h, i) {(char)a, (char)b, (char)c, (char)d, (char)e, (char)f, (char)g, (char)h, (char)i}
#define b10(a, b, c, d, e, f, g, h, i, j) {(char)a, (char)b, (char)c, (char)d, (char)e, (char)f, (char)g, (char)h, (char)i, (char)j}
#define b11(a, b, c, d, e, f, g, h, i, j, k) {(char)a, (char)b, (char)c, (char)d, (char)e, (char)f, (char)g, (char)h, (char)i, (char)j, (char)k}
#define b12(a, b, c, d, e, f, g, h, i, j, k, l) {(char)a, (char)b, (char)c, (char)d, (char)e, (char)f, (char)g, (char)h, (char)i, (char)j, (char)k, (char)l}
#define b13(a, b, c, d, e, f, g, h, i, j, k, l, m) {(char)a, (char)b, (char)c, (char)d, (char)e, (char)f, (char)g, (char)h, (char)i, (char)j, (char)k, (char)l, (char)m}
#define b14(a, b, c, d, e, f, g, h, i, j, k, l, m, n) {(char)a, (char)b, (char)c, (char)d, (char)e, (char)f, (char)g, (char)h, (char)i, (char)j, (char)k, (char)l, (char)m, (char)n}
#define b15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) {(char)a, (char)b, (char)c, (char)d, (char)e, (char)f, (char)g, (char)h, (char)i, (char)j, (char)k, (char)l, (char)m, (char)n, (char)o}
#define b16(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) {(char)a, (char)b, (char)c, (char)d, (char)e, (char)f, (char)g, (char)h, (char)i, (char)j, (char)k, (char)l, (char)m, (char)n, (char)o, (char)p}
#define b17(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) {(char)a, (char)b, (char)c, (char)d, (char)e, (char)f, (char)g, (char)h, (char)i, (char)j, (char)k, (char)l, (char)m, (char)n, (char)o, (char)p, (char)q}
#define b18(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, s) {(char)a, (char)b, (char)c, (char)d, (char)e, (char)f, (char)g, (char)h, (char)i, (char)j, (char)k, (char)l, (char)m, (char)n, (char)o, (char)p, (char)q, (char)s}
#define b19(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, s, t) {(char)a, (char)b, (char)c, (char)d, (char)e, (char)f, (char)g, (char)h, (char)i, (char)j, (char)k, (char)l, (char)m, (char)n, (char)o, (char)p, (char)q, (char)s, (char)t}
#define b20(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, s, t, u) {(char)a, (char)b, (char)c, (char)d, (char)e, (char)f, (char)g, (char)h, (char)i, (char)j, (char)k, (char)l, (char)m, (char)n, (char)o, (char)p, (char)q, (char)s, (char)t, (char)u}


//template<size_t N>
//constexpr std::array<char,N> Convert(const std::array<int, N>& utf8)
//{
//	std::array<char, N> ret;
//	for (int i = 0; i < N; ++i)
//		ret[i] = (char)utf8[i];
//	return ret;
//}

#include "ftts.h"
#include "Processor.h"
#include "JSProcessor.h"
#include "LJSProcessor.h"