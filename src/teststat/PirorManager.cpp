#include <Piror.h>
#include <PirorManager.h>
#include <iostream>

void
PirorManager::AddPiror(Piror piror){

    pirorList.push_back(piror);
}

std::vector<Piror>
PirorManager::GetPirors(){
    return pirorList;
}
