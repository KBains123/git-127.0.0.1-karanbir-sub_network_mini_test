//
//  weight_generator.hpp
//  sub_network_test3

#ifndef weight_generator_hpp
#define weight_generator_hpp

#include "substrate.hpp"
#include <vector>

using SubVec = std::vector<Substrate>;

class WeightGenerator
{
private:
    SubVec substrates;
    friend float g_get_weight(WeightGenerator& w1,WeightGenerator& w2);
    friend void scale_gene_output(WeightGenerator& w);
    //Parameters determine weight generator response:
    float response_threshold;
    float response_rate;
    float response_weight;
    
    void zero_input_responses();
    float get_generated_weight();
    float get_scaled_response(const float r);
public:
    //Randomly modifies parameters and substrates:
    void mutate_existing_structures();
    // Generates/Deletes structures:
    void gen_del_substrates();
    
    WeightGenerator(const float _response_thresh,
                    const float _response_rate,
                    const float _response_weight,
                    const Substrate _init_substrate);
};

using Gene = std::vector<WeightGenerator>;
using Genes = std::vector<Gene>;
float g_get_weight(WeightGenerator& w1,WeightGenerator& w2);
void g_mutate_gene(Gene& gene);
void g_gen_del_gene(Genes& genes);
void g_gen_del_weight_gen(Genes& genes);
#endif /* weight_generator_hpp */
