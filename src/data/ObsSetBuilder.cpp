#include <ObsSetBuilder.h>
#include <ObsSet.h>
#include <algorithm>
#include <Exceptions.h>
#include <ContainerTools.hpp>

ObsSet
ObsSetBuilder::Build(const StrVec& setNames_, const StrVec& allNames_){
    std::vector<size_t> indices;
    indices.reserve(setNames_.size());
    for(size_t i = 0; i < setNames_.size(); i++){
        const std::string& name = setNames_.at(i);

        StrVec::const_iterator it = std::find(allNames_.begin(), allNames_.end(), name);
        
        if(it == allNames_.end())
            throw NotFoundError("ObsSetBuilder:: can't find observable " + name + \
                                " only have: \n\t" + ContainerTools::ToString(allNames_));
        
        indices.push_back(it - allNames_.begin());
    }    
    return ObsSet(indices);
}
