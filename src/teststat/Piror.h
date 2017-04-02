#ifndef __OXSX_PIROR__
#define __OXSX_PIROR__
#include <vector>
#include <string>
#include <stddef.h>
#include <Function.h>
#include <Gaussian.h>


class Piror{
 public:
     Piror(){;}
     Piror(std::vector<std::string> parameters, Function* func):
        parameterList(parameters), function(func){
        // parameterList=parameters;
        // independent= parameterList[0];
        // dependences=parameterList.erase(std::remove(parameterList.begin(), parameterList.end(),0), parameterList.end());
        // functions=func;
     }

     void SetParameterList(std::vector<std::string> dependences);
     void SetFunction(Function* func);
     // void SetFunction(Gaussian* func);
     double Probability(std::vector<double> parameters);

 private:
    std::vector<std::string> parameterList;
    Function* function;
};
#endif

