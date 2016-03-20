#include <Rand.h>
#include <stdlib.h>
#include <TRandom3.h>
#include <iostream>

TRandom3
Rand::fRandomGen = TRandom3();

double
Rand::Uniform(double max_){
    return max_ * fRandomGen.Rndm();
}

int
Rand::Shoot(int max_){
    return int((max_) * Uniform());
}

double 
Rand::Gaus(double mean_, double sigma_){
    return fRandomGen.Gaus(mean_, sigma_);
}

void
Rand::SetSeed(unsigned seed_){
    fRandomGen.SetSeed(seed_);
}

unsigned
Rand::GetSeed(){
    return fRandomGen.GetSeed();
}

int
Rand::Poisson(double rate_){
    return fRandomGen.Poisson(rate_);
}
