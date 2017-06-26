#include <GaussianFitter.h>
#include <Gaussian.h>
#include <iostream>
#include <sstream>
#include <Formatter.hpp>
#include <ContainerTools.hpp>
#include <Exceptions.h>
#include <algorithm>

using ContainerTools::ToString;

GaussianFitter::GaussianFitter(Gaussian* gaus, const size_t& nDims_){
    fOrignalFunc = gaus; 
    std::stringstream ss;
    for (int i = 0; i <nDims_; ++i) {
        ss << "means" << "_" << i;
        fMeansNames.push_back(ss.str());
        ss.str("");
        ss << "stddevs" << "_" << i;
        fStdDevsNames.push_back(ss.str());
        ss.str("");
    }
}

GaussianFitter::GaussianFitter(Gaussian* gaus, const std::set<std::string>& parametersNames_){
    fOrignalFunc = gaus; 
    std::stringstream ss;
    std::vector<std::string> nameHolder(parametersNames_.size());
    std::copy(parametersNames_.begin(), parametersNames_.end(), nameHolder.begin()); 
    for (int i = 0; i <nameHolder.size()/2; ++i)
        fMeansNames.push_back(nameHolder.at(i));
    for (int i = nameHolder.size()/2; i <nameHolder.size(); ++i)
        fStdDevsNames.push_back(nameHolder.at(i));
}

void
GaussianFitter::RenameParameter(const std::string& old_, const std::string& new_){
    std::vector<std::string>::iterator it;
    it=find(fMeansNames.begin(),fMeansNames.end(),old_);

    if(it==fMeansNames.end()){
        it=find(fStdDevsNames.begin(),fStdDevsNames.end(), old_);
        if(it==fStdDevsNames.end())
            throw NotFoundError(Formatter()<<"GaussianFitter:: When attempting to renaming the parameter "<< old_<<", it wasn't found. Available names: "<<
                    ToString(GetParameterNames()) );
        *it=new_;
        return;
    }
    *it=new_;
}

void
GaussianFitter::SetParameter(const std::string& name_, double value_){
    std::vector<std::string>::iterator it;

    it=find(fMeansNames.begin(),fMeansNames.end(), name_);

    if(it==fMeansNames.end()){

        it=find(fStdDevsNames.begin(),fStdDevsNames.end(), name_);

        if(it==fStdDevsNames.end())
            return;

        fOrignalFunc->SetStDev(it-fStdDevsNames.begin(),value_);
        return;
    }
    fOrignalFunc->SetMean(it-fMeansNames.begin(),value_);
}

double
GaussianFitter::GetParameter(const std::string& name_) const{
    std::vector<std::string>::const_iterator it;

    it=find(fMeansNames.begin(),fMeansNames.end(), name_);

    if(it==fMeansNames.end()){
        it=find(fStdDevsNames.begin(),fStdDevsNames.end(), name_);

        if(it==fStdDevsNames.end())
            throw NotFoundError(Formatter()<<"GaussianFitter:: Parameter : "<<
                                name_<<
                                " was not known to the GaussianFitter. Available names: "<<
                                ToString(GetParameterNames()) );

        return fOrignalFunc->GetStDev(it-fStdDevsNames.end());
    }
    return fOrignalFunc->GetMean(it-fStdDevsNames.end());
}

void
GaussianFitter::SetParameters(const ParameterDict& ps_){
    for (ParameterDict::const_iterator i = ps_.begin(); i != ps_.end(); ++i) {
        SetParameter(i->first,i->second);
    }
}

ParameterDict
GaussianFitter::GetParameters() const{
    std::vector<double> means = fOrignalFunc->GetMeans();
    std::vector<double> stddevs= fOrignalFunc->GetStdDevs();
    std::vector<double> values;

    values.reserve( means.size() + stddevs.size() ); // preallocate memory
    values.insert( values.end(), means.begin(), means.end() );
    values.insert( values.end(), stddevs.begin(), stddevs.end() );

    std::vector<std::string> names;
    names.reserve( fMeansNames.size() + fStdDevsNames.size() ); // preallocate memory
    names.insert( names.end(), fMeansNames.begin(), fMeansNames.end() );
    names.insert( names.end(), fStdDevsNames.begin(), fStdDevsNames.end() );

    return ContainerTools::CreateMap(names,values);
}

size_t
GaussianFitter::GetParameterCount() const{
    return fMeansNames.size()+fStdDevsNames.size();
}

std::set<std::string>
GaussianFitter::GetParameterNames() const{
    //BL : The order of the returned set is required to be means+stddevs by
    //the Gaussian copy constructor. 
    std::set<std::string> names;
    for (int i = 0; i < fMeansNames.size(); ++i)
        names.insert(fMeansNames.at(i));
    for (int i = 0; i < fStdDevsNames.size(); ++i) 
        names.insert(fStdDevsNames.at(i));
    return names;
}

