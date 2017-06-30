#include <ComponentManager.h>
#include <ContainerTools.hpp>
#include <Exceptions.h>
#include <algorithm>
#include <iostream>

using ContainerTools::GetKeys;
using ContainerTools::ToString;

void 
ComponentManager::AddComponent(FitComponent*  componentPtr_){
    fComponents.push_back(componentPtr_);
    fComponentCount++;
}

void 
ComponentManager::SetParameters(const ParameterDict& params_){
    if(!params_.size())
        return;

    // let them each take what they need, they'll complain if something is missing
    for(size_t i = 0; i < fComponents.size(); i++)
        fComponents[i]->SetParameters(params_);
}

ParameterDict
ComponentManager::GetParameters() const{
    ParameterDict pd;
    for(size_t i = 0; i < fComponents.size(); i++){
        ParameterDict compPD = fComponents.at(i)->GetParameters();
        pd.insert(compPD.begin(), compPD.end());
    }
    return pd;
}

std::set<std::string>
ComponentManager::GetParameterNames() const{
    return GetKeys(GetParameters());
}

int
ComponentManager::GetTotalParameterCount() const{
    return GetParameters().size();
}

void
ComponentManager::Clear(){
    fComponentCount  = 0;
    fComponents.clear();
}

size_t
ComponentManager::FindParameter(const std::string& paramName_) const{
    for(size_t i = 0; i < fComponents.size(); i++){
        FitComponent* component = fComponents[i];
        std::vector<std::string> names = component->GetParameterNames();

        std::vector<std::string>::iterator it = std::find(names.begin(), names.end(), 
                                                          paramName_);
        if(it != names.end())
            return (it - names.begin());
    }
    throw NotFoundError(Formatter() << "ComponentManager::GetParameter "
                        << " requested non-existent parameter "
                        << paramName_
                        );
}

double
ComponentManager::GetParameter(const std::string& paramName_) const{
    ParameterDict dict = GetParameters();
    try{
        return dict.at(paramName_);
    }
    catch(const std::out_of_range& e_){
        throw ParameterError("No fit component has parameter " + paramName_ + ", available names are: \n" + ToString(GetKeys(dict)));
    }
}

size_t
ComponentManager::GetComponentCount() const{
    return fComponentCount;
}
