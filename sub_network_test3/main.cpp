//
//  main.cpp
//  sub_network_test3

#include "Rand_Gen.hpp"
#include  "genome.hpp"

#include <iostream>
#include <assert.h>

int main()
{
    Genome genome(3,3,3,2);
    for(int i=0;i<1000000000;i++){
        genome.mutate_existing_structures();
    }
    //Mutate existing structures only mutates internals genes:
    std::cout << "exit" << std::endl;
}

