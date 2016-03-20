#include <EventSystematic.h>
#include <PdfExceptions.h>

std::vector<double>
EventSystematic::GetParameters() const {
    return fParams;
}

void
EventSystematic::SetParameters(const std::vector<double>& params_){
    fParams = params_;
}

size_t
EventSystematic::GetParamCount() const{
    return fParams.size();
}

void
EventSystematic::SetParameter(size_t index_, double val_){
    try{
        return fParams[index_] = val;
    }
    catch(const std::out_of_bounds& e)
        throw OutOfBoundsError("EventSystematic::SetParameter out of bounds!");
}

double
EventSystematic::GetParameter(size_t index_) const {
    try{
        return fParams.at(index_);
    }
    catch(const std::out_of_bounds& e)
        throw OutOfBoundsError("EventSystematic::SetParameter out of bounds!");
}
