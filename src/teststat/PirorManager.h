#ifndef __OXSX_PIROR_MANAGER__
#define __OXSX_PIROR_MANAGER__
#include <vector>
#include <string>
#include <stddef.h>
#include <Function.h>
#include <Piror.h>

class PirorManager{
    public:
        PirorManager(){;}
        PirorManager(std::vector<Piror> initList):
            pirorList(initList){;}
        void AddPiror(Piror piror);
        std::vector<Piror> GetPirors();

    private:
        std::vector<Piror> pirorList;
};
#endif

