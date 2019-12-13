//
//  ext_input_gene.hpp
//  sub_network_test3

#ifndef ext_input_gene_hpp
#define ext_input_gene_hpp

#include "weight_generator.hpp"
#include "linker.hpp"

/*Passes information to internal genes:*/
struct ExternalInputGene
{
    LinkVec output_linkers;
    WeightGenerator output_weights;
};

using ExtInputVec = std::vector<ExternalInputGene>;

class ExternalInputGeneList
{
private:
    ExtInputVec
public:
};


#endif /* ext_input_gene_hpp */
