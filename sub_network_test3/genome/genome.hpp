//
//  genome.hpp
//  sub_network_test3

#ifndef genome_hpp
#define genome_hpp

#include "internal_gene_list.hpp"
#include "weight_generator.hpp"
#include "linker.hpp"

//For all external
struct ExtIOGene
{
    Linker sub_linker;
    //Passes inputs to internal genes:
    Genes genes;
    void mutate_linker();
    void mutate_existing_genes();
    void gen_del_genes();
    void gen_del_gene_structures();
};

class Genome
{
private:
    Internals internal_genes;
    ExtIOGene external_input_genes;
    ExtIOGene external_output_gene;
public:
    void mutate_existing_structures();
    void gen_del_gene_structures();
    void gen_del_internal_genes();
    void gen_del_internal_layer();
};

#endif /* genome_hpp */
