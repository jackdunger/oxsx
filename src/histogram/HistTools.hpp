#include <AxisCollection.h>
#include <Histogram.h>

template<typename T1>
std::vector<Histogram> 
HistTools::MakeAllHists(const AxisCollection& axes_,
                        const T1& names_){
    std::vector<Histogram> hists;
    hists.reserve(names_.size());

    typedef typename T1::const_iterator It;
    for(It it = names_.begin(); it != names_.end(); ++it){
        AxisCollection axes;
        axes.AddAxis(axes_.GetAxis(*it));
        hists.push_back(Histogram(axes));
    }
    return hists;
}
