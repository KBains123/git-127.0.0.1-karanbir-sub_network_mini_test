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
    void gen_del_weight_gen();
    void gen_del_substrates();
    void allocate_position(const int source_pos);
    void delete_position(const int pos);
    ExtIOGene(const int num_internal,const int num_genes);
};
using ExtIOGenes = std::vector<ExtIOGene>;
void g_allocate_IO_genes(ExtIOGenes& ext_IO_genes,const int source_pos);
void g_erase_IO_genes(ExtIOGenes& ext_IO_genes,const int pos);

class Genome
{
private:
    Internals internal_genes;
    ExtIOGenes external_input_genes;
    ExtIOGenes external_output_genes;
    ExtIOGene& get_rand_ext_gene_list();
public:
    /*Allocates/Deletes linker from
     external IO lists:*/
    void allocate_IO_position(const int source_pos);
    void erase_IO_position(const int pos);
    
    //Mutators:
    void mutate_existing_structures();
    void gen_del_substrates();
    void gen_del_genes();
    void gen_del_weight_gen();
    void gen_del_internal_layer();
    
    Genome(const Genome &G2);
    Genome(const int num_ext_inputs,const int num_ext_outputs,
           const int num_initial_layers,const int num_initial_genes);
};

#endif /* genome_hpp */
