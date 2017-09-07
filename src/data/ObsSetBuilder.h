#ifndef __OXSX__OBSSETBUILDER__
#define __OXSX__OBSSETBUILDER__
#include <string>
#include <vector>

class ObsSet;
class ObsSetBuilder{
 public:
    typedef std::vector<std::string> StrVec;
    static ObsSet Build(const StrVec& setNames_, const StrVec& allNames_);

};
#endif
