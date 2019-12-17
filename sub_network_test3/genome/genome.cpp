//
//  genome.cpp
//  sub_network_test3

#include "genome.hpp"
#include "Rand_Gen.hpp"
#include "mutation_values.hpp"
#include <assert.h>

///ExtIOGene:
void ExtIOGene::mutate_linker()
{
    sub_linker.mutate();
}
void ExtIOGene::mutate_existing_genes()
{
    auto rand_gene = get_rand_point(genes);
    get_rand_point(rand_gene).mutate_existing_structures();
}
void ExtIOGene::gen_del_substrates()
{
    get_rand_point(get_rand_point(genes)).gen_del_substrates();
}
void ExtIOGene::delete_position(const int pos)
{
    sub_linker.delete_linker(pos);
}
void ExtIOGene::allocate_position(const int source_pos)
{
    sub_linker.append_linker(source_pos);
}
ExtIOGene& Genome::get_rand_ext_gene_list()
{
    if(b_thresh(0.5)){
        return get_rand_point(external_output_genes);
    }else{
        return get_rand_point(external_input_genes);
    }
}
ExtIOGene::ExtIOGene(const int num_internal,const int num_genes):
sub_linker(num_internal)
{
    /*
     Initial substrate parameters (ordered):
     
     -active point
     -response threshold
     -max threshold
     -output weight
     */
    Substrate initial_substrate{
        0.5,
        0,
        1,
        1,
    };
    
    /*
     Initial gene parameters (ordered):
     
     -response threshold
     -response rate
     -response weight
     -intial substrate
     */
    WeightGenerator initial_weight_gen{
        0,
        0,
        1,
        initial_substrate,
    };
    auto init_gene = Gene{initial_weight_gen};
    genes.resize(num_genes,init_gene);
}

///Genome:
void Genome::mutate_existing_structures()
{
    if(b_thresh(INTERNAL_V_EXT)){
        if(b_thresh(GENE_V_LINKER)){
            internal_genes.mutate_existing_genes();
        }else{
            internal_genes.mutate_existing_linkers();
        }
    }else{
        
        //Mutate ext IOs:
        if(b_thresh(GENE_V_LINKER)){
            get_rand_ext_gene_list().mutate_existing_genes();
        }else{
            get_rand_ext_gene_list().mutate_linker();
        }
    }
}
void Genome::gen_del_substrates()
{
    if(b_thresh(INTERNAL_V_EXT)){
        internal_genes.gen_del_substrates();
    }else{
        get_rand_ext_gene_list().gen_del_substrates();
    }
}
void Genome::gen_del_weight_gen()
{
    if(b_thresh(INTERNAL_V_EXT)){
        internal_genes.gen_del_weight_gen();
    }else{
        g_gen_del_weight_gen(get_rand_ext_gene_list().genes);
    }
}
void Genome::gen_del_genes()
{
    internal_genes.gen_del_gene();
}
void Genome::gen_del_internal_layer()
{
    internal_genes.gen_del_layers();
}
void Genome::allocate_IO_position(const int source_pos)
{
    g_allocate_IO_genes(external_input_genes,source_pos);
    g_allocate_IO_genes(external_output_genes,source_pos);
}
void Genome::erase_IO_position(const int pos)
{
    g_erase_IO_genes(external_input_genes,pos);
    g_erase_IO_genes(external_output_genes,pos);
}
Genome::Genome(const Genome &G2):
internal_genes{G2.internal_genes},
external_input_genes{G2.external_input_genes},
external_output_genes{G2.external_output_genes}
{
    internal_genes.set_parent(this);
}
Genome::Genome(const int num_ext_inputs,const int num_ext_outputs,
               const int num_initial_layers,const int num_initial_genes):
internal_genes(this,num_initial_layers,num_initial_genes)
{
    auto new_IO_gene = ExtIOGene{num_initial_layers,num_initial_genes};
    external_input_genes = ExtIOGenes(num_ext_inputs,new_IO_gene);
    external_output_genes = ExtIOGenes(num_ext_outputs,new_IO_gene);
}

//Allocates IO space within IO gene vector:
void g_allocate_IO_genes(ExtIOGenes& ext_IO_genes,const int source_pos)
{
    for(auto &gene:ext_IO_genes){
        gene.allocate_position(source_pos);
    }
}
//Erase IO space within IO gene vector:
void g_erase_IO_genes(ExtIOGenes& ext_IO_genes,const int pos)
{
    for(auto &gene:ext_IO_genes){
        gene.delete_position(pos);
    }
}



