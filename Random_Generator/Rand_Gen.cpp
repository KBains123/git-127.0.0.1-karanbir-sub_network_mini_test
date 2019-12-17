//
//  Rand_Gen.cpp
//  sub_network_test

#include "Rand_Gen.hpp"
#include "mutation_values.hpp"
#include <assert.h>
#include <random>

//Random number generator: generates value over uniform distribution from 0-1:
constexpr int RAND_SEED = 13234232;
std::mt19937 generator{RAND_SEED};
std::uniform_real_distribution<double> dis{0.0, 1.0};
float f_uniform(){
    return dis(generator);
}

//Returns random integer in range:
int get_randint(const int low,const int high){
    int rand_int = floor(f_uniform()*(high-low));
    if(rand_int+low==high){
        rand_int--;
    }
    int return_val = rand_int + low;
    assert(in_range(return_val,low,high));
    return rand_int + low;
}

//Returns true if random value exceeds threshold:
bool b_thresh(const float thresh){
    return f_uniform()<thresh;
}

/** THIS IS STOLEN I DID NOT WRITE THIS*/
#define PI 3.141593

float myErfInv(float x){
    float tt1, tt2, lnx, sgn;
    sgn = (x < 0) ? -1.0f : 1.0f;
    
    x = (1 - x)*(1 + x);        // x = 1 - x*x;
    lnx = logf(x);
    
    tt1 = 2/(PI*0.147) + 0.5f * lnx;
    tt2 = 1/(0.147) * lnx;
    
    return(sgn*sqrtf(-tt1 + sqrtf(tt1*tt1 - tt2)));
}

inline float normalCDF(double value)
{
    return myErfInv(2*value-1)*M_SQRT2;
}
/** THIS IS STOLEN I DID NOT WRITE THIS^*/

//Returns random value using a normal distribution PDF:
float f_rand_normal(const float scale){
    float a;
    if(b_thresh(0.5)){
        a = GLOBAL_MUT*scale*(0.5-f_uniform());
    }else{a = GLOBAL_MUT*scale*0.1*(0.5-f_uniform());}
    return a;
}

//Returns random sign -1 or 1:
float get_rand_sign(){
    if(b_thresh(0.5)){
        return (0.9+f_uniform()*0.2);
    }
    return -(0.9+f_uniform()*0.2);
}








