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
    int get_rand_leaf_layer();
    Genome *parent_genome{};
    void allocate_parent_layer(Layer* parent_layer,const int source_pos);
    void erase_parent_layer(Layer* parent_layer,const int pos);
public:
    void mutate_existing_genes();
    void mutate_existing_linkers();
    void gen_del_substrates();
    void gen_del_weight_gen();
    void gen_del_gene();
    void gen_del_layers();
    //Relinks all internals to objects defined in current:
    void link_all_layers();
    void set_parent(Genome* _parent_genome) {parent_genome = _parent_genome;}
    Internals(Genome* _parent_genome,const int num_initial_layers,const int num_initial_genes);
    Internals(const Internals& I2);
};

struct Layer
{
    Layer* parent_layer{};
    Linker super_linker;
    Linker sub_linker;
    
    Genes i_genes;
    Genes o_genes;
    LayerVec sub_layers;
    Gene& get_rand_gene();
    Genes& get_rand_IO_genes();
    void link_to_all_layers(PLayerVec& PLayer_vec,int &curr_index);
    void gen_del_gene();
    void gen_del_substrates();
    void gen_del_weight_gen();
    void mutate_existing_genes();
    void mutate_existing_linkers();
    Layer(const int num_initial_layers,
          const int num_initial_genesconst,
          const int active_pos);
};

#endif /* internal_gene_list_hpp */
