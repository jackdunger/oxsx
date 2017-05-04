// A fit in nhits for signal and a background to two data sets together, where one of the systematic is constrained
// to be the same between the two data sets but the normalisations are not.
// Test of the composite likelihood class implementation

#include <BinnedPdf.h>        
#include <ROOTNtuple.h>        
#include <CompositeBinnedNLLH.h>
#include <BinnedNLLH.h>        
#include <GridSearch.h>
#include <DataSetGenerator.h>        
#include <OXSXDataSet.h>         
#include <PdfConverter.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <Rand.h>
#include <Gaussian.h>
#include <Convolution.h>
#include <Scale.h>
#include <Minuit.h>
#include <IntegrableFunction.h> 
#include <BoxCut.h>
#include <QuadraticConstraint.h>

const std::string sig1MCfile   = "SolarBi210_rat5_MCpdf.ntuple_oxsx.root";
const std::string sig2MCfile   = "SolarPo210_rat5_MCpdf.ntuple_oxsx.root";
const std::string sigTreeName = "output";

const std::string dataFile1 = "SolarBi210_rat5_data.ntuple_oxsx.root";
const std::string dataFile2 = "SolarPo210_rat5_data.ntuple_oxsx.root";
const std::string dataTreeName = "output";

int main(){

    bool addsys=true;
    // start by setting the random number generator - this makes it change each time run.
    Rand::SetSeed(0);
  
    ////////////////////        
    // 1. Set Up PDFs //        
    ////////////////////        
         
    // Set up binning        
    int nbins  = 48;
    float low  = 0;
    float high = 600;
    int buffhi = 4;
    int bufflo = 4;
    AxisCollection axes;
    axes.AddAxis(PdfAxis("nhits", low, high, nbins, "nhits"));
  
    // Only interested in first bit of data ntuple
    std::vector<size_t> reps;
    reps.push_back(0);
    DataRepresentation dataRep(reps);
         
    // Set up pdfs with these bins in this observable
    // separately for the two data sets although for now use same MC for each
    // data set A
    BinnedPdf sigA1Pdf(axes);      sigA1Pdf.SetDataRep(dataRep);
    BinnedPdf sigA2Pdf(axes);      sigA2Pdf.SetDataRep(dataRep);
    // data set B
    BinnedPdf sigB1Pdf(axes);      sigB1Pdf.SetDataRep(dataRep);
    BinnedPdf sigB2Pdf(axes);      sigB2Pdf.SetDataRep(dataRep);
  
    std::cout << "Initialised Pdfs" << std::endl;        
         
    /////////////////////////////////////        
    // 2. Fill with data and normalise //        
    /////////////////////////////////////        
         
    ROOTNtuple sig1MC(sig1MCfile, sigTreeName);
    ROOTNtuple sig2MC(sig2MCfile, sigTreeName);
  
    BoxCut boxCutnhits(0,50,550); // cuts away the overflows
    BoxCut boxCutposr(1,0,4000);  // FV cut
  
    // only apply the radial cut to the pdfs
    for(size_t i = 0; i < sig1MC.GetNEntries(); i++){
	    if(boxCutposr.PassesCut(sig1MC.GetEntry(i))){
        sigA1Pdf.Fill(sig1MC.GetEntry(i));
        sigB1Pdf.Fill(sig1MC.GetEntry(i));
      }
    }        
          
    for(size_t i = 0; i < sig2MC.GetNEntries(); i++){
      if(boxCutposr.PassesCut(sig2MC.GetEntry(i))){
        sigA2Pdf.Fill(sig2MC.GetEntry(i));
        sigB2Pdf.Fill(sig2MC.GetEntry(i));
      }
    }        
  
    sigA1Pdf.Normalise();
    sigA2Pdf.Normalise();
    sigB1Pdf.Normalise();
    sigB2Pdf.Normalise();
  
    std::cout << "Filled pdfs " << std::endl;
  
    // lets have a look at them
    TH1D th1fromPdf   = PdfConverter::ToTH1D(sigA1Pdf, false);
    th1fromPdf.SetName("pdf_1");
    TH1D th2fromPdf   = PdfConverter::ToTH1D(sigA2Pdf, false);
    th2fromPdf.SetName("pdf_2");
    TFile CheckPDFs("CheckPdfs_Min.root","recreate");
    th1fromPdf.Write();
    th2fromPdf.Write();
 
    // Systematics...
    // Now smear with a gaussian nhits resolution
    Gaussian gaussianPdf(0,20);   // Set mean and sigma
    Convolution smearer;
    smearer.SetFunction(&gaussianPdf);
    smearer.SetAxes(axes);        // axes defined above
    smearer.SetDataRep(dataRep);  // just 1 in nhits
    smearer.SetPdfDataRep(dataRep);
    smearer.MakeFittable();
    smearer.Construct();
  
    // check the smearer is getting the right parameters
    std::cout << "Initial smearing with gaussian parameters: " << smearer.GetParameters()[0] << " "
              << smearer.GetParameters()[1]<< std::endl;

    // Here I want to take a look at the PDFs smeared
    BinnedPdf sigA1PdfSmear = smearer(sigA1Pdf);
    BinnedPdf sigA2PdfSmear = smearer(sigA2Pdf);
  
    TH1D th1fromPdf_smear   = PdfConverter::ToTH1D(sigA1PdfSmear, false);
    th1fromPdf_smear.SetName("pdf_1_smear");
    TH1D th2fromPdf_smear   = PdfConverter::ToTH1D(sigA2PdfSmear, false);
    th2fromPdf_smear.SetName("pdf_2_smear");
      // Can draw directly
      TCanvas * normCan = new TCanvas();
      th1fromPdf_smear.Draw();
      th2fromPdf_smear.Draw("same");
      normCan->Print("Test.png");
    // or save to file
    th1fromPdf_smear.Write();
    th2fromPdf_smear.Write();
  
    // Now scale the pdfs
  
    Scale myscale;
    myscale.SetAxes(axes);
    myscale.SetScaleFactor(1.2);
    myscale.SetDataRep(dataRep);  // in nhits
    myscale.SetPdfDataRep(dataRep);
    myscale.MakeFittable();
    myscale.Construct();
  
    // Here I want to look at the PDFs scaled (not smeared)
    BinnedPdf sigA1PdfScale = myscale(sigA1Pdf);
    BinnedPdf sigA2PdfScale = myscale(sigA2Pdf);
  
    TH1D th1fromPdf_Scale   = PdfConverter::ToTH1D(sigA1PdfScale, false);
    th1fromPdf_Scale.SetName("pdf_1_Scale");
    TH1D th2fromPdf_Scale   = PdfConverter::ToTH1D(sigA2PdfScale, false);
    th2fromPdf_Scale.SetName("pdf_2_Scale");
      // Can draw directly
      TCanvas * normCan2 = new TCanvas();
      th1fromPdf_Scale.Draw();
      th2fromPdf_Scale.Draw("same");
      normCan2->Print("Test2.png");
    // or save to file
    th1fromPdf_Scale.Write();
    th2fromPdf_Scale.Write();

  
    CheckPDFs.Close();
  
  
  
    ////////////////////////////        
    // 3. Set Up LH function  //        
    ////////////////////////////        
    ROOTNtuple bi210(dataFile1, dataTreeName);
    ROOTNtuple po210(dataFile2, dataTreeName);
  
    // Set up a loop to do bias tests
    const int ntests = 1;
//    const int ntests = 20;
  
    // data set A composition
    int nAbi = 500;
    int nApo = 50;
    // data set B composition
    int nBbi = 100;
    int nBpo = 500;
  
  
    CutCollection thecuts;
    thecuts.AddCut(boxCutnhits);
    thecuts.AddCut(boxCutposr);
  
    std::vector<double> fits[ntests];
    for(int itest=0;itest<ntests;++itest){
    
      // DataSetGenerator
      DataSetGenerator dataGenA;
      dataGenA.AddDataSet(&bi210, nAbi);  // second arg is the expected rate
      dataGenA.AddDataSet(&po210, nApo);
      dataGenA.SetCuts(thecuts);          // apply both nhits and posr cut when creating data sets
      DataSetGenerator dataGenB;
      dataGenB.AddDataSet(&bi210, nBbi);  // second arg is the expected rate
      dataGenB.AddDataSet(&po210, nBpo);
      dataGenB.SetCuts(thecuts);          // apply both nhits and posr cut when creating data sets
  
      // Generate a data set
      OXSXDataSet fakeDataA  = dataGenA.PoissonFluctuatedDataSet();
      OXSXDataSet fakeDataB  = dataGenB.PoissonFluctuatedDataSet();
  
      // Try looking at this data set
      TFile CheckPDFs2("CheckPdfs_Min.root","UPDATE");
      TH1D thedataA(Form("thedataA_%d",itest),Form("Data A histogram for set %d", itest),nbins, low, high);
      for(int id=0;id<fakeDataA.GetNEntries();++id){
        float val = fakeDataA.GetEntry(id).GetDatum(0);
        thedataA.Fill(val);
      }
      thedataA.Write();
      
      TH1D thedataB(Form("thedataB_%d",itest),Form("Data B histogram for set %d", itest),nbins, low, high);
      for(int id=0;id<fakeDataB.GetNEntries();++id){
        float val = fakeDataB.GetEntry(id).GetDatum(0);
        thedataB.Fill(val);
      }
      thedataB.Write();

      // Create a composite binned likelihood made up of two individual ones
      CompositeBinnedNLLH lh;
      BinnedNLLH lhA;
      BinnedNLLH lhB;
      
      lhA.SetBufferAsOverflow(false); // when the buffer is applied it throws away the bins that are smeared off the end of the buffer and renormalises, rather than piling the probability into the first bin.
      if(addsys)lhA.SetBuffer(0,bufflo, buffhi);    // creates a buff of 2 bins at either side of axis 0
      lhA.AddCut(boxCutposr);      // need to make sure data is binned with this cut as well as MC
      lhA.SetDataSet(&fakeDataA);
      lhA.AddPdf(sigA1Pdf);
      lhA.AddPdf(sigA2Pdf);
      if(addsys)lhA.AddSystematic(&smearer);
      if(addsys)lhA.AddSystematic(&myscale);

      lhB.SetBufferAsOverflow(false); // when the buffer is applied it throws away the bins that are smeared off the end of the buffer and renormalises, rather than piling the probability into the first bin.
      if(addsys)lhB.SetBuffer(0,bufflo, buffhi);    // creates a buff of 2 bins at either side of axis 0
      lhB.AddCut(boxCutposr);      // need to make sure data is binned with this cut as well as MC
      lhB.SetDataSet(&fakeDataB);
      lhB.AddPdf(sigA1Pdf);
      lhB.AddPdf(sigA2Pdf);
      if(addsys)lhB.AddSystematic(&smearer);
      // if(addsys)lhB.AddSystematic(&myscale); // for now no scale on B

      // constrain the systematics
      if(addsys){
        lhA.SetConstraint("Convolution : Gaussian Means 0", 0., 10.);
        lhA.SetConstraint("Convolution : Gaussian st.devs 0", 0., 5.);
        lhA.SetConstraint("Scale Factor", 1., 0.05);
        // don't apply constraint to B as it is fixed to be the same as A anyway
      }
      if(addsys){
        lhB.SetConstraint("Convolution : Gaussian st.devs 0", 0., 1.);
        // don't apply constraint to B Mean as it is fixed to be the same as A anyway
      }
      lhA.RegisterFitComponents();
      lhB.RegisterFitComponents();
 
      // Now we make the composite likelihood out of A and B
      lh.AddBinnedNLLH(lhA);
      lh.AddBinnedNLLH(lhB);
      // with the smearing parameters shared
      lh.SetSharedParameter("Convolution : Gaussian Means 0");
//      lh.SetSharedParameter("Convolution : Gaussian st.devs 0");
      
      // Lets do some checks on what we set up
      std::vector<std::string> paramNamesA = lhA.GetParameterNames();
      for(int i=0; i<paramNamesA.size(); ++i){
        std::cout << "A Parameter " << i << " is called " << paramNamesA[i] << std::endl;
      }
      std::vector<std::string> paramNamesB = lhB.GetParameterNames();
      for(int i=0; i<paramNamesB.size(); ++i){
        std::cout << "B Parameter " << i << " is called " << paramNamesB[i] << std::endl;
      }
      // Initialise this now
      std::cout << "Initialising the composite BNNLH now " << std::endl;
      lh.Initialise();
      
      std::cout << "Built Composite LH function for test " << itest << std::endl;
      std::vector<std::string> paramNamesComp = lh.GetParameterNames();
      std::cout << " with " << lh.GetParameterCount() << " parameters " << std::endl;
      for(int i=0; i<paramNamesComp.size(); ++i){
        std::cout << "Composite Parameter " << i << " is called " << paramNamesComp[i] << std::endl;
      }
      
      
      // Set up the optimisation
      std::vector<double> minima;
      minima.push_back(10);               // normalisation of sig1 in data set A
      minima.push_back(10);               // normalisation of sig2 in data set A
      if(addsys)  minima.push_back(-20.); // Gaussian mean and sigma
      if(addsys)  minima.push_back(0.);
      if(addsys)  minima.push_back(0.9);
      minima.push_back(10);               // normalisation of sig1 in data set B
      minima.push_back(10);               // normalisation of sig2 in data set B
      if(addsys)  minima.push_back(-20.); // Gaussian mean and sigma
      std::vector<double> maxima;
      maxima.push_back(1000);
      maxima.push_back(1000);
      if(addsys)  maxima.push_back(20.);
      if(addsys)  maxima.push_back(20.);
      if(addsys)  maxima.push_back(1.1);
      maxima.push_back(1000);             // normalisation of sig1 in data set B
      maxima.push_back(1000);             // normalisation of sig2 in data set B
      if(addsys)  maxima.push_back(20.);
      std::vector<double> initialval;
      initialval.push_back(200);
      initialval.push_back(200);
      if(addsys)  initialval.push_back(-2.);
      if(addsys)  initialval.push_back(5.);
      if(addsys)  initialval.push_back(0.95);
      initialval.push_back(200);
      initialval.push_back(200);
      if(addsys)  initialval.push_back(-2.);
      std::vector<double> initialerr;
      initialerr.push_back(20);
      initialerr.push_back(20);
      if(addsys)  initialerr.push_back(0.1);
      if(addsys)  initialerr.push_back(0.1);
      if(addsys)  initialerr.push_back(0.1);
      initialerr.push_back(20);
      initialerr.push_back(20);
      if(addsys)  initialerr.push_back(0.1);
  
      Minuit min;
      min.SetMinima(minima);
      min.SetMaxima(maxima);
      min.SetInitialValues(initialval);
      min.SetInitialErrors(initialerr);

      ////////////
      // 4. Fit //
      ////////////
      std::cout << "Starting Fit for test " << itest << std::endl;
  
      FitResult result = min.Optimise(&lh);
  
      fits[itest] = result.GetBestFit();
      result.Print();
    }
  
    TFile CheckPDFs2("CheckPdfs_Min.root","UPDATE");
    // Now do something to plot all the fits.
    TH1D *hfitsA = new TH1D("hfitsA","",100,nAbi*0.8,nAbi*1.2);
    TH1D *hfitsB = new TH1D("hfitsB","",100,nApo*0.8,nApo*1.2);
    TH1D *hfitsC = new TH1D("hfitsC","",100,-10,10);
    TH1D *hfitsD = new TH1D("hfitsD","",100,0,10);
    TH1D *hfitsE = new TH1D("hfitsE","",100,-2,2);
    TH1D *hfitsF = new TH1D("hfitsF","",100,nBbi*0.8,nBbi*1.2);
    TH1D *hfitsG = new TH1D("hfitsG","",100,nBpo*0.8,nBpo*1.2);
    float num[ntests];
    for(int i=0;i<ntests;++i){
      hfitsA->Fill(fits[i][0]);
      hfitsB->Fill(fits[i][1]);
      hfitsC->Fill(fits[i][2]);
      hfitsD->Fill(fits[i][3]);
      hfitsE->Fill(fits[i][4]);
      hfitsF->Fill(fits[i][5]);
      hfitsG->Fill(fits[i][6]);
      std::cout << fits[i][0] << " " << fits[i][1] << " " << fits[i][2] << " " << fits[i][3] << " " << fits[i][4] << " " << fits[i][5] << " " << fits[i][6] << std::endl;
      num[i] = i+1;
    }
    TCanvas *Can = new TCanvas("Can");
    Can->Divide(2,3);
    Can->cd(1);
    hfitsA->Draw();
    Can->cd(2);
    hfitsB->Draw();
    Can->cd(3);
    hfitsC->Draw();
    Can->cd(4);
    hfitsD->Draw();
    Can->cd(5);
    hfitsE->Draw();
    Can->cd(6);
    hfitsF->Draw();
    Can->cd(7);
    hfitsG->Draw();
    Can->Print("Plots_Min.png");

    hfitsA->Write();
    hfitsB->Write();
    hfitsC->Write();
    hfitsD->Write();
    hfitsE->Write();
    hfitsF->Write();
    hfitsG->Write();
    CheckPDFs.Close();
    return 0;
}
