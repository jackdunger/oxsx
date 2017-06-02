#include <SystematicManager.h>

// const std::vector<Systematic*>&
// SystematicManager::GetSystematics() const{
//     return fSystematics;
// }

const std::map<std::string, std::vector<Systematic*> >& 
SystematicManager::GetSystematics() const{
    return groups;
}

void
SystematicManager::Construct(){
    // Don't do anything if there are no systematics
    if(!groups.size())
        return;

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


const SparseMatrix&
SystematicManager::GetTotalResponse(std::string groupName_) const{
     return allResponses.at(groupName_);
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
                fWorkingEDs_[j].SetBinContents(GetTotalResponse(name).operator()(fWorkingEDs_.at(j).GetBinContents()));
        }
    }
}
