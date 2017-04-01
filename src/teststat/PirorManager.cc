#include <PirorManager.h>
#include <iostream>


void
PirorManager::AddPiror(std::vector<std::string> dependences, Function * func){


    listOfConstraints.push_bash(dependences);
    listOfFunctions.push_bash(func);
}

double
PirorManager::Probability(std::vector<double> parameters){

    std::cout << "in" << std::endl;

}
