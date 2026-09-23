// Compatibility shim: Tracy sources include "ini.h" (a copy of rxi/ini
// 0.1.1 with C linkage declarations added for C++). libini exports
// <ini/ini.h> with the same API but without C linkage declarations.
//
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <ini/ini.h>

#ifdef __cplusplus
}
#endif
