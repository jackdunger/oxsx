#ifndef __OXSX_ANALYTIC_ED__
#define __OXSX_ANALYTIC_ED__
#include <EventDistribution.h>
#include <FitComponent.h>
#include <ObsSet.h>
#include <string>

class PDF;
class AnalyticED : public EventDistribution, public FitComponent{
 public:
    AnalyticED(const std::string&, PDF*); // make a copy
    AnalyticED(const AnalyticED& other_); // deep copy
    ~AnalyticED();         // frees fFunction
    EventDistribution* Clone() const;
    
    double Probability(const Event&) const;
    double Probability(const std::vector<double>&) const;

    double Integral() const;
    void   Normalise();

    void   SetObservables(const ObsSet&);
    ObsSet GetObservables() const;
    
    unsigned GetNDims() const;

    std::string GetName() const;
    void SetName(const std::string&);

    // FitComponent interface : pass on calls to fFunction, change names
    void MakeFittable();
    std::vector<std::string> GetParameterNames() const;
    std::vector<double>      GetParameters() const;
    size_t                   GetParameterCount() const;
    void SetParameters(const std::vector<double>&);
    

 private:
    ObsSet        fObservables;
    PDF*          fFunction;
    double        fNorm;
    std::string   fName;
};
#endif
