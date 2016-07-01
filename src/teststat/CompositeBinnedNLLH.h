/****************************************************************************/
/* This method combines a number of likelihoods and returns the summed      */
/* output. eg MLLH_total = MLL_1+MLL_2.                                     */
/* This class handles the distribution of parameters between the components */
/* Parameters will be ordered in the order MLLHs are added to the composite */
/****************************************************************************/

#ifndef __CompositeBinnedNLLH__
#define __CompositeBinnedNLLH__
#include <TestStatistic.h>
#include <BinnedNLLH.h>

class CompositeBinnedNLLH : public TestStatistic{
 public:
  CompositeBinnedNLLH() {}
  virtual ~CompositeBinnedNLLH() {};
  
  //Inherited methods that must be implemented:
  //This method will call the Evaluate method for each component and sum them
  double Evaluate();
  
  //This method will set the parameters - need to distribute them between the components
  void SetParameters(const std::vector<double>&);
  //Get the parameters from each component and order them as expected in the return vector
  std::vector<double> GetParameters() const;
  //Count up the parameters over all the components
  int GetParameterCount() const;
  //Call the RegisterFitComponent methods of the components
  void RegisterFitComponents();
 
  //Additional methods:
  //How many BinnedMLLH components are there?
  int GetComponentCount() const;
  //Add a BinnedNLLH object
  void AddBinnedNLLH(const BinnedNLLH&);
  
  
 private:
  std::vector<BinnedNLLH> fComponentsNHHL;
};
#endif