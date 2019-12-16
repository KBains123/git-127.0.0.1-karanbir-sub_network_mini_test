//
//  substrate.cpp
//  sub_network_test3

#include "substrate.hpp"
#include "Rand_Gen.hpp"
#include "mutation_values.hpp"

#include <math.h>
#include <assert.h>
#include <iostream>

inline float get_scaled_dist(float dist,const float thresh);

std::vector<float> vals;

/* Mutates linker parameters */
void Substrate::mutate()
{
    int mutate_point = get_randint(0, 4);
    switch (mutate_point)
    {
        case 0:
            if(b_thresh(SUBSTRATE_A_RESET)){
                active_point = f_uniform();
            }else{
                active_point += f_rand_normal(SUBSTRATE_A_POINT);
            }
            clamp(active_point, 0, 1);
            assert(in_range(active_point,0,1));
            break;
        case 1:
            response_threshold += f_rand_normal(SUBSTRATE_A_POINT);
            response_threshold = abs(response_threshold);
            clamp(response_threshold,0,1);
            assert(in_range(response_threshold,0,1));
            break;
        case 2:
            out_weight += f_rand_normal(SUBSTRATE_O_WEIGHT);
            break;
        case 3:
            max_threshold += f_rand_normal(SUBSTRATE_O_THRESH);
            max_threshold *= (max_threshold>0);
            assert(max_threshold>=0);
            break;
        default:
            std::cout << "Out of bounds mutation point!\n";
            throw;break;
    }
}

float Substrate::get_output()
{
    float return_val = binding;
    if(return_val>max_threshold){
        max_threshold=return_val;
    }
    assert(in_range(return_val,0,max_threshold));
           
    return_val *= out_weight;
    assert(!isnan(return_val));
    return return_val;
}

/*Updates binding value based upon substrate-substrate
 interactions: */
void sim_substrate_interaction(Substrate& s1, Substrate& s2)
{
    assert(in_range(s1.active_point,0,1));
    assert(in_range(s2.active_point,0,1));
    
    //(largest distance,shortest distance) -> (0,1):
    float dist = 1-abs(s1.active_point-s2.active_point);
    
    //Appending to substrates:
    s1.binding += get_scaled_dist(dist, s1.response_threshold);
    s2.binding += get_scaled_dist(dist, s2.response_threshold);
}

//Constructors:
Substrate::Substrate(const Substrate &L)
{
    active_point  = L.active_point;
    out_weight = L.out_weight;
    max_threshold = L.max_threshold;
    response_threshold = L.response_threshold;
    assert(in_range(active_point,0,1));
    assert(!isnan(out_weight));
    assert(in_range(response_threshold,0,1));
}
Substrate::Substrate(const float _active_point,
                     const float _response_threshold,
                     const float _max_threshold,
                     const float _out_weight)
{
    assert(in_range(_active_point, 0, 1));
    assert(in_range(_out_weight, 0, 1));
    assert(!isnan(_max_threshold));
    
    active_point  = _active_point;
    max_threshold = _max_threshold;
    response_threshold = _response_threshold;
    out_weight = _out_weight;
}

//Returns distance in range (0,1):
inline float get_scaled_dist(float dist,const float thresh)
{
    assert(in_range(dist,0,1));
    assert(in_range(thresh,0,1));
    
    float return_val{0};
    dist -= thresh;
    if(dist > 0){
        return_val = dist*(1.0/(1.0-thresh));
    }
    
    //Verifying that return value is in range:
    assert(in_range(return_val,0,1));
    return return_val;
}







