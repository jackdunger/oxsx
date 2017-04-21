/***************************************************************************************************/
/* A collection of BinnedPDFs with normalisations for calculating the probability of a given obs   */
/* given some rates and some pdfs                                                                  */
/***************************************************************************************************/
#ifndef __OXSX_BINNED_ED_MANAGER__
#define __OXSX_BINNED_ED_MANAGER__
#include <BinnedED.h>
#include <FitComponent.h>
#include <ParameterManager.h>
#include <vector>

class Event;
class SystematicManager;
class BinnedEDShrinker;
class BinnedEDManager : public FitComponent{
 public:
    BinnedEDManager() : fNPdfs(0) {}

    void   AddPdf(const BinnedED&);
    void   AddPdfs(const std::vector<BinnedED>&);

    double Probability(const Event&) const;
    double BinProbability(size_t) const;
    
    const std::vector<double>& GetNormalisations() const;
    void SetNormalisations(const std::vector<double>& normalisations_);

    void ApplySystematics(const SystematicManager& sysMan_);
    void ApplyShrink(const BinnedEDShrinker&);

    const BinnedED& GetOriginalPdf(size_t index_) const;
    unsigned GetNPdfs() const;
    size_t   GetNDims() const;
    

    // Make a fittable component - i.e. rescale the binned pdfs inside to fit
    void MakeFittable();
    std::vector<std::string> GetParameterNames() const;
    std::vector<double> GetParameters() const;
    size_t GetParameterCount() const;
    void SetParameters(const std::vector<double>&);

 private:
    ParameterManager       fParameterManager;
    std::vector<BinnedED>  fOriginalPdfs;
    std::vector<BinnedED>  fWorkingPdfs;
    std::vector<double>    fNormalisations;
    int                    fNPdfs;
    size_t fNDims;
};
#endif
