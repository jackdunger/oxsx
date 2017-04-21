/***************************************************************************************
The idea of this class is to take a folder of files and read them all into memory.
It might be a good idea to make this class a bit more high level and get it to take a obs
set and to fill histograms accordingly.
***************************************************************************************/

#ifndef __OXSX_FILE_MANAGER__
#define __OXSX_FILE_MANAGER__
#include <BinnedEDManager.h>
#include <CutCollection.h>
#include <string>
#include <vector>

class FileManager{
 public:
    FileManager(){;}
    FileManager(const std::string& name, const std::string& folder_, const std::string& treeName_);
    FileManager(const std::vector<std::string>& names,const std::vector<std::string> & folders_, const std::vector<std::string>& treeNames_);
    ~FileManager(){;}

    void FillEDs();

    void GetInfo();

    void AddFolder(const std::string& name_,const std::string& path_,const std::string& treeName_);

    void AddBinnedEDManager(const BinnedEDManager&);
    BinnedEDManager GetBinnedEDManager() const;

    void AddCuts(const CutCollection&);
    CutCollection GetCuts() const;
    
 private:
    int Nfiles;
    std::vector<std::string> names;
    std::vector<std::string> folders;
    std::vector<std::string> treeNames;
    BinnedEDManager EDman;
    CutCollection cuts;
};
#endif
