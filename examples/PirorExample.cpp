#include <Piror.h>
#include <Function.h>
#include <PirorManager.h>
#include <iostream>
#include <Gaussian.h>

// class Func: public Gaussian
// {
//     public:
//         Func(){;}
//         double operator() (std::vector<double> params){
//             return params[0]-params[1];
//         }
//
//     private:
//         #<{(| data |)}>#
// };

int main(int argc, char *argv[])
{
    std::cout << "Starting test" << std::endl; 

    Gaussian* func = new Gaussian(1,2);
    // Func* func = new Func();
    Piror* pir = new Piror();
    pir->SetFunction(func);
    std::vector<std::string> str;
    str.push_back("alpha");
    pir->SetParameterList(str);

    // std::vector<std::string> params;
    // params.push_back("norm1");
    // params.push_back("norm2");
    // Piror* pir2 = new Piror(params,func);
    
    std::vector<Piror> pirors;
    pirors.push_back(*pir);
    // pirors.push_back(*pir2);
    PirorManager * man = new PirorManager(pirors);
     
    std::vector<Piror> pirorList= man->GetPirors();
    for (int i = 0; i < pirorList.size(); ++i) {
        
        std::cout << "1" << std::endl;
        std::vector<double> red;
        red.push_back(1);
        // red.push_back(3);
        std::cout << pirorList.at(i).Probability(red)<<std::endl; 
    }
    // for (std::vector<Piror>::iterator i = man->GetPirors().begin(); i != man->GetPirors().end(); ++i) {
    for (std::vector<Piror>::iterator i = pirorList.begin(); i != pirorList.end(); ++i) {
        std::cout << "1" << std::endl;
        std::vector<double> red;
        red.push_back(1);
        // red.push_back(3);
        std::cout<< (*i).Probability(red)<<std::endl; 
    }
    std::map<std::string, double> red;
    red[std::string("alpha")]=1;
        
    std::cout << man->GetProbabilities(red) << std::endl;

    return 0;
}
