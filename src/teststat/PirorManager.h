#ifndef __OXSX_PIROR_MANAGER__
#define __OXSX_PIROR_MANAGER__
#include <vector>
#include <map>
#include <string>
#include <stddef.h>
#include <Function.h>
#include <Piror.h>

class PirorManager{
    public:
        PirorManager(){;}
        PirorManager operator=(const PirorManager& other_);
        PirorManager(std::vector<Piror> initList):
            pirorList(initList){;}

        PirorManager(const PirorManager&);

        void AddPiror(Piror piror);
        std::vector<Piror> GetPirors();
        // size_t GetNPirors();
        double GetProbabilities( const std::map<std::string, double>& params_);
        double GetLogProbabilities( const std::map<std::string, double>& params_);

    private:
        std::vector<Piror> pirorList;
};
#endif

