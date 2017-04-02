#include <Piror.h>
#include <iostream>
#include <Exceptions.h>


void 
Piror::SetParameterList(std::vector<std::string> parameters){
    parameterList = parameters;
    // independent= parameterList[0];
    // dependences=parameterList.erase(std::remove(parameterList.begin(), parameterList.end(),0), parameterList.end());
}

void
Piror::SetFunction(Function* func){
    function=func;
}

// void
// Piror::SetFunction(Gaussian* func){
//     function=func;
// }

double
Piror::Probability(std::vector<double> parameters){

    if (parameters.size()==parameterList.size())
        throw std::runtime_error("Number of parameters provided not equal to the number expected.");
    // if (!independent.empty())
    //     throw std::runtime_error("Independent parameter not set.");

    return (*function)(parameters);

}
