// A fit in energy for signal and a background
#include <string>
#include <vector>
#include <math.h>
#include <Rand.h>
#include <fstream>
#include <iostream>

#include <TCanvas.h>
#include <ROOTNtuple.h>
#include <TRandom3.h>
#include <TH1D.h>

#include <BinnedED.h>
#include <BinnedEDGenerator.h>
#include <SystematicManager.h>
#include <BinnedNLLH.h>
#include <FitResult.h>
#include <Minuit.h>
#include <DistTools.h>
#include <Minuit.h>
#include <Convolution.h>
#include <Scale.h>
#include <BoolCut.h>
#include <BoxCut.h>
#include <Gaussian.h>
#include <ParameterDict.h>
#include <ContainerTools.hpp>
#include <NuOsc.h>
#include <SurvProb.h>
#include "AntinuUtils_old.cpp"

Double_t LHFit_fit(BinnedED &data_set_pdf, BinnedED **spectra_pdf, Double_t *reactor_scale, Double_t *reactor_scale_err, std::vector<std::string> &reactor_names, std::vector<Double_t> &distances, std::vector<std::string> &reactor_types, Double_t param_d21, Double_t param_s12, Double_t param_s13, bool &fit_validity){

    printf("Begin fit--------------------------------------\n");
    printf("LHFit_fit:: del_21:%.9f, sin2_12:%.7f, sin2_13:%.7f\n", param_d21, param_s12, param_s13);

    char name[1000];
    const ULong64_t n_pdf = reactor_names.size();
    ObsSet data_rep(0);
    // set up binning
    AxisCollection axes;
    Double_t e_min = 2;//0.425*6;//0.425*2; //2; //*6 for kamland paper #1, *2 for paper #2
    Double_t e_max = 8;//0.425*19;//0.425*19; //8;
    Int_t n_bins = 60;//13;//17; //(8-2)*10; //13 for paper #1, 17 for paper #2
    axes.AddAxis(BinAxis("mc_neutrino_energy", e_min, e_max, n_bins));

    // create LH function
    BinnedNLLH lh_function;
    lh_function.SetBufferAsOverflow(true);
    int buff = 1;
    lh_function.SetBuffer(0, buff, buff);
    lh_function.SetDataDist(data_set_pdf); // initialise withe the data set

    // setup max and min ranges
    ParameterDict minima;
    ParameterDict maxima;
    ParameterDict initial_val;
    ParameterDict initial_err;

    // setup survival probability
    printf("Setup survival probability...\n");
    SurvProb *surv_prob[n_pdf];
    BinnedED **reactor_pdf = new BinnedED*[n_pdf];
    for (ULong64_t i = 0; i < n_pdf; i++){
        // for each reactor, load spectrum pdf for reactor type
        reactor_pdf[i] = new BinnedED(reactor_names[i], axes);
        reactor_pdf[i]->SetObservables(0);

        // setup survival probability
        surv_prob[i] = new SurvProb(param_d21, param_s12, distances[i]);
        surv_prob[i]->Setsinsqrtheta13s(param_s13); // manual, fixed setting of theta13
        sprintf(name, "%s_systematic", reactor_names[i].c_str());
        NuOsc reactor_systematic = NuOsc(name);
        reactor_systematic.SetFunction(surv_prob[i]);
        reactor_systematic.SetAxes(axes);
        reactor_systematic.SetTransformationObs(data_rep);
        reactor_systematic.SetDistributionObs(data_rep);
        reactor_systematic.Construct();

        if ((reactor_types[i]=="PWR")||(reactor_types[i]=="BWR"))
            reactor_pdf[i]->Add(reactor_systematic(*spectra_pdf[0]),1); //use PWR pdf
        else if (reactor_types[i]=="PHWR")
                reactor_pdf[i]->Add(reactor_systematic(*spectra_pdf[1]),1); //use PHWR pdf
            else{
               printf("Throw: Reactor doesn't match any loaded type...\n");
               continue;
            }

        // Setting optimisation limits
        sprintf(name, "%s_norm", reactor_names[i].c_str());
        Double_t min = reactor_scale[i]-1.96*reactor_scale_err[i]*reactor_scale[i];
        Double_t max = reactor_scale[i]+1.96*reactor_scale_err[i]*reactor_scale[i];
        if (min < 0) min = 0;
        minima[name] = min;
        maxima[name] = max;
        printf("  added reactor %d/%d: %s, norm: %.3f+/-%.3f (min:%.3f max:%.3f)\n", i+1, n_pdf, reactor_names[i].c_str(), reactor_scale[i], reactor_scale_err[i]*reactor_scale[i], min, max);
        initial_val[name] = reactor_scale[i];
        initial_err[name] = reactor_scale_err[i]*reactor_scale[i];

        lh_function.AddDist(*reactor_pdf[i], true);
        lh_function.SetConstraint(name, reactor_scale[i], reactor_scale_err[i]*reactor_scale[i]);
    }

    // fit
    printf("Built LH function, fitting...\n");
    Minuit min;
    min.SetMethod("Migrad");
    min.SetMaxCalls(100000);
    min.SetTolerance(0.01);
    min.SetMinima(minima);
    min.SetMaxima(maxima);
    min.SetInitialValues(initial_val);
    min.SetInitialErrors(initial_err);

    FitResult fit_result = min.Optimise(&lh_function);
    fit_result.SetPrintPrecision(9);
    ParameterDict best_fit = fit_result.GetBestFit();
    fit_result.Print();
    fit_validity = fit_result.GetValid();

    Double_t lh_val = 99999; // positive non-sensical value to return if fit is not valid
    if (fit_validity == true)
        lh_val =(-1)*lh_function.Evaluate();

    printf("fit valid: %d, lh_value:%.9f\n", fit_validity, lh_val);
    printf("End fit--------------------------------------\n");
    return lh_val;
}

