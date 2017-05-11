#include <Minuit.h>
#include <Minuit2/MnMigrad.h>
#include <Minuit2/MnMinimize.h>
#include <Minuit2/MnSimplex.h>
#include <Minuit2/FunctionMinimum.h>
#include <Minuit2/MnUserParameters.h>
#include <FitResult.h>
#include <Exceptions.h>
#include <Formatter.hpp>

using ROOT::Minuit2::MnMigrad;
using ROOT::Minuit2::MnMinimize;
using ROOT::Minuit2::MnSimplex;
using ROOT::Minuit2::MnUserParameters;

// FIXME:: CHeck the length of the vectors as you set them match fNparams

Minuit::~Minuit(){
    delete fMinimiser;
}

void
Minuit::SetMethod(const std::string& method_){
    fMethod = method_;   
}

std::string
Minuit::GetMethod() const{
    return fMethod;
}

void
Minuit::SetInitialErrors(const std::vector<double>& errs_){
    fInitialErrors = errs_;
}

void
Minuit::SetInitialValues(const std::vector<double>& vals_){
    fInitialValues = vals_;
}

void
Minuit::SetUpperContourEdge(double val_){
    fMinuitFCN.SetUp(val_);
}

double
Minuit::GetUpperContourEdge() const{
    return fMinuitFCN.GetUp();
}

void
Minuit::SetMaxima(const std::vector<double>& maxima_) {fMaxima = maxima_;}

std::vector<double> 
Minuit::GetMaxima() const {return fMaxima;}

void
Minuit::SetMinima(const std::vector<double>& minima_) {fMinima = minima_;}

std::vector<double> 
Minuit::GetMinima() const {return fMinima;}

void 
Minuit::Initialise(){
    delete fMinimiser;
    fMinimiser = NULL;

    // max or min?
    if(fMaximising)
        fMinuitFCN.SetSignFlip(true);   

    // Create parameters and set limits
    MnUserParameters params(fInitialValues, fInitialErrors);
    //Should these be the same size?
    if(fMinima.size() && fMaxima.size())
        for(size_t i = 0; i < fInitialValues.size(); i++)
            params.SetLimits(i, fMinima.at(i), fMaxima.at(i));
        
    if("Migrad" == fMethod)
        fMinimiser = new MnMigrad(fMinuitFCN, params);

    else if ("Minimize" == fMethod)
        fMinimiser = new MnMinimize(fMinuitFCN, params);
    
    else if ("Simplex" == fMethod)
        fMinimiser = new MnSimplex(fMinuitFCN, params);
    else 
        throw ValueError(Formatter() << "Minuit::Unrecognised method - "
                         << fMethod
                         );
}

void
Minuit::Fix(size_t index_){
    fFixedParameters.insert(index_);
}

void
Minuit::Release(size_t index_){
  fFixedParameters.erase(index_);
}

void
Minuit::SetMaxCalls(unsigned max_) {
  fMaxCalls = max_;
}

unsigned
Minuit::GetMaxCalls() const {
  return fMaxCalls;
}

const FitResult&
Minuit::Optimise(TestStatistic* testStat_){
    testStat_ -> RegisterFitComponents();
    
    size_t nParams = testStat_ -> GetParameterCount();
    if(   fInitialValues.size() != nParams
       || fInitialErrors.size() != nParams
       || fMinima.size() != nParams
       || fMaxima.size() != nParams
       )
        throw LogicError(Formatter() 
                         << "Minuit initialisation error - Got "
                         << fMinima.size() << " Minima, "
                         << fMaxima.size() << " Maxima, "
                         << fInitialValues.size() << " Initial Values and "
                         << fInitialErrors.size() << " Initial Errors"
                         << " - Need one per fit parameter (" << nParams
                         << ")"
                         );
                         
    fMinuitFCN = MinuitFCN(testStat_);
    Initialise();
    
    std::set<size_t>::iterator it = fFixedParameters.begin();
    for(; it != fFixedParameters.end(); ++it)
      fMinimiser -> Fix(*it);

    // defaults are same as ROOT defaults
    ROOT::Minuit2::FunctionMinimum fnMin  = fMinimiser -> operator()(fMaxCalls, fTolerance); 

    fFitResult.SetBestFit(fMinimiser -> Params());
    fFitResult.SetParameterNames(testStat_->GetParameterNames());
    fFitResult.SetValid(fnMin.IsValid());
    return fFitResult;
}

void 
Minuit::SetTolerance(double tol_) {
  fTolerance = tol_;
}

double
Minuit::GetTolerance() const {
  return fTolerance;
}

FitResult
Minuit::GetFitResult() const{
    return fFitResult;
    
}
