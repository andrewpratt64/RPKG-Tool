// Andrew Pratt 2021
#pragma once


// Utility for using either a narrow or wide string, depending on whether the program uses unicode or not
#ifdef UNICODE
#define StrMaybeUnicode(str) L##str
#else
#define StrMaybeUnicode(str) str
#endif