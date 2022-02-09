#pragma once

#include <string>
#include <filesystem>

#ifndef TO_STRING
#define TO_STRING(var) std::string(#var)
#endif

#ifndef TO_STRING_SINGLE_QUOTES
#define TO_STRING_SINGLE_QUOTES(var) std::string("'") + TO_STRING(var) + std::string("'")
#endif

#ifndef TO_STRING_DOUBLE_QUOTES
#define TO_STRING_DOUBLE_QUOTES(var) std::string(1, '"') + TO_STRING(var) + std::string(1, '"')
#endif

#ifndef FUNCTION_INFO
#define FUNCTION_INFO() std::string(__FUNCTION__)
#endif

#ifndef FILE_INFO
#define FILE_INFO() std::filesystem::path(__FILE__).make_preferred().string()
#endif

#ifndef LINE_INFO
#define LINE_INFO() std::to_string(__LINE__)
#endif

#ifndef CALL_INFO
#define CALL_INFO() FUNCTION_INFO() + "(" + FILE_INFO() + ":" + LINE_INFO() + ")"
#endif
