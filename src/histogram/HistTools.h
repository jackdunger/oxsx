#ifndef __OXSX_HIST_TOOLS__
#define __OXSX_HIST_TOOLS__
#include <vector>
#include <map>
#include <string>
#include <set>


class AxisCollection;
class Histogram;

class HistTools{
 public:
    static std::vector<Histogram> MakeAllHists(const AxisCollection&, 
                                               const std::set<std::pair<std::string, std::string> >& combinations_);

    template<typename T1>
    static std::vector<Histogram> MakeAllHists(const AxisCollection&, 
                                               const T1& names_);

    static void FillAllHists(std::vector<Histogram>&, const std::map<std::string, double>& fillvals);
};

#include <HistTools.hpp>
#endif
