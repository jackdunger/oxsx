#ifndef __OXSX_SPECTRAL_FIT_PDF__
#define __OXSX_SPECTRAL_FIT_PDF__
#include <BinnedED.h>
#include <FitComponent.h>
#include <ParameterManager.h>

class SpectralFitDist : public BinnedED, public FitComponent{
 public:
 SpectralFitDist(const std::string& name_, 
                 const AxisCollection& axes_) : BinnedED(name_, axes_) {}

    // Make this fittable with each bin content adjustable
    void MakeFittable() {}
    std::vector<std::string> GetParameterNames() const;
    std::vector<double> GetParameters() const;
    size_t GetParameterCount() const;
    void   SetParameters(const std::vector<double>&);
};
#endif
