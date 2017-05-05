#include <iostream>
#include <FileManager.h>
#include <BinnedED.h>
#include <AxisCollection.h>
#include <BinAxis.h>
#include <DistTools.h>
#include <BoxCut.h>
#include <CutCollection.h>

#include <TH1D.h>
#include <TCanvas.h>

#include <math.h>
#include <string>

int main(int argc, char *argv[])
{
    //Supply name, path and treeName of folders.
    std::string name_1 = ""; std::string path_1 =""; std::string treeName_1="";
    std::string name_2 = ""; std::string path_2 =""; std::string treeName_2="";


    //Set up a axes.
    AxisCollection axes;        
    axes.AddAxis(BinAxis("energy", 0, 3, 100, "Energy"));
         
    // Only interested in first bit of the ntuple.
    ObsSet obsSet(0);        

    //Set some cut values.
    double ELow=0.;
    double EHigh=2.5;

    //Make a cut collection.
    CutCollection cutCol;
    BoxCut Ecut(0,ELow,EHigh);
    cutCol.AddCut(Ecut);
         
    // Set up pdf with these bins in this observable with given names.        
    BinnedED first(name_1,axes); first.SetObservables(obsSet);
    BinnedED second(name_2,axes); second.SetObservables(obsSet);
    BinnedED third("combined",axes); third.SetObservables(obsSet);

    std::vector<BinnedED> pdfs;
    pdfs.push_back(first);
    pdfs.push_back(second);
    pdfs.push_back(third);


    FileManager man;
    
    //Add folder paths with associated names and tree names.
    //Note folder name should be equal to the name of the pdf it will fill.
    // man.AddFolder(<name>,<path>,<treeName>);
    man.AddFolder(name_1,path_1,treeName_1);
    man.AddFolder(name_2,path_2,treeName_2);
    man.AddFolder("combined",path_1,treeName_1);
    man.AddFolder("combined",path_2,treeName_2);

    man.AddCuts(cutCol);

    // Fill pdfs, in place.
    man.FillEDs(pdfs); 

    std::cout << "Filled EDs, generating plots" << std::endl;
    
    TCanvas c1("","",1000,1000);
    int nSections= ceil(sqrt(pdfs.size()));
    c1.Divide(nSections,nSections);

    std::vector<TH1D> hist2;
    for (int i = 0; i < pdfs.size(); ++i) {
        hist2.push_back( DistTools::ToTH1D(pdfs.at(i)));
    }
    for (int i = 0; i < hist2.size(); ++i) {
        c1.cd(i+1);
        hist2.at(i).Draw();
    }
    c1.Print("AllFilledEDs.png");

    return 0;
}
