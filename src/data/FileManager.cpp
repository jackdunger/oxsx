#include <BinnedEDManager.h>
#include <CutCollection.h>
#include <FileManager.h>
#include <ROOTNtuple.h>
#include <string>
#include <vector>
#include <glob.h>


FileManager::FileManager(const std::string& name_, const std::string& folder_, const std::string& treeName_){
    names.push_back(name_);
    folders.push_back(folder_);
    treeNames.push_back(treeName_);
}

FileManager::FileManager(const std::vector<std::string>& names_, const std::vector<std::string> & folders_, const std::vector<std::string>& treeNames_){
    //Check that vectors are all the same size. 
    if(names_.size()==folders_.size() && names_.size()==treeNames_.size()){
        names=names_;
        folders=folders_;
        treeNames=treeNames_;
        Nfiles += folders_.size();
    }else{
        std::cout << "Got a problem!" << std::endl;
    }
}


void
FileManager::AddFolder(const std::string& name_,const std::string& path_,const std::string& treeName_){
    names.push_back(name_);
    folders.push_back(path_);
    treeNames.push_back(treeName_);
    Nfiles +=1;
}

inline std::vector<std::string> glob(const std::string& pat){
    using namespace std;
    glob_t glob_result;
    glob(pat.c_str(),GLOB_TILDE,NULL,&glob_result);
    vector<string> ret;
    for(unsigned int i=0;i<glob_result.gl_pathc;++i){
        ret.push_back(string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return ret;
}

void 
FileManager::FillEDs(){
    /*
     * We want to:
     *  open the files 
     *  load events into EDs in BinnedEDManager via the cut collections.
     *
    */

    //is EdManager Set?
    std::cout << "folders.size() = "<< folders.size()<< std::endl;
    for (int i = 0; i < folders.size(); ++i) {
        //glob folder.
        std::vector<std::string> files = glob(folders.at(i)+"*.root");
        // for (int i = 0; i < files.size(); ++i) {
        //     std::cout << files.at(i) << std::endl;
        // }
        BinnedED& dist = EDman.GetOriginalPdf(names.at(i));    

        for (int file = 0; file < files.size(); ++file) {
            ROOTNtuple temp(files.at(file),treeNames.at(i));
            for (int j = 0; j < temp.GetNEntries(); ++j) {
                if(cuts.PassesCuts(temp.GetEntry(j))){
                    dist.Fill(temp.GetEntry(j));
                }
            }//Entries loop
        }//File loop
    }//Folder loop

}

BinnedEDManager 
FileManager::GetBinnedEDManager() const{
    return EDman;
}

void
FileManager::AddBinnedEDManager(const BinnedEDManager& man_){
    EDman= man_;
}

void
FileManager::AddCuts(const CutCollection& cuts_){
    cuts=cuts_;
}


CutCollection
FileManager::GetCuts() const{
    return cuts;
}

