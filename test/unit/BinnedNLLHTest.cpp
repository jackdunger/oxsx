#include <catch.hpp>
#include <BinnedNLLH.h>
#include <Gaussian.h>
#include <DistTools.h>
#include <OXSXDataSet.h>
#include <iostream>

TEST_CASE("Binned NLLH, 3 rates no systematics"){
    Gaussian gaus1(1, 10);
    Gaussian gaus2(1, 20);
    Gaussian gaus3(1, 30);

    AxisCollection axes;
    axes.AddAxis(BinAxis("axis1", -40, 40 , 200));

    BinnedED pdf1("a", DistTools::ToHist(gaus1, axes));
    BinnedED pdf2("b", DistTools::ToHist(gaus2, axes));
    BinnedED pdf3("c", DistTools::ToHist(gaus3, axes));


    size_t centralBin = pdf1.FindBin(std::vector<double>(1,0));
    double prob1 = pdf1.GetBinContent(centralBin);
    double prob2 = pdf2.GetBinContent(centralBin);
    double prob3 = pdf3.GetBinContent(centralBin);

    pdf1.SetObservables(0);
    pdf2.SetObservables(0);
    pdf3.SetObservables(0);
    
    BinnedNLLH lh;
    lh.AddPdf(pdf1);
    lh.AddPdf(pdf2);
    lh.AddPdf(pdf3);

    OXSXDataSet data;
    data.AddEntry(Event(std::vector<double>(1, 0)));
    
    lh.SetDataSet(&data);

    lh.RegisterFitComponents();
    SECTION("Correct Probability"){
        double sumLogProb = -log(prob1 + prob2 + prob3);
        double sumNorm    = 3;
        
        ParameterDict params;
        params["a_norm"] = 1;
        params["b_norm"] = 1;
        params["c_norm"] = 1;
        lh.SetParameters(params);
        REQUIRE(lh.Evaluate() == Approx(sumNorm + sumLogProb));
    }
    SECTION("Correct Probability with constraint"){
        lh.SetConstraint("a_norm", 3, 1);

        double sumLogProb = -log(prob1 + prob2 + prob3);
        double sumNorm    = 3;
        double constraint = 2;

        ParameterDict params;
        params["a_norm"] = 1;
        params["b_norm"] = 1;
        params["c_norm"] = 1;
        lh.SetParameters(params);
        REQUIRE(lh.Evaluate() == Approx(sumNorm + sumLogProb + constraint));
    }
}
