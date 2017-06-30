#include <catch.hpp>
#include <FileManager.h>
#include <iostream>
#include <TTree.h>
#include <TBranch.h>
#include <TFile.h>

TEST_CASE("Test FileManager"){
    
    // Create fake data file.
    TFile *f = new TFile("FileManagerTest.root","RECREATE");
    TTree *T = new TTree("ntuple","");
    f->cd();
    double x;
    T->Branch("x",&x,"x/D");
    for (double i = 0; i < 10; ++i) {
        x=i;
        T->Fill();
    }
    T->Write();
    f->Close();

    //Make dummy axes.
    AxisCollection axes;        
    axes.AddAxis(BinAxis("energy", 0, 3, 100, "Energy"));
         
    // Dummy obsSet.
    ObsSet obsSet(0);        
         
    // Set up pdf with these bins in this observable with given names.        
    BinnedED first("one",axes); first.SetObservables(obsSet);
    BinnedED second("two",axes); second.SetObservables(obsSet);
    BinnedED third("combined",axes); third.SetObservables(obsSet);

    std::vector<BinnedED> pdfs;
    pdfs.push_back(first);
    pdfs.push_back(second);
    pdfs.push_back(third);


    FileManager man;
    
    //Add folder paths with associated names and tree names.
    //Note folder name should be equal to the name of the pdf it will fill.
    // man.AddFolder(<name>,<path>,<treeName>);
    man.AddFolder("one",".","output");
    man.AddFolder("two",".","output");
    man.AddFolder("combined",".","output");
    man.AddFolder("combined",".","output");

    // Fill pdfs, in place.
    man.FillEDs(pdfs); 

    SECTION("Name set correctly"){
        REQUIRE(man.GetNames().at(0) == "one" && man.GetNames().at(2) == "combined" );
    }

    SECTION("Paths the same"){
        REQUIRE(man.GetPaths().at(0) == man.GetPaths().at(1));
    }

    SECTION("Output tree names the same"){
        REQUIRE(man.GetTreeNames().at(0) == man.GetTreeNames().at(1));
    }
    
    SECTION("Check Filling"){
        REQUIRE(pdfs.at(0).GetBinContent(0)==pdfs.at(1).GetBinContent(0));
        REQUIRE(pdfs.at(0).GetBinContent(0)==0);
    }

    remove("FileManagerTest.root");
}
