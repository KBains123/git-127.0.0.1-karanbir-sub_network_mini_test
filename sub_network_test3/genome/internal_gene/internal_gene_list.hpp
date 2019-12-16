//
//  internal_gene_list.hpp
//  sub_network_test3

#ifndef internal_gene_list_hpp
#define internal_gene_list_hpp

#include "linker.hpp"
#include "weight_generator.hpp"
#include <vector>

struct Layer;
class Genome;
using LayerVec = std::vector<Layer>;
using PLayerVec = std::vector<Layer*>;
class Internals
{
private:
    LayerVec initial_layers;
    //Pointers to all layers in current:
    PLayerVec all_layers;
    Layer* get_rand_layer();
    Layer* get_rand_leaf_layer();
    Genome *parent_genome;
public:
    void mutate_existing_genes();
    void mutate_existing_linkers();
    void gen_del_gene();
    void gen_del_substrates();
    void gen_del_layers();
    //Relinks all internals to objects defined in current:
    void link_all_layers();
    Internals(Genome* _parent_genome,const int num_initial_layerss,const int num_initial_genes);
    Internals(Internals& I2);
};

struct Layer
{
    Layer* parent_layer{};
    Linker super_linker;
    Linker sub_linker;
    
    Genes i_genes;
    Genes o_genes;
    LayerVec sub_layers;
    Genes& get_rand_genes();
    void link_to_all_layers(PLayerVec& PLayer_vec,int &curr_index);
    void gen_del_gene();
    void gen_del_substrates();
    void mutate_existing_genes();
    void mutate_existing_linkers();
    Layer(const int num_initial_layers,
          const int num_initial_genesconst,
          const int active_pos);
};

#endif /* internal_gene_list_hpp */
