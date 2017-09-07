#include <DataSet.h>
#include <ObsSet.h>
#include <Exceptions.h>
#include <string>
#include <vector>
#include <algorithm>
#include <ObsSetBuilder.h>

ObsSet 
DataSet::MakeDataRep(const std::vector<std::string> observableNames_) const{    
    return ObsSetBuilder::Build(observableNames_, GetObservableNames());
}


