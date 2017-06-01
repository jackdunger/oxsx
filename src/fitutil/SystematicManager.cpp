#include <SystematicManager.h>

const std::vector<Systematic*>& 
SystematicManager::GetSystematics() const{
    return fSystematics;
}

void
SystematicManager::Construct(){
    // Don't do anything if there are no systematics
    if(!fSystematics.size())
        return;

    // // construct the response matricies
    // for(size_t i = 0; i < fSystematics.size(); i++)
    //     fSystematics[i] -> Construct();

    // Construct the response matrices
    // Over all systematics in each group
    // for (std::map<std::string,std::vector<Systematics*> >::const_iterator GroupName =groups.begin(); groupName != groups.end(); ++GroupName ) {
    //     for(size_t i = 0; i < GroupName->second.size(); i++)
    //         groups[GroupName->first].at(i) -> Construct();
    // }
            

    // Here you need to make a response for each group.
    // everything *= whatever;

    //Get the "all" response.

    // SparseMatrix all = groups["all"].at(0) -> GetResponse();
    // for(size_t i = 1; i < groups["all"].size(); i++)
    //   all *= groups["all"].at(i) -> GetResponse();
    //
    // allResponses["all"]=all;
     
    for (std::map<std::string,std::vector<Systematic*> >::const_iterator GroupName =groups.begin(); GroupName != groups.end(); ++GroupName ) {

        // Construct the response matrices
        // Over all systematics in each group
        for(size_t i = 0; i < GroupName->second.size(); i++)
            groups[GroupName->first].at(i) -> Construct();

        // Here you need to make a response for each group.
        // everything *= whatever;

        // If the all group construct if not the take the all group and build upon it.
        if (GroupName->first == "all") {
            // What about if the vector doesn't hold anything.
            SparseMatrix resp = groups[GroupName->first].at(0) -> GetResponse();

            for(size_t i = 1; i < GroupName->second.size(); i++){
                  resp *= groups[GroupName->first].at(i) -> GetResponse();
            }

            allResponses[GroupName->first]=resp;
            
        }else{
            // What about if the vector doesn't hold anything.
            SparseMatrix resp = GetTotalResponse(std::string("all"));

            for(size_t i = 0; i < GroupName->second.size(); i++){
                  resp *= groups[GroupName->first].at(i) -> GetResponse();
            }

            allResponses[GroupName->first]=resp;
        }
    }

}

const SparseMatrix&
SystematicManager::GetTotalResponse() const{
     return fTotalResponse;
}

// const SparseMatrix& GetTotalResponse(std::string groupName_ ) const;

const SparseMatrix&
SystematicManager::GetTotalResponse(std::string groupName_) const{
     return allResponses.at(groupName_);
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
SystematicManager::AddSystmatic(Systematic* sys_, std::string groupName_){
     groups[groupName_].push_back(sys_);
     fNSystematics++;
}

void 
SystematicManager::AddPdfToGroup(std::string& groupName_, BinnedED& ED_){
    EDnames[groupName_].push_back(ED_.GetName());
}

void
SystematicManager::DistortEDs(std::vector<BinnedED>& fWorkingEDs_){
    //You need to loop over your groups and apply everything then whatever is left. 
    //  - Loop over the groups
    //  - Get EDs name and find the systematic to apply, things in "everything" dont care about name.
    //  - Need to think about ordering the systematics in the container because map will iterate in alphabetical order.

    for(size_t j = 0; j < fWorkingEDs_.size(); j++){
        //Get the name of the ED.
        std::string name = fWorkingEDs_.at(j).GetName();

        //Is "name" in group X
        for (std::map<std::string,std::vector<std::string> >::const_iterator group = EDnames.begin(); group != EDnames.end(); ++group) {
            std::vector<std::string> v = group->second;
            if (std::find(v.begin(), v.end(), name) != v.end())
            {
                // Element in vector.
                fWorkingEDs_[j].SetBinContents(GetTotalResponse(name).operator()(fWorkingEDs_.at(j).GetBinContents()));
            }
        }
        

        //What if the vectors are a different size?
        // fWorkingEDs_[j].SetBinContents(sysMan_.GetTotalResponse().operator()(fWorkingEDs_.at(j).GetBinContents()));
        // fWorkingEDs_[j].SetBinContents(sysMan_.GetTotalResponse().operator()(fWorkingEDs_.at(j).GetBinContents()));
    }
}
