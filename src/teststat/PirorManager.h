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
        void AddPiror(Piror piror);
        std::vector<Piror> GetPirorList();

    private:
        std::vector<Piror> pirorList;
};
#endif

