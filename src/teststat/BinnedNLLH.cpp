#include <BinnedNLLH.h>
#include <math.h>
#include <DataSet.h>
#include <Exceptions.h>
#include <DistFiller.h>
#include <CutLog.h>
#include <iostream>

double 
BinnedNLLH::Evaluate(){
    if(!fDataSet && !fCalculatedDataDist) 
        throw LogicError("BinnedNNLH function called with no data set and no DataDist! set one of these first");
    
    if (!fCalculatedDataDist)
        BinData();
    
    if(!fAlreadyShrunk){
        fDataDist = fPdfShrinker.ShrinkDist(fDataDist);
        fAlreadyShrunk = true;
    }

    // Construct systematics 
    fSystematicManager.Construct(); 
    // Apply systematics
    std::cout << "before apply" << std::endl;
    fPdfManager.ApplySystematics(fSystematicManager);
    std::cout << "after apply" << std::endl;

    // Apply Shrinking
    fPdfManager.ApplyShrink(fPdfShrinker);

    // loop over bins and calculate the likelihood
    double nLogLH = 0;
    for(size_t i = 0; i < fDataDist.GetNBins(); i++){
        double prob = fPdfManager.BinProbability(i);
        if(!prob)
            throw std::runtime_error("BinnedNLLH::Encountered zero probability bin!");
        nLogLH -= fDataDist.GetBinContent(i) *  log(prob);        
    }

    // Extended LH correction
    const std::vector<double>& normalisations = fPdfManager.GetNormalisations();
    for(size_t i = 0; i < normalisations.size(); i++)
        nLogLH += normalisations.at(i);
            
    // Constraints
    for(std::map<std::string, QuadraticConstraint>::iterator it = fConstraints.begin();
        it != fConstraints.end(); ++it)
        nLogLH += it->second.Evaluate(fComponentManager.GetParameter(it->first));
   
    //Pirors 
    if(fPirorsSet) {
        std::map<std::string, double> tmp;

        std::vector<std::string> names =fComponentManager.GetParameterNames();
        std::vector<double> values =fComponentManager.GetParameters();
        for (int i = 0; i < fComponentManager.GetTotalParameterCount(); ++i) {
            std::cout<<"tmp["<<names.at(i)<<"]=values.at("<<i<<") = "<<values.at(i)<<std::endl;
            tmp[names.at(i)]=values.at(i);
        }
        std::vector<Piror> pirors=fPirorManager.GetPirors();

        std::cout <<  "pirors.size() = "<<pirors.size()<< std::endl;
        for (int i = 0; i < pirors.size(); ++i) {
            std::cout << "in" << std::endl;
            std::vector<std::string> names=pirors.at(i).GetParameterList();
                for (int j = 0; j < names.size(); ++j) {
                    std::cout << "inin" << std::endl;
                    std::cout <<"name"<<j<<" = "<< names.at(j)<< std::endl;
                }
            
        }
        std::cout << "before adding = "<<nLogLH << std::endl;
        nLogLH += fPirorManager.GetLogProbabilities(tmp);
        std::cout << "value = "<<fPirorManager.GetLogProbabilities(tmp) << std::endl;
        std::cout << "after adding = "<<nLogLH << std::endl;

    }

    std::cout << "END of EVAL" << std::endl;
    return nLogLH;
}

void
BinnedNLLH::BinData(){
    fDataDist =  BinnedED(fPdfManager.GetOriginalPdf(0)); // make a copy for same binning and data rep
    fDataDist.Empty();
    CutLog log(fCuts.GetCutNames());
    DistFiller::FillDist(fDataDist, *fDataSet, fCuts, log);
    fCalculatedDataDist = true;    
    fSignalCutLog = log;
}

void
BinnedNLLH::SetPdfManager(const BinnedEDManager& man_){
    fPdfManager = man_;
}

void
BinnedNLLH::SetSystematicManager(const SystematicManager& man_){
    fSystematicManager = man_;
}

void
BinnedNLLH::AddPdf(const BinnedED& pdf_){
    fPdfManager.AddPdf(pdf_);
}

void 
BinnedNLLH::AddSystematic(Systematic* sys_, std::string  group_){
    // fSystematicManager.Add(sys_);
    fSystematicManager.AddSystematic(sys_, group_);
}

void
BinnedNLLH::SetPirorManger(const PirorManager& man_){
    fPirorManager= man_;
    fPirorsSet=true;
}

