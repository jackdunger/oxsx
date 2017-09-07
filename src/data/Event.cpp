#include <Event.h>
#include <ObsSet.h>
#include <Exceptions.h>
#include <ContainerTools.hpp>
using ContainerTools::CreateMap;

Event::Event(const std::vector<double>& obs_){
    fObservations = obs_;
    fNObservables  = obs_.size();
    fObsNames = NULL;
}

std::vector<double> 
Event::GetData() const{
    return fObservations;
}

double
Event::GetDatum(size_t index_) const{
    try{
        return fObservations.at(index_);
    }

    catch(const std::out_of_range& e_){
        throw NotFoundError(Formatter() << "Event::Attempted access on non-existent observable " << index_);
    }
}

std::vector<double> 
Event::ToObsSet(const ObsSet& rep_) const{
    size_t nObs = rep_.GetNObservables();    
    if (!nObs)
        throw RepresentationError("Event Data queried with empty representation!");
    
    std::vector<double> vals(nObs, 0); // can you do this better with iterators?
    try{
        for(size_t i = 0; i < nObs; i++)
            vals[i] = fObservations.at(rep_.GetIndex(i));
    }    
    catch(std::out_of_range&){
        throw RepresentationError("Data representation asked for indicies not in the data!");
    }

    return vals;
}

void
Event::SetObservableNames(std::vector<std::string> const* p_){
    fObsNames = p_;
}

const std::vector<std::string>&
Event::GetObservableNames() const{
    if(!fObsNames)
        throw NULLPointerAccessError("Event::GetObservableNames -> names have not been set!");
    return *fObsNames;
}

std::map<std::string, double>
Event::GetObsMap() const{
    return CreateMap(GetObservableNames(), fObservations);
}
