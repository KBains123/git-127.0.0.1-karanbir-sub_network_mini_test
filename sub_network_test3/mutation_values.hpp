//
//  mutation_values.hpp
//  sub_network_test3

#ifndef mutation_values_hpp
#define mutation_values_hpp

#define DISABLE_SUPER_LINKER_MUTATION   true

#define GLOBAL_MUT           1.0

#define SUBSTRATE_A_POINT    0.01
#define SUBSTRATE_THRESH     0.001
#define SUBSTRATE_O_THRESH   0.01
#define SUBSTRATE_O_WEIGHT   0.01
#define SUBSTRATE_A_RESET    0.3

#define WEIGHT_GEN_THRESH    0.01
#define WEIGHT_GEN_RATE      0.01
#define WEIGHT_GEN_WEIGHT    0.01
#define WEIGHT_SUBSTRATES    0.5

#define LINKER_VALUE        0.05

#define INTERNAL_V_EXT      0.9
#define GENE_V_LINKER       0.6

inline bool in_range(const float val, const float min, const float max);
inline void clamp(float& val, const float min, const float max);

//Returns true if value is in range of max-min:
inline bool in_range(const float val, const float min, const float max)
{
    return (val <= max) && (val >= min);
}

//Clamps value to min-max range:
inline void clamp(float& val, const float min, const float max)
{
    if(val>max){
        val = max;
        return;
    }
    if(val<min){
        val = min;
        return;
    }
}

#endif /* mutation_values_hpp */

