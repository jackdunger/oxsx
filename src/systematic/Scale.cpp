#include <Scale.h>
#include <sstream>
#include <DoubleParameter.h>
#include <Exceptions.h>

void 
Scale::Construct(){
    if (fScaleFactor <= 0)
        throw ValueError("Scale factor must be >0 !");
    
    if(fTransObs.GetNObservables() != 1)
        throw RepresentationError("Scale systematic must have a 1D representation!");

    const AxisCollection& axes       = fAxes;
    // the axis to scale
    const size_t  scaleAxisDataIndex = fTransObs.GetIndex(0);
    const BinAxis& scaleAxis         = axes.GetAxis(fDistObs.GetDataIndexPos(scaleAxisDataIndex));


    const size_t nBins               = axes.GetNBins(); 
    const size_t scaleAxisNBins      = scaleAxis.GetNBins(); 
 
    for(size_t i = 0; i < nBins; i++){
        // For each old bin, work out the contributions into all of the new bins
        // indices in other components should be unaffected
        std::vector<size_t> oldIndices = axes.UnpackIndices(i);
        size_t scaleBin                = oldIndices.at(fDistObs.GetDataIndexPos(scaleAxisDataIndex));
        
        double scaledLow   = scaleAxis.GetBinLowEdge(scaleBin)  * fScaleFactor;
        double scaledHigh  = scaleAxis.GetBinHighEdge(scaleBin) * fScaleFactor;
        double scaledWidth = scaledHigh - scaledLow;

        // new bin to map into, mapping only happens if the indices are the same except the one to 
        // scale so, loop over the bins in the scale axes and leave other indices the same
        // the others are zero from initialisation 
        
        std::vector<size_t> newIndices = oldIndices;
        for(size_t j = 0; j < scaleAxisNBins; j++){
            newIndices[fDistObs.GetDataIndexPos(scaleAxisDataIndex)] = j;
            size_t newScaleBin = j;
                        
            double newLow  = scaleAxis.GetBinLowEdge(newScaleBin);
            double newHigh = scaleAxis.GetBinHighEdge(newScaleBin);

            double contribution;
            // Is it in the scale region at all?
            if (newLow > scaledHigh || newHigh < scaledLow) 
                contribution = 0;

            else{
                // Is it fully in the region?
                bool includedFromBelow = newLow > scaledLow;
                bool includedFromAbove = newHigh < scaledHigh;

                if (includedFromBelow && includedFromAbove) 
                    // fully inside
                    contribution = 1/GetScaleFactor();

                else if (includedFromBelow) 
                    // spills partly over the top
                    contribution = (scaledHigh - newLow)/scaledWidth;

                else 
                    // spills partly over the bottom
                    contribution = (newHigh - scaledLow)/scaledWidth;
                
            }
            
            fResponse.SetComponent(axes.FlattenIndices(newIndices), i, contribution);
        }
               
    }
    return;
}

void
Scale::SetScaleFactor(double scaleFactor_){
    fScaleFactor = scaleFactor_;
}

double
Scale::GetScaleFactor() const{
    return fScaleFactor;
}


//////////////////////////////////////////////////////////////////
// Make this object fittable, so the scale factor is adjustable //
//////////////////////////////////////////////////////////////////

std::vector<std::string>
Scale::GetParameterNames() const{
    return std::vector<std::string>(1, "Scale Factor");
}

std::vector<double>
Scale::GetParameters() const{
    return std::vector<double>(1, fScaleFactor);
}

size_t
Scale::GetParameterCount() const{
    return 1;
}

void
Scale::SetParameters(const std::vector<double>& params_){
    if(params_.size() != 1)
        throw ParameterCountError("Scale systematic has only 1 parameter!");
    fScaleFactor = params_.at(0);
}
