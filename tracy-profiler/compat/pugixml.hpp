// Compatibility shim: Tracy sources include <pugixml.hpp>, as upstream's
// build puts pugixml's src/ directory on the include path. pugixml exports
// <pugixml/pugixml.hpp>.
//
#pragma once

#include <pugixml/pugixml.hpp>
