#ifndef __OXSX_HIST_TOOLS__
#define __OXSX_HIST_TOOLS__
#include <vector>
#include <string>
#include <stddef.h>
#include <Function.h>


class PirorManager{
 public:
     PirorManager(){;}
     void AddPiror(std::vector<std::string> dependences, Function * func);
     double Probability(std::vector<double> parameters);

 private:
    std::vector< std::vector<std::string> > dependences;
    std::vector<Function*> functions;
};
#endif

