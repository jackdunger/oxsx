#include <CompositeBinnedNLLH.h>
#include <assert.h>


double CompositeBinnedNLLH::Evaluate(){
  //If not initialised, do so
  if(!fInitialised)this->Initialise();
  double SumNLLH = 0;
  for(size_t i = 0; i < fComponentsNHHL.size(); i++){
    SumNLLH += fComponentsNHHL.at(i).Evaluate();
  }
  return SumNLLH;
}

void CompositeBinnedNLLH::SetParameters(const std::vector<double>& allfloatingparams){
  //If not initialised, do so
  if(!fInitialised)this->Initialise();
  int start = 0;
  for(size_t i = 0; i < fComponentsNHHL.size(); i++){
    int nparam = fComponentsNHHL.at(i).GetParameterCount();
    std::vector<double> pass_params;
    for(size_t j = 0; j< nparam; ++j){
      pass_params.push_back(allfloatingparams[fFitIndex[start+j]]);
    }
    fComponentsNHHL.at(i).SetParameters(pass_params);
    start += nparam;
  }
}

// NOTE : The next 2 methods are analagous so if changing logic in one, do it for both - parameters and their names
std::vector<double> CompositeBinnedNLLH::GetParameters() const{
// JW - Not sure where this is used - does it want the fit parameters back? I presume so.
  //If not initialised, this method will fail
  if(!fInitialised){
    std::cout << "CompositeBinnedNLLH:Error - trying to access parameters from unitialised object" << std::endl;
    // How do I return and error code?
  }
  //Get the parameters we passed to the separate NLLH methods back - trying not to store them here as well
  std::vector<double> theInternalparams;
  for(size_t i = 0; i < fComponentsNHHL.size(); i++){
    std::vector<double> pass_params = fComponentsNHHL.at(i).GetParameters();
    int nparam = fComponentsNHHL.at(i).GetParameterCount();
    for(size_t j = 0; j< nparam; ++j){
      theInternalparams.push_back(pass_params.at(i));
    }
  }
  // and now output the fitted parameters only, removing shared ones
  std::vector<double> theFittedparams((theInternalparams.size()-fSharedParams.size()),-999);
  for(int i = 0; i < theInternalparams.size(); ++i){
    theFittedparams[fFitIndex[i]]=theInternalparams[i];
  }
  
  return theFittedparams;
}

std::vector<std::string> CompositeBinnedNLLH::GetParameterNames() const{
  //If not initialised, method will fail
  if(!fInitialised){
    std::cout << "CompositeBinnedNLLH:Error - trying to access parameters from unitialised object" << std::endl;
    // How do I return and error code?
  }
  //Get the parameters we passed to the separate NLLH methods back - trying not to store them here as well
  std::vector<std::string> theInternalparams;
  for(size_t i = 0; i < fComponentsNHHL.size(); i++){
    std::vector<std::string> pass_params = fComponentsNHHL.at(i).GetParameterNames();
    int nparam = fComponentsNHHL.at(i).GetParameterCount();
    for(size_t j = 0; j< nparam; ++j){
      theInternalparams.push_back(pass_params.at(j));
    }
  }
  // and now output the fitted parameters only, removing shared ones
  std::vector<std::string> theFittedparamNames((theInternalparams.size()-fSharedParams.size()),"NULL");
  for(int i = 0; i < theInternalparams.size(); ++i){
    theFittedparamNames[fFitIndex[i]]=theInternalparams[i];
  }
  return theFittedparamNames;

}

int CompositeBinnedNLLH::GetParameterCount() const{
  //If not initialised, method will fail
  if(!fInitialised){
    std::cout << "CompositeBinnedNLLH:Error - trying to access parameters from unitialised object" << std::endl;
    return 0;
  }
  int nparam = 0;
  for(size_t i=0;i< fFitIndex.size(); ++i){
    // taking into account shared parameters, number of parameters to fit is the higher number mapped (+1 as mapping starts at 0)
    if((fFitIndex.at(i)+1)>nparam)nparam++;
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
  fInitialised = false;   
}

void CompositeBinnedNLLH::SetSharedParameter(const std::string& name){
  fSharedParams.push_back(name);
  fInitialised = false;
}

void CompositeBinnedNLLH::Initialise(){
  // if not already done so - make sure the fit components are all registered
  for(size_t i = 0; i < fComponentsNHHL.size(); i++){
    fComponentsNHHL.at(i).RegisterFitComponents();
  }
  // Firstly we create a vector of all the parameter names for the internal parameters
  std::vector<std::string> internal_pnames;
  for(size_t i = 0; i < fComponentsNHHL.size(); i++){
    std::vector<std::string> component_pnames= fComponentsNHHL.at(i).GetParameterNames();
    for(size_t j=0; j < component_pnames.size(); ++j){
      internal_pnames.push_back(component_pnames.at(j));
    }
  }
  // Now we look at the shared parameters and find the first instance
  std::vector<int> firstShared;
  for(size_t i = 0;i<fSharedParams.size(); ++i){
    for(size_t j=0; j < internal_pnames.size(); ++j){
      if(internal_pnames.at(j).compare(fSharedParams.at(i)) == 0){
        firstShared.push_back(j);
        break;
      }
    }
  }
  // check that worked
  std::cout << "CompositeBinnedNLLH: Found " << firstShared.size() << " shared parameters " << std::endl;
  assert(firstShared.size()==fSharedParams.size());
  // Now we do our mapping
  int count_unique_params = 0;
  for(size_t j=0; j < internal_pnames.size(); ++j){
    fFitIndex.push_back(count_unique_params); // default is to map to the next value
    for(size_t i = 0;i<fSharedParams.size(); ++i){
      // look to see if its a shared parameter, and if so, overwrite mapping to first instance
      if(internal_pnames.at(j).compare(fSharedParams.at(i)) == 0){
        fFitIndex[j]=firstShared.at(i);
        if(j != firstShared.at(i))count_unique_params--;  // If not the first instance don't increment the count.
      }
    }
    count_unique_params++;
  }
  fInitialised = true;
}

void
CompositeBinnedNLLH::PrintComponents(){

  // if not already done so - make sure the fit components are all registered
  for(size_t i = 0; i < fComponentsNHHL.size(); i++){
    fComponentsNHHL.at(i).RegisterFitComponents();
  }
  //Print things
  for(size_t i = 0; i < fComponentsNHHL.size(); i++){
    fComponentsNHHL.at(i).PrintParameters();
  }

}
