#ifndef LangExt_hpp
#define LangExt_hpp

#include <string.h>
#include <limits.h>

#define using(decl) do { decl; try
#define end_using(name) catch (...) { delete name; throw; } } while (false)
#define end_using_array(name) catch (...) { delete [] name; throw; } } while (false)

#endif
