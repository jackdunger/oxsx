#include <FileManager.h>
#include <CutCollection.h>
#include <ROOTNtuple.h>
#include <Exceptions.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>     // std::distance
#include <glob.h>
#include <iostream>


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

// void
// FileManager::FillEDs(std::vector<BinnedED>& fPdfs_){
//     #<{(|
//      * We want to:
//      *  open the files 
//      *  load events into EDs in BinnedEDManager via the cut collections.
//      *
//      |)}>#
//     std::cout << "In to FillEDs" << std::endl;
//     for (size_t i = 0; i < fPdfs_.size(); ++i) {
//
//         std::string name = fPdfs_.at(i).GetName();        
//         std::cout << "processing pdf = "<< name << std::endl;
//         try {
//             for (int name_ = 0; name_ < names.size(); ++name_){
//                 if( name==names.at(name_)){
//                     // size_t index = std::distance(names.begin(), pos);
//                     std::vector<std::string> files = glob(folders.at(name_)+"*.root");
//
//                     // for (int file = 0; file < files.size(); ++file) {
//                     //     std::cout <<"file = "<<file<<" "<< files.at(file) << std::endl;
//                     // }
//
//                     std::cout << "about to fill" << std::endl;
//                     for (int file = 0; file < files.size(); ++file) {
//                         ROOTNtuple temp(files.at(file),treeNames.at(name_));
//                         // std::cout << "opened temp root" << std::endl;
//                         // std::cout << "temp.GetNEntries() = "<<temp.GetNEntries() << std::endl;
//                         for (int j = 0; j < temp.GetNEntries(); ++j) {
//                             if(cuts.PassesCuts(temp.GetEntry(j))){
//                                 fPdfs_.at(i).Fill(temp.GetEntry(j));
//                             }
//                         }//Entries loop
//                     }//File loop
//                 }else{
//                     std::cerr << "Out of Range error: "<<'\n';
//                     std::cerr << "name = \""<< name<< "\" not in FileManger's folder path"  << '\n';
//                 }
//
//             }
//         }catch (const std::out_of_range& oor) {
//             std::cerr << "Out of Range error: " << oor.what() << '\n';
//             std::cerr << "name = \""<< name<< "\" not in FileManger's folder path"  << '\n';
//         }
//
//
//     }//vector of pdfs.
// }

void
FileManager::FillEDs(std::vector<BinnedED>& fPdfs_){
    /*
     * We want to:
     *  open the files 
     *  load events into EDs in BinnedEDManager via the cut collections.
     *
     */
    std::cout << "In to FillEDs" << std::endl;
    for (size_t i = 0; i < fPdfs_.size(); ++i) {

        std::string name = fPdfs_.at(i).GetName();        
        std::cout << "processing pdf = "<< name << std::endl;
        for (int i = 0; i < names.size(); ++i) {
            std::cout << "names.at"<<i<<") = "<<names.at(i) << std::endl;
        }
        try {
            std::vector<std::string>::iterator pos = std::find(names.begin(), names.end(), name);
            size_t kill = std::distance(names.begin(), pos);
            std::cout << "kill = "<<kill << std::endl;
            folders.at(kill);

            std::vector<std::string>::iterator iter = names.begin();

            while ((iter = std::find(iter, names.end(), name)) != names.end())
            {
                // Do something with iter
                size_t index = std::distance(names.begin(), iter);
                std::vector<std::string> files = glob(folders.at(index)+"*.root");

                // for (int i = 0; i < files.size(); ++i) {
                //     std::cout <<"i = "<<i<<" "<< files.at(i) << std::endl;
                // }

                std::cout << "about to fill" << std::endl;
                for (int file = 0; file < files.size(); ++file) {
                    ROOTNtuple temp(files.at(file),treeNames.at(i));
                    // std::cout << "opened temp root" << std::endl;
                    // std::cout << "temp.GetNEntries() = "<<temp.GetNEntries() << std::endl;
                    for (int j = 0; j < temp.GetNEntries(); ++j) {
                        if(cuts.PassesCuts(temp.GetEntry(j))){
                            fPdfs_.at(i).Fill(temp.GetEntry(j));
                        }
                    }//Entries loop
                }//File loop
                iter++;
            }

            }catch (const std::out_of_range& oor) {
                std::cerr << "Out of Range error: " << oor.what() << '\n';
                std::cerr << "name = \""<< name<< "\" not in FileManger's folder path"  << '\n';
            }


    }//vector of pdfs.
}

void
FileManager::AddCuts(const CutCollection& cuts_){
    cuts=cuts_;
}

CutCollection
FileManager::GetCuts() const{
    return cuts;
}

