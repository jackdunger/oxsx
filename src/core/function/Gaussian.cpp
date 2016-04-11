#include <Gaussian.h>
#include <math.h>
#include <PdfExceptions.h>
#include <ContainerParameter.h>
#include <gsl/gsl_cdf.h>
#include <sstream>

/////////////////////////
// Constructory Things //
/////////////////////////

void
Gaussian::Initialise(const std::vector<double>& means_, const std::vector<double>& stdDevs_){
    SetMeansStdDevs(means_, stdDevs_);
    SetNDims(means_.size());
    fMeans = means_;
    fStdDevs = stdDevs_;
    fCdfCutOff = 6; // default val
}

Gaussian::Gaussian(const std::vector<double>& means_, const std::vector<double>& stdDevs_){
    Initialise(means_, stdDevs_);
}

Gaussian::Gaussian(size_t nDims_){
    Initialise(std::vector<double>(nDims_, 0), std::vector<double>(nDims_, 1));
}

Gaussian::Gaussian(double mean_, double stdDev_){
    Initialise(std::vector<double>(1, mean_), std::vector<double>(1, stdDev_));
}

Function* 
Gaussian::Clone() const{
    return static_cast<Function*> (new Gaussian(*this));
}

/////////////////////
// Getters/Setters //
/////////////////////

double 
Gaussian::GetMean(size_t dimension_) const{
    try{
        return fMeans.at(dimension_);
    }
    catch(const std::out_of_range& e_){
        throw DimensionError("Requested Gaussian mean beyond function dimensionality!");
    }
}

double 
Gaussian::GetStDev(size_t dimension_) const{
    try{
        return fStdDevs.at(dimension_);
    }
    catch(const std::out_of_range& e_){
        throw DimensionError("Requested Gaussian stdDev beyond function dimensionality!");
    }
}

std::vector<double>
Gaussian::GetMeans() const {
    return fMeans;
}

void
Gaussian::SetMeansStdDevs(const std::vector<double>& means_, 
                           const std::vector<double>& stdDevs_){
    if (means_.size() != stdDevs_.size())
        throw DimensionError("Tried to set Gaussian function with #means != #stdDevs!");

    for(size_t i = 0; i < stdDevs_.size(); i++)
        if(stdDevs_.at(i) <= 0)
            throw ParameterError("Gaussian standard deviation must be greater than 0!");

    fMeans = means_;
    fStdDevs = stdDevs_;
    SetNDims(means_.size());
}

std::vector<double>
Gaussian::GetStdDevs() const {
    return fStdDevs;
}

double
Gaussian::GetCdfCutOff() const{
    return fCdfCutOff;
}

void 
Gaussian::SetCdfCutOff(double cutOff_){
    fCdfCutOff = cutOff_;
}


/////////////////
// Probability //
/////////////////

double 
Gaussian::operator() (const std::vector<double>& vals_) const{
    if (vals_.size() != GetNDims())
        throw DimensionError("Gaussian dimensionality does not match the input vector to evaluate!");

    double exponent = 0;
    double mean;
    double stdDev;
    double nDevs;
    for(size_t i = 0; i < GetNDims(); i++){
        mean  = fMeans.at(i);
        stdDev = fStdDevs.at(i);

        nDevs = (vals_.at(i) - mean)/stdDev;
        exponent += nDevs * nDevs;
    }

    double norm = 1;
    for(size_t i = 0; i < GetNDims(); i++)
        norm *= sqrt(2*M_PI) * fStdDevs.at(i);

    return exp(- 0.5 * exponent) / norm; 
}

double 
Gaussian::Cdf(size_t dim_, double val_) const{
    double nDevs = (val_ - GetMean(dim_))/GetStDev(dim_);
    if (nDevs > fCdfCutOff)
        return 1;
    if(nDevs < -1 * fCdfCutOff)
        return 0;

    return gsl_cdf_gaussian_P(val_ - GetMean(dim_), GetStDev(dim_));
}

double 
Gaussian::Integral(const std::vector<double>& mins_, const std::vector<double>& maxs_) const{
    if(mins_.size() != GetNDims() || maxs_.size() != GetNDims())
        throw DimensionError("Gaussian, tried to integrate over interval of wrong dimensionality");

    double integral = 1;
    for(size_t i = 0; i < mins_.size(); i++)
        integral *= ( Cdf(i, maxs_[i]) - Cdf(i, mins_[i]));
  
    return integral;  
}


////////////////////////
// Make it fittable   //
////////////////////////

void
Gaussian::MakeFittable(){
    std::stringstream ss1;
    std::stringstream ss2;

    for(size_t i = 0; i < GetNDims(); i++){
        ss1 << "Gaussian Mean " << i;
        ss2 << "Gaussian Std. Dev. " << i;
        AddAsParameter(new VectorParameter(fMeans, i),  ss1.str());
        AddAsParameter(new VectorParameter(fStdDevs, i), ss2.str());
        ss1.str("");
        ss2.str("");
    }
}