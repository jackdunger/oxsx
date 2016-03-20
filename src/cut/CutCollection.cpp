#include <CutCollection.h>
#include <EventData.h>
#include <Cut.h>
#include <iostream>
bool 
CutCollection::PassesCuts(const EventData& event_) const{
  for(size_t i = 0; i < fCuts.size(); i++)
    if (!fCuts.at(i)->PassesCut(event_))
      return false;

  return true;
}

void
CutCollection::AddCut(const Cut& cut_){
  fCuts.push_back(cut_.Clone());
}

CutCollection::~CutCollection(){
  for(size_t i = 0; i < fCuts.size(); i++){
    delete fCuts.at(i);
  }
}

CutCollection::CutCollection(const CutCollection& other_){
  fCuts.reserve(other_.fCuts.size());
  for(size_t i = 0; i < other_.fCuts.size(); i++)
	fCuts.push_back(other_.fCuts.at(i)->Clone());
}

CutCollection&
CutCollection::operator= (const CutCollection& other_){
  for(size_t i = 0; i < fCuts.size(); i++)
	delete fCuts.at(i);

  fCuts.resize(0);
  fCuts.reserve(other_.fCuts.size());
  for(size_t i = 0; i < other_.fCuts.size(); i++)
      fCuts.push_back(other_.fCuts.at(i)->Clone());
  return *this;
}
