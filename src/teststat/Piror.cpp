#include <Piror.h>
#include <iostream>
#include <Exceptions.h>

Piror::Piror(const Piror& other_){
    //What do you need to copy over?
    parameterList= other_.parameterList;
    function = dynamic_cast<Function*>(other_.function->Clone());
}

Piror& Piror::operator=(const Piror& other_)
    {
        // Piror* newPiror = new Piror(*this);
        // return *newPiror;
        // This line returns the a Piror which is copied from "this" using the copy operator.
        return *(new Piror(*this));
    }

Piror::~Piror(){
    delete function;
}

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

Function*
Piror::GetFunction(){
    return function;
}

double
Piror::Probability(std::vector<double> parameters){

    if (parameters.size()==parameterList.size())
        throw std::runtime_error("Number of parameters provided not equal to the number expected.");
    // if (!independent.empty())
    //     throw std::runtime_error("Independent parameter not set.");

    return (*function)(parameters);

}
