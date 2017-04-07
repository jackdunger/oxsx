#include <Piror.h>
#include <math.h>
#include <PirorManager.h>
#include <iostream>

PirorManager::PirorManager(const PirorManager& other_ ){
    //What do you need to copy over?
    // function = dynamic_cast<function*>(other_.function->Clone());
    // pirorList = dynamic_cast< std::vector<double> > ( other_.GetPirors() );
    pirorList =  other_.pirorList;
}

PirorManager PirorManager::operator=(const PirorManager& other_)
    {
        return PirorManager(*this);
    }

void
PirorManager::AddPiror(Piror piror){

    pirorList.push_back(piror);
}

std::vector<Piror>
PirorManager::GetPirors(){
    return pirorList;
}

double PirorManager::GetProbabilities( const std::map<std::string, double>& params_){
    double result=0;
    for (std::vector<Piror>::iterator iter = pirorList.begin(); iter != pirorList.end(); ++iter ) {
        std::vector<double> parameterHolder;
        std::vector<std::string> parameterList = (*iter).GetParameterList();

        for (std::vector<std::string>::iterator i = parameterList.begin(); i != parameterList.end(); ++i) {
            parameterHolder.push_back(params_.at((*i))); 
        }
        //How do i know what params to feed it?

        result += (*iter).Probability(parameterHolder);
    }
    return result;

}


double PirorManager::GetLogProbabilities( const std::map<std::string, double>& params_){
    double result=0;
    for (std::vector<Piror>::iterator iter = pirorList.begin(); iter != pirorList.end(); ++iter ) {
        std::vector<double> parameterHolder;
        std::vector<std::string> parameterList = (*iter).GetParameterList();

        for (std::vector<std::string>::iterator i = parameterList.begin(); i != parameterList.end(); ++i) {
            parameterHolder.push_back(params_.at((*i))); 
        }
        //How do i know what params to feed it?

        result += log((*iter).Probability(parameterHolder));
    }
    return result;

}
