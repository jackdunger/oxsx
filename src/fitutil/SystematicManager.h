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
    SystematicManager(): fNSystematics(0) {;}
    ~SystematicManager() {}

    void AddSystmatic(Systematic*,const std::string groupName_ = std::string("all"));
    void AddPdfToGroup(std::string& groupName_, BinnedED& ED_);

    const std::map<std::string,std::vector<Systematic*> >& GetSystematics() const;
    // const std::vector<Systematic*>& GetSystematics() const;
    
    size_t GetNSystematics() const;
    // const SparseMatrix& GetTotalResponse() const;

    const SparseMatrix& GetTotalResponse(std::string groupName_ = std::string("all") ) const;

    void DistortEDs(std::vector<BinnedED>& fWorkingEDs);

    void Construct();
    
 private:
    std::vector<Systematic*> fSystematics;
    SparseMatrix fTotalResponse;
    size_t fNSystematics;
    std::map<std::string,SparseMatrix> allResponses;
    std::map<std::string,std::vector<Systematic*> > groups;
    std::map<std::string,std::vector<std::string> > EDnames;
    std::vector<std::string> sysOrder; // This isn't used.
};
#endif
