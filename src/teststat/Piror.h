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
        ~Piror();
        Piror& operator=(const Piror&);
        Piror(std::vector<std::string> parameters, Function* func):
            parameterList(parameters), function(func){;}

        //Copy operator
        Piror(const Piror&);
        //Need to make a assignment copy.

        void SetParameterList(std::vector<std::string> dependences);
        void SetFunction(Function* func);
        Function* GetFunction();

        double Probability(std::vector<double> parameters);

    private:
        std::vector<std::string> parameterList;
        Function* function;
};
#endif