void
BinnedNLLH::SetDataSet(DataSet* dataSet_){
    fDataSet = dataSet_;
    fCalculatedDataDist = false;
}

DataSet*
BinnedNLLH::GetDataSet(){
    return fDataSet;
}

void
BinnedNLLH::SetDataDist(const BinnedED& binnedPdf_){
    fDataDist = binnedPdf_;
    fCalculatedDataDist = true;
}

BinnedED
BinnedNLLH::GetDataDist() const{
    return fDataDist;
}


void
BinnedNLLH::SetBuffer(size_t dim_, unsigned lower_, unsigned upper_){
    fPdfShrinker.SetBuffer(dim_, lower_, upper_);
}

std::pair<unsigned, unsigned>
BinnedNLLH::GetBuffer(size_t dim_) const{
    return fPdfShrinker.GetBuffer(dim_);
}

void
BinnedNLLH::SetBufferAsOverflow(bool b_){
    fPdfShrinker.SetUsingOverflows(b_);
}

bool
BinnedNLLH::GetBufferAsOverflow() const{
    return fPdfShrinker.GetUsingOverflows();
}

void
BinnedNLLH::AddPdfs(const std::vector<BinnedED>& pdfs_){
  for(size_t i = 0; i < pdfs_.size(); i++)
    AddPdf(pdfs_.at(i));
}

void
BinnedNLLH::AddSystematics(const std::vector<Systematic*> systematics_){
  for(size_t i = 0; i < systematics_.size(); i++)
    AddSystematic(systematics_.at(i));
}


void
BinnedNLLH::SetNormalisations(const std::vector<double>& norms_){    
    fPdfManager.SetNormalisations(norms_);
}

std::vector<double>
BinnedNLLH::GetNormalisations() const{
    return fPdfManager.GetNormalisations();
}

void
BinnedNLLH::AddCut(const Cut& cut_){
    fCuts.AddCut(cut_);
}

void 
BinnedNLLH::SetCuts(const CutCollection& cuts_){
    fCuts = cuts_;
}

void 
BinnedNLLH::SetConstraint(const std::string& paramName_, double mean_, double sigma_){
    fConstraints[paramName_] = QuadraticConstraint(mean_, sigma_);
}


double
BinnedNLLH::GetSignalCutEfficiency() const{
    return fSignalCutEfficiency;
}

void
BinnedNLLH::SetSignalCutEfficiency(double eff_){
    fSignalCutEfficiency = eff_;
}

CutLog
BinnedNLLH::GetSignalCutLog() const{
    return fSignalCutLog;
}

void
BinnedNLLH::SetSignalCutLog(const CutLog& lg_){
    fSignalCutLog = lg_;
}

/////////////////////////////////////////////////////////
// Declare which objects should be adjusted by the fit //
/////////////////////////////////////////////////////////
void
BinnedNLLH::RegisterFitComponents(){
    fComponentManager.Clear();
    fComponentManager.AddComponent(&fPdfManager);

    //Need to get the group loop over all of them.
    const std::map<std::string, std::vector<Systematic*> > sys_ = fSystematicManager.GetSystematics();

    for (std::map<std::string, std::vector<Systematic*> >::const_iterator group_ = sys_.begin(); group_ !=sys_.end(); ++group_) {
        for (int i = 0; i < group_->second.size(); ++i) {
            fComponentManager.AddComponent(group_->second.at(i));
        }

    }
}

void
BinnedNLLH::SetParameters(const std::vector<double>& params_){
    try{
        fComponentManager.SetParameters(params_);
    }
    catch(const ParameterCountError& e_){
        throw ParameterCountError(std::string("BinnedNLLH::") + e_.what());
    }
}
                                             
void
BinnedNLLH::PrintParameters() const{
    std::cout << "-----Printing parameters for likelihood ------"<< std::endl;

    std::vector<std::string> names =fComponentManager.GetParameterNames();
    std::vector<double> values =fComponentManager.GetParameters();
    for (int i = 0; i < fComponentManager.GetTotalParameterCount(); ++i) {
       std::cout <<"name = "<<names.at(i)<<" value = "<<values.at(i)  << std::endl; 
    }
}
                 
std::vector<double>
BinnedNLLH::GetParameters() const{
    return fComponentManager.GetParameters();
}

int
BinnedNLLH::GetParameterCount() const{
    return fComponentManager.GetTotalParameterCount();
}

std::vector<std::string>
BinnedNLLH::GetParameterNames() const{
    return fComponentManager.GetParameterNames();
}
