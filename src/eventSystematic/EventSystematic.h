#ifndef __OXSX_EVENT_SYSTEMATIC__
#define __OXSX_EVENT_SYSTEMATIC__
#include <DataRepresentation.h>
#include <vector>

class EventData;
class EventSystematic{
 public:
    EventData operator()(const EventData&) = 0;
    std::vector<double> GetParameters() const;
    void SetParameters(const std::vector<double>&);

    size_t GetParamCount() const;
    void   SetParameter(size_t index_, double val_);
    double GetParameter(size_t index_);

    void   SetDataRep(const DataRepresentation&);
    DataRepresentation GetDataRep() const;
 private:
    std::vector<double> fParams;
    DataRepresentation  fDataRep;
}
#endif
