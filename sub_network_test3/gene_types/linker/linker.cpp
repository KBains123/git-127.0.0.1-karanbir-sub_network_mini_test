//
//  linker.cpp
//  sub_network_test3

#include "linker.hpp"
#include "Rand_Gen.hpp"
#include "mutation_values.hpp"
#include <assert.h>

void Linker::append_linker(const float val)
{
    linkers.push_back(val);
}
void Linker::delete_linker(const int pos)
{
    linkers.erase(linkers.begin()+pos);
}

/*Randomly modifies linker values,
 no structures generated or deleted */
void Linker::mutate()
{
    int rand_point = get_randint(0,(int)linkers.size());
    linkers[rand_point] += f_rand_normal(LINKER_VALUE);
    clamp(linkers[rand_point],0,1);
    assert(in_range(linkers[rand_point],0,1));
}

//Returns linking weight between two linkers:
float get_link_weight(Linker& L1, Linker& L2)
{
    assert(L1.linkers.size()==L2.linkers.size());
    
    int num_linkers = (int)L1.linkers.size();
    float return_val{};
    for(int i=0;i<num_linkers;++i){
        return_val += L1.linkers[i]*L2.linkers[i];
    }
    clamp(return_val,0,1);
    
    assert(in_range(return_val,0,1));
    return return_val;
}











