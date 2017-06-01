#include <SystematicManager.h>

const std::vector<Systematic*>& 
SystematicManager::GetSystematics() const{
    return fSystematics;
}

void
SystematicManager::Construct(){
    // Dont do anything if there are no systematics
    if(!fSystematics.size())
        return;

    // construct the response matricies
    for(size_t i = 0; i < fSystematics.size(); i++)
        fSystematics[i] -> Construct();

    
    // Assemble the overall response
    fTotalResponse = fSystematics.at(0) -> GetResponse();
    for(size_t i = 1; i < fSystematics.size(); i++)
      fTotalResponse *= fSystematics.at(i) -> GetResponse();
}

const SparseMatrix&
SystematicManager::GetTotalResponse() const{
     return fTotalResponse;
}
                                        
void 
SystematicManager::Add(Systematic* systematic_){    
    fSystematics.push_back(systematic_);
    fNSystematics++;
}
void 
SystematicManager::Add(Systematic* systematic_){    
    fSystematics.push_back(systematic_);
    fNSystematics++;
}

size_t
SystematicManager::GetNSystematics() const{
    return fNSystematics;
}

void
AddSystmatic(Systematic*, std::string& groupName_){


}

void 
AddPdf(std::string& groupName_, BinnedED& ED_){

}

void
DistortEDs(std::vector<BinnedED>& workingEDs_){


}
