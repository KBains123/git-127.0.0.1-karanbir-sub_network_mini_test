//
//  internal_gene.hpp
//  sub_network_test3

#ifndef internal_gene_hpp
#define internal_gene_hpp

#include "weight_generator.hpp"
#include "linker.hpp"

//Linker layers are in descending order:
struct LinkPair
{
    LinkVec input_linker;
    LinkVec output_linker;
};

struct InternalGene
{
    //Linkers to super-mid genes:
    LinkPair super_mid_linker;
    //Linkers to sub genes:
    LinkPair sub_linker;
    
    //Output linker
    WeightGenerator output_weights;
    WeightGenerator inputs_weights;
};

#endif /* internal_gene_hpp */
