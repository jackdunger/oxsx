/***********************************************************************************************/
/* Represents the observations associated with a single event.                                 */
/* Wraps a vector of the data points. Pdfs can query it with their own representation of the   */
/* data to get the relevant indicies ordered correctly.                                        */
/* For example, the data may come as (energy, radius, time). But the pdf is only interested in */
/* (radius, energy) pairs so it has the representation (1, 0).                                 */
/* EventData(4, 2, 1).ToRepresentation(pdfRep) = {2, 4}                                        */
/***********************************************************************************************/

#ifndef __OXSX_EVENT__
#define __OXSX_EVENT__
#include <cstdlib>
#include <vector>
#include <map>
#include <string>

class ObsSet;
class Event{
 public:
    Event(const std::vector<double>& obs_);
    std::vector<double> GetData() const;
    double GetDatum(size_t) const;
    std::vector<double> ToObsSet(const ObsSet&) const;

    size_t GetNObservables() const {return fNObservables;}

    void SetObservableNames(std::vector<std::string> const * p_);
    const std::vector<std::string>& GetObservableNames() const;

    std::map<std::string, double> GetObsMap() const;
    
 private:
    size_t fNObservables;
    std::vector<double> fObservations;
    std::vector<std::string> const* fObsNames;
};
#endif
