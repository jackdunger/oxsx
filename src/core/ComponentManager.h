#ifndef __OXSX_COMPONENT_MANAGER__
#define __OXSX_COMPONENT_MANAGER__
#include <FitComponent.h>
#include <vector>
#include <string>

class ComponentManager{
 public:
    ComponentManager() : fTotalParamCount(0), fComponentCount(0) {}
    void AddComponent(FitComponent*);

    void SetParameters(const std::vector<double>&);
    std::vector<std::string>  GetParameterNames() const;
    std::vector<double>       GetParameters() const;
    int                       GetTotalParameterCount() const;
    void                      Clear();

    double GetParameter(const std::string&) const;
    size_t FindParameter(const std::string&) const;

 private:
    std::vector<FitComponent*>  fComponents;
    std::vector<int>            fParamCounts;

    int fTotalParamCount;
    int fComponentCount;
};
#endif
