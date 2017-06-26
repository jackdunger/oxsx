#ifndef __OXSX_GAUSSIAN_FITTER__
#define __OXSX_GAUSSIAN_FITTER__
#include <vector>      
#include <string>
#include <ParameterDict.h>
#include <set>
#include <iostream>

class Gaussian;

class GaussianFitter{
public:

    GaussianFitter(Gaussian* gaus){
        fOrignalFunc = gaus; 
    }

    GaussianFitter(const GaussianFitter& obj){
        fOrignalFunc = obj.fOrignalFunc;
        fMeans = obj.fMeans;
        fStdDevs = obj.fStdDevs;
    }

    GaussianFitter operator=(const GaussianFitter& other_)
    {
        return GaussianFitter(*this);
    }

    void init(){
        SetMeanNames("means");
        SetStdDevNames("stddevs");
    }

    void   SetParameter(const std::string& name_, double value);
    double GetParameter(const std::string& name_) const;

    void   SetParameters(const ParameterDict&);
    ParameterDict GetParameters() const;
    size_t GetParameterCount() const;

    std::set<std::string> GetParameterNames() const;
    void   RenameParameter(const std::string& old_, const std::string& new_);

private:
    Gaussian* fOrignalFunc;
    std::vector<std::string> fMeans;
    std::vector<std::string> fStdDevs;
};
#endif
