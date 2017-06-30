#include <HistTools.h>
#include <Histogram.h>
#include <ContainerTools.hpp>

std::vector<Histogram>
HistTools::MakeAllHists(const AxisCollection& axes_,
                        const std::set<std::pair<std::string, std::string> >& combinations_){
    std::vector<Histogram> hists;
    hists.reserve(combinations_.size());
    
    typedef std::set<std::pair<std::string, std::string> >::iterator SetIt;

    for(SetIt it = combinations_.begin(); it != combinations_.end(); ++it){
        AxisCollection axes;
        axes.AddAxis(axes_.GetAxis(it->first));
        axes.AddAxis(axes_.GetAxis(it->second));
            
        hists.push_back(Histogram(axes));
    }
    return hists;
}

void
HistTools::FillAllHists(std::vector<Histogram>& hists_, const std::map<std::string, double>& fillVals_){
    // note this assumes that the histograms have axes that match the keys of fillvals throws otherwise
    for(size_t i = 0; i < hists_.size(); i++){
        hists_.at(i).Fill(fillVals_);
    }    
}
