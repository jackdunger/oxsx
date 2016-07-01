#include <CompositeBinnedNLLH.h>


double CompositeBinnedNLLH::Evaluate(){
  double SumNLLH = 0;
  for(size_t i = 0; i < fComponentsNHHL.size(); i++){
    SumNLLH += fComponentsNHHL.at(i).Evaluate();
  }
  return SumNLLH;
}

void CompositeBinnedNLLH::SetParameters(const std::vector<double>& allparams){
  int start = 0;
  for(size_t i = 0; i < fComponentsNHHL.size(); i++){
    int nparam = fComponentsNHHL.at(i).GetParameterCount();
    std::vector<double> pass_params(&allparams[start],&allparams[start+nparam]);
    fComponentsNHHL.at(i).SetParameters(pass_params);
    start += nparam;
  }
}

std::vector<double> CompositeBinnedNLLH::GetParameters() const{
  std::vector<double> theparams;
  for(size_t i = 0; i < fComponentsNHHL.size(); i++){
    std::vector<double> pass_params = fComponentsNHHL.at(i).GetParameters();
    for(size_t j = 0; j < pass_params.size(); ++j){
      theparams.push_back(pass_params.at(j));
    }
  }
  return theparams;
}

int CompositeBinnedNLLH::GetParameterCount() const{
  int nparam = 0;
  for(size_t i = 0; i < fComponentsNHHL.size(); i++){
    nparam += fComponentsNHHL.at(i).GetParameterCount();
  }
  return nparam;
}

void CompositeBinnedNLLH::RegisterFitComponents(){
  for(size_t i = 0; i < fComponentsNHHL.size(); i++){
    fComponentsNHHL.at(i).RegisterFitComponents();
  }
}

int CompositeBinnedNLLH::GetComponentCount() const{
  return fComponentsNHHL.size();
}

void CompositeBinnedNLLH::AddBinnedNLLH(const BinnedNLLH& theNLLH){
  fComponentsNHHL.push_back(theNLLH);
}