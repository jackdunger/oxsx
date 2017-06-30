/****************************************************************************/
/* This method combines a number of likelihoods and returns the summed      */
/* output. eg MLLH_total = MLL_1+MLL_2.                                     */
/* This class handles the distribution of parameters between the components */
/* Parameters will be ordered in the order MLLHs are added to the composite */
/* The clever bit are the methods that allow you to pass the same parameter */
/* (eg a systematic) to the different MLLH                                  */
/* The fFitIndex keeps track of the mapping from phsyical parameter i to the*/
/* parameters floating in the fit, j, accounting for the shared parameters  */
/* fFitIndex[i] = j                                                         */
/****************************************************************************/

#ifndef __CompositeBinnedNLLH__
#define __CompositeBinnedNLLH__
#include <TestStatistic.h>
#include <BinnedNLLH.h>
#include <string>

class CompositeBinnedNLLH : public TestStatistic{
 public:
  CompositeBinnedNLLH() {fInitialised = 0;}
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
  //Return the parameter names
  virtual std::vector<std::string> GetParameterNames() const;
  //Call the RegisterFitComponent methods of the components
  void RegisterFitComponents();
 
  //Additional methods:
  //How many BinnedMLLH components are there?
  int GetComponentCount() const;
  //Add a BinnedNLLH object
  void AddBinnedNLLH(const BinnedNLLH&);
  
  //The clever bit
  //Set a shared parameter between the PDFs using the parameter name
  void SetSharedParameter(const std::string&);
  //After setting all the components and any shared parameters, initialise before fitting to do the book-keeping
  void Initialise();
  //Print components to screen. 
  void PrintComponents();
  
  
 private:
  std::vector<BinnedNLLH> fComponentsNHHL;
  std::vector<std::string> fSharedParams;
  // We map order all the parameters of each component in order.
  // In the FitIndex we store the mapping to fit parameter - if there are shared parameters there are less fit parameters than internal parameters
  std::vector<int> fFitIndex;  // store the mapping of internal parameter
  bool fInitialised;    // set a flag to ensure we have initialised for book-keeping
};
#endif
