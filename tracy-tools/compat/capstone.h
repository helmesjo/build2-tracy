// Compatibility shim: Tracy sources include <capstone.h>, as upstream's build
// puts capstone's include/capstone/ directory on the include path.
// libcapstone exports <capstone/capstone.h>.
//
#pragma once

#include <capstone/capstone.h>
