//
//  Rand_Gen.hpp
//  sub_network_test

#ifndef Rand_Gen_hpp
#define Rand_Gen_hpp

#include <vector>
#include <math.h>

//Returns random integer in range:
int get_randint(const int low,const int high);

//Random number generator: generates value over uniform distribution from 0-1:
float f_uniform();

//Returns true if random value exceeds threshold:
bool b_thresh(const float thresh);

//Returns random value using a normal distribution PDF:
float f_rand_normal(const float scale);

//Returns random point from vector:
template<typename T>
T& get_rand_point(std::vector<T>& vec){
    int p = get_randint(0, (int)vec.size());
    return vec[p];
}

//Returns random sign -1 or 1:
float get_rand_sign();

#endif /* Rand_Gen_hpp */
