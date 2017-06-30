#ifndef __OXSX_PARAMETER_DICT__
#define __OXSX_PARAMETER_DICT__
#include <map>
#include <string>

// This could be changed to something more sophisticated in the future, so we should assume this thing keeps a map like
// interface, but you shouldn't rely on the particular implementation i.e. the ordering etc.

typedef std::map<std::string, double> ParameterDict;

#endif