int main(int argc, char *argv[]) {

    if (argc != 7){
        std::cout<<"Error: 6 arguments expected."<<std::endl;
        return 1; // return>0 indicates error code
    }
    else{
        const std::string &in_path = argv[1];
        const std::string &data_path = argv[2];
        const std::string &info_file = argv[3];
        const std::string &parameter_file = argv[4];
        const size_t flux_data = atoi(argv[5]);
        const std::string &out_file = argv[6];
        printf("Begin--------------------------------------\n");

        // read in reactor information
        std::vector<std::string> reactor_names;
        std::vector<Double_t> distances;
        std::vector<std::string> reactor_types;
        std::vector<ULong64_t> n_cores;
        std::vector<Double_t> powers;
        std::vector<Double_t> power_errs;
        readInfoFile(info_file, reactor_names, distances, reactor_types, n_cores, powers, power_errs);

        // read in parameter information
        std::vector<Double_t> d_21s;
        std::vector<Double_t> s_12s;
        std::vector<Double_t> s_13s;
        readParameterFile(parameter_file, d_21s, s_12s, s_13s);

        const ULong64_t n_pdf = reactor_names.size();
        BinnedED **spectra_pdf = new BinnedED*[n_pdf]; // PWR=0, PHWR=1
        Double_t *reactor_scale = new Double_t[n_pdf];
        Double_t *reactor_scale_err = new Double_t[n_pdf];
        const ULong64_t n_parameter_sets = d_21s.size();
        Double_t lh_values[n_parameter_sets];

        for (size_t i=0; i<(size_t)reactor_names.size(); i++)
            reactor_scale_err[i] = power_errs[i]/powers[i];

        BinnedED data_set_pdf = LHFit_initialise(spectra_pdf, reactor_scale, in_path, data_path, reactor_names, flux_data);

        bool fit_validity = 0;
        for (ULong64_t i=0; i<n_parameter_sets; i++) {
            printf("Fit number: %llu of %llu\n", i+1, n_parameter_sets);
            lh_values[i] = LHFit_fit(data_set_pdf, spectra_pdf, reactor_scale, reactor_scale_err, reactor_names, distances, reactor_types, d_21s[i], s_12s[i], s_13s[i], fit_validity);
        }

        //Write fit coefficients to txt file
        printf("writing to: %s\n", out_file.c_str());
        FILE *fOut = fopen(out_file.c_str(), "w");
        fprintf(fOut,"d21,s12,s13,lh_value,fitValidity\n");
        for (ULong64_t i=0; i<n_parameter_sets; i++)
            fprintf(fOut,"%.9f,%.7f,%.7f,%.9f,%llu\n", d_21s[i], s_12s[i], s_13s[i], lh_values[i], fit_validity);
        fclose(fOut);

        printf("End--------------------------------------\n");
        return 0; // completed successfully
    }
}