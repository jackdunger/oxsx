#ifndef __OXSX_CUT__
#define __OXSX_CUT__
class EventData;

class Cut{
 public:
  virtual bool PassesCut(const EventData& ev_) const = 0;
  virtual ~Cut() {}
  virtual Cut* Clone() const = 0;
};
#endif
