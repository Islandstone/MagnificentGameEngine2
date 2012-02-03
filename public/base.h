#ifndef _HEADER_BASE_INCLUDE
#define _HEADER_BASE_INCLUDE

#include <string>

typedef std::string String;

// Hack
//std::string wtoa(const std::wstring& wstr);

#define ARRAYSIZE(array) ( sizeof(array) / sizeof(array[0]) )

#endif
