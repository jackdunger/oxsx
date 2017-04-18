#include <BinnedEDManager.h>
#include <SystematicManager.h>
#include <BinnedEDShrinker.h>
#include <BinnedED.h>
#include <Exceptions.h>
#include <sstream>

unsigned 
BinnedEDManager::GetNPdfs() const{
    return fOriginalPdfs.size();
}

size_t
BinnedEDManager::GetNDims() const{
    return fNDims;
}

double 
BinnedEDManager::Probability(const Event& data_) const{
    double sum = 0;

    for(size_t i = 0; i < fWorkingPdfs.size(); i++){
        sum += fNormalisations.at(i) * fWorkingPdfs[i].Probability(data_);
    }

    return sum;
}

double
BinnedEDManager::BinProbability(size_t bin_) const{
    double sum = 0;
    try{
        for(size_t i = 0; i < fWorkingPdfs.size(); i++){
            sum += fNormalisations.at(i) * fWorkingPdfs.at(i).GetBinContent(bin_);
    
        }
    }
    catch(const std::out_of_range&){
        throw LogicError("BinnedEDManager:: Normalisation vector doesn't match pdf vector - are the normalisations set?");
    }
    return sum;
}


void
BinnedEDManager::SetNormalisations(const std::vector<double>& normalisations_){
    if (normalisations_.size() != fOriginalPdfs.size())
        throw LogicError("BinnedEDManager: number of norms doesn't match #pdfs");
    fNormalisations = normalisations_;
}

void
BinnedEDManager::ApplySystematics(const SystematicManager& sysMan_){
    // If there are no systematics dont do anything
    //  ( working pdfs = original pdfs from initialisation)
    
    if(!sysMan_.GetSystematics().size())
        return;

    for(size_t j = 0; j < fOriginalPdfs.size(); j++){
        fWorkingPdfs[j] = fOriginalPdfs.at(j);
        fWorkingPdfs[j].SetBinContents(sysMan_.GetTotalResponse().operator()(fOriginalPdfs.at(j).GetBinContents()));
    }
}

const BinnedED&
BinnedEDManager::GetOriginalPdf(size_t index_) const{
    return fOriginalPdfs.at(index_);
}

void
BinnedEDManager::AddPdf(const BinnedED& pdf_){
    //Only possible to add to BinnedEDManager if ED has name set.
    if( pdf_.GetName().empty()){
        fOriginalPdfs.push_back(pdf_);
        fWorkingPdfs.push_back(pdf_);
        fNPdfs++;
    }else{
        std::cout << "Name not set. Pdf must have name set to be added to BinnedEDManager." << std::endl;
    }
}

void 
BinnedEDManager::AddPdfs(const std::vector<BinnedED>& pdfs_){
    for(size_t i = 0; i < pdfs_.size(); i++){
        AddPdf(pdfs_.at(i));
    }
    //Is this an error! fNPdfs populated twice.
    fNPdfs += pdfs_.size();
}

const std::vector<double>&
BinnedEDManager::GetNormalisations() const{
    return fNormalisations;
}

void
BinnedEDManager::ApplyShrink(const BinnedEDShrinker& shrinker_){
    if (!shrinker_.GetBuffers().size())
        return;
        
    // only shrink if not already shrunk! FIXME: more obvious behaviour
    if (!fWorkingPdfs.size() || fWorkingPdfs.at(0).GetNBins() != fOriginalPdfs.at(0).GetNBins())
        return;

    for (size_t i = 0; i < fWorkingPdfs.size(); i++){
        fWorkingPdfs[i] = shrinker_.ShrinkDist(fWorkingPdfs.at(i));
        fWorkingPdfs[i].Normalise();
    }
    
}


////////////////////////////////
// Make this a fit component! //
////////////////////////////////

void
BinnedEDManager::MakeFittable(){
    fParameterManager.Clear();
    if(fNormalisations.size() < fNPdfs)
        fNormalisations.resize(fNPdfs, 0);
    fParameterManager.AddContainer(fNormalisations, "Pdf Normalisation");
}

std::vector<std::string>
BinnedEDManager::GetParameterNames() const {
    return fParameterManager.GetParameterNames();
}

std::vector<double>
BinnedEDManager::GetParameters() const{
    return fParameterManager.GetParameters();
}

size_t
BinnedEDManager::GetParameterCount() const{
    return fParameterManager.GetParameterCount();
}

void
BinnedEDManager::SetParameters(const std::vector<double>& params_){
    try{
        fParameterManager.SetParameters(params_);
    }
    catch(const ParameterCountError& e_){
        throw ParameterCountError("BinnedEDManager:: " + 
                                  std::string(e_.what())
                                  );
    }
}
