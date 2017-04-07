#ifndef __OXSX_PIROR__
#define __OXSX_PIROR__
#include <vector>
#include <string>
#include <stddef.h>
#include <Function.h>
#include <Gaussian.h>


class Piror{
    public:
        Piror(){
            std::cout << "size of paralist = "<<parameterList.size() << std::endl;
        }
        Piror(std::vector<std::string> parameters, Function* func):
            parameterList(parameters), function(func){;}

        void SetParameterList(std::vector<std::string> dependences);
        void SetFunction(Function* func);

        double Probability(std::vector<double> parameters);

    private:
        std::vector<std::string> parameterList;
        Function* function;
};
#endif

