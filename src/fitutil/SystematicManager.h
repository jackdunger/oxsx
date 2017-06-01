/******************************************************************************************************/
/* Manages a set of pdfs, recieves a list of parameters and passes them out to each of the systematics*/
/* and triggeres their reconstruction. Systematics inside are passed to a set of pdfs to change       */
/* them                                                                                               */
/******************************************************************************************************/

#ifndef __SYSTEMATIC_MANAGER__
#define __SYSTEMATIC_MANAGER__
#include <vector>
#include <Systematic.h>
#include <SparseMatrix.h>

class SystematicManager{
 public:
    SystematicManager(): fNSystematics(0) {
    
    }
    ~SystematicManager() {}

    void AddSystmatic(Systematic*, std::string& groupName_);
    void AddPdf(std::string& groupName_, BinnedED& ED_);

    void Add(Systematic*);
    const std::vector<Systematic*>& GetSystematics() const;
    
    size_t GetNSystematics() const;
    const SparseMatrix& GetTotalResponse() const;

    void DistortEDs(std::vector<BinnedED>& workingEDs_);

    void Construct();
    
 private:
    std::vector<Systematic*> fSystematics;
    SparseMatrix fTotalResponse;
    size_t fNSystematics;
    std::map<std::string,Systematics*> groups;
};
#endif
