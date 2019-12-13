//
//  ext_output_gene.hpp
//  sub_network_test3

#ifndef ext_output_gene_hpp
#define ext_output_gene_hpp

#include "weight_generator.hpp"
#include "linker.hpp"

/*Collects information from
 internal genes*/
struct ExternalOutputGene
{
    LinkVec input_linker;
    WeightGenerator inputs_weights;
};

#endif /* ext_output_gene_hpp */
