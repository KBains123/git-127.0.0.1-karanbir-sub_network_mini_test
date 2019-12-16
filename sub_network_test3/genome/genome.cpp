//
//  genome.cpp
//  sub_network_test3

#include "genome.hpp"
#include "Rand_Gen.hpp"

///ExtIOGene:
void ExtIOGene::mutate_linker()
{
    sub_linker.mutate();
}
void ExtIOGene::mutate_existing_genes()
{
    get_rand_point(genes).mutate_existing_structures();
}
void ExtIOGene::gen_del_genes()
{
    int rand_gene_index = get_randint(0,(int)genes.size());
    if(b_thresh(0.5)){
        
        //Generate new gene:
        genes.
    }else{
        
    }
}
