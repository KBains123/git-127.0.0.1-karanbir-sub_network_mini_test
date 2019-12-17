//
//  weight_generator.cpp
//  sub_network_test3

#include "weight_generator.hpp"
#include "mutation_values.hpp"
#include "Rand_Gen.hpp"

#include <iostream>
#include <assert.h>
#include <math.h>

#define MAX_SUBSTRATES  5
void scale_gene_output(WeightGenerator& w);

void WeightGenerator::mutate_existing_structures()
{
    if(b_thresh(WEIGHT_SUBSTRATES)){
        get_rand_point(substrates).mutate();
    }else{
        int mutate_point = get_randint(0, 3);
        switch (mutate_point)
        {
            case 0:
                response_threshold += f_rand_normal(WEIGHT_GEN_THRESH);
                assert(!isnan(response_threshold));
                break;
            case 1:
                response_rate += f_rand_normal(WEIGHT_GEN_RATE);
                clamp(response_rate,0,1);
                assert(!isnan(response_weight));
                assert(in_range(response_rate,0,1));
                break;
            case 2:
                response_weight += f_rand_normal(WEIGHT_GEN_WEIGHT);
                response_weight *= get_rand_sign();
                assert(!isnan(response_weight));
                break;
            default:
                std::cout << "Out of bounds mutation point!\n";
                throw;break;
        }
    }
}

// Generates/Deletes structures:
void WeightGenerator::gen_del_substrates()
{
    int rand_source = get_randint(0,(int)substrates.size());
    if(b_thresh(0.5)){
        if(substrates.size()<MAX_SUBSTRATES){
            substrates.push_back(substrates[rand_source]);
            substrates.back().mutate();
            //Scaling substrate weights:
            for(auto &s:substrates){
                s.scale_out_weight(float(substrates.size()-1)/
                                   substrates.size());
            }
        }
    }else{
        if(substrates.size()>1){
            for(auto &s:substrates){
                //Scaling substrate weights:
                s.scale_out_weight(float(substrates.size())/
                                   (substrates.size()-1));
            }
            substrates.erase(substrates.begin()+rand_source);
        }
    }
    assert(substrates.size()<=MAX_SUBSTRATES);
    assert(substrates.size()>0);
}

void WeightGenerator::zero_input_responses()
{
    for(auto &s:substrates) s.reset_binding();
}

//Returns output with given substrate inputs:
float WeightGenerator::get_generated_weight()
{
    float response{};
    for(auto &s:substrates){
        response += s.get_output();
    }
    return get_scaled_response(response);
}

//Returns response scaled by thresh and build rate:
float WeightGenerator::get_scaled_response(const float r)
{
    assert(!isnan(r));
    
    float sub_thresh = (1-response_threshold)*response_rate;
    float return_val;
    if(r>response_threshold){
        if(r>response_threshold+sub_thresh){
            return_val = 1;
        }else{
            return_val = (r-response_threshold)/sub_thresh;
            
        }
    }else{
        return_val = 0;
    }
    assert(in_range(return_val, 0, 1));
    return_val *= response_weight;
    assert(!isnan(return_val));
    return return_val;
}

WeightGenerator::WeightGenerator(const float _response_thresh,
                                 const float _response_rate,
                                 const float _response_weight,
                                 const Substrate _init_substrate)
{
    assert(in_range(_response_thresh, 0, 1.0));
    
    response_threshold = _response_thresh;
    response_rate = _response_rate;
    response_weight = _response_weight;
    substrates.push_back(_init_substrate);
}

//Returns response weight:
float g_get_weight(WeightGenerator& w1,WeightGenerator& w2)
{
    w1.zero_input_responses();
    w2.zero_input_responses();
    
    //Linking substrates:
    for(auto &s1:w1.substrates){
        for(auto &s2:w2.substrates){
            sim_substrate_interaction(s1, s2);
        }
    }
    float weight1 = w1.get_generated_weight();
    float weight2 = w2.get_generated_weight();
    float return_val = weight1*weight2;
    
    assert(!isnan(return_val));
    assert(in_range(return_val,-1e3, 1e3));
    return return_val;
}

//Scales gene output to %50 of current value:
void scale_gene_output(WeightGenerator& w)
{
    w.response_weight *= 0.5;
    assert(!isnan(w.response_weight));
}
//Mutates random weight within gene:
void g_mutate_gene(Gene& gene)
{
    auto& mut_weight_gen = get_rand_point(gene);
    mut_weight_gen.mutate_existing_structures();
}
/*Randomly deletes or appends a gene*/
void g_gen_del_gene(Genes& genes)
{
    assert(genes.size()>0);
    int curr_gene_size = (int)genes.size();
    int rand_gene = get_randint(0,curr_gene_size);
    if(b_thresh(0.5)){
        
        //Delete gene:
        if(genes.size()>0)
            genes.erase(genes.begin()+rand_gene);
    }else{
        
        //Appends and mutates new gene:
        genes.push_back(genes[rand_gene]);
        
        //Minimising initial mutation effect:
        auto &new_gene = genes.back();
        auto &source_gene = genes[rand_gene];
        std::for_each(new_gene.begin(),new_gene.end(),&scale_gene_output);
        std::for_each(source_gene.begin(),source_gene.end(),&scale_gene_output);
        g_mutate_gene(new_gene);
    }
    assert(genes.size()!=curr_gene_size);
}
void g_gen_del_weight_gen(Genes& genes)
{
    auto& mut_gene = get_rand_point(genes);
    int old_size = (int)mut_gene.size();
    int rand_point = get_randint(0,(int)mut_gene.size());
    if(b_thresh(0.5)){
        if(old_size>1){
            mut_gene.erase(mut_gene.begin()+rand_point);
            assert(mut_gene.size()!=old_size);
        }
    }else{
        auto& source_weight = mut_gene[rand_point];
        mut_gene.push_back(source_weight);
        
        scale_gene_output(source_weight);
        scale_gene_output(mut_gene.back());
        assert(mut_gene.size()!=old_size);
    }
}




