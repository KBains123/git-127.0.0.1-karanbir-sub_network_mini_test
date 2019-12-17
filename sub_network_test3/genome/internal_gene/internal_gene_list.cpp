//
//  internal_gene.cpp
//  sub_network_test3

#include <assert.h>
#include "internal_gene_list.hpp"
#include "mutation_values.hpp"
#include "genome.hpp"
#include "Rand_Gen.hpp"

int find_layer(LayerVec& layers,Layer* curr_layer);
void append_super_linkers(LayerVec& layers,const int source_pos);
void erase_super_linkers(LayerVec& layers,const int del_pos);

///Internals:
void Internals::gen_del_gene()              { get_rand_layer()->gen_del_gene();}
void Internals::mutate_existing_genes()     { get_rand_layer()->mutate_existing_genes();}
void Internals::gen_del_substrates()        { get_rand_layer()->gen_del_substrates();}

void Internals::mutate_existing_linkers()
{
    auto rand_layer = get_rand_layer();
    if(DISABLE_SUPER_LINKER_MUTATION){
        rand_layer->sub_linker.mutate();
    }else{
        if(b_thresh(0.5)){
            rand_layer->super_linker.mutate();
        }else{
            rand_layer->sub_linker.mutate();
        }
    }
}
void Internals::gen_del_weight_gen()
{
    get_rand_layer()->gen_del_weight_gen();
}
void Internals::gen_del_layers()
{
    int rand_layer_point = get_rand_leaf_layer();
    auto rand_layer = all_layers[rand_layer_point];
    assert(rand_layer->sub_layers.size()==0);
    auto parent_layer = rand_layer->parent_layer;
    LayerVec* sub_layer_vec;
    
    if(parent_layer){
        sub_layer_vec = &parent_layer->sub_layers;
    }else{ //If defined within initial layer:
        sub_layer_vec = &initial_layers;
    }
    
    int layer_pos = find_layer(*sub_layer_vec,rand_layer);
    if(b_thresh(0.5)){
        
        //Delete structure:
        sub_layer_vec->erase(sub_layer_vec->begin()+layer_pos);
        erase_super_linkers(*sub_layer_vec,layer_pos);
        
        //Deleting linkers referencing deleted layer:
        erase_parent_layer(parent_layer,layer_pos);
        
        //Deleting linker pointer:
        all_layers.erase(all_layers.begin()+rand_layer_point);
    }else{
        
        //Append structure:
        sub_layer_vec->push_back(*rand_layer);
        append_super_linkers(*sub_layer_vec,layer_pos);
        
        //Creating linkers for new layer:
        allocate_parent_layer(parent_layer,layer_pos);
        
        //Creating linker pointer:
        all_layers.push_back(&sub_layer_vec->back());
    }
    link_all_layers();
}

//Returns layer with no sub layers:
int Internals::get_rand_leaf_layer()
{
    int num_sub_layers{1};
    int rand_layer_index{};
    while(num_sub_layers>0){
        rand_layer_index = get_randint(0,(int)all_layers.size());
        auto rand_layer = all_layers[rand_layer_index];
        num_sub_layers = (int)rand_layer->sub_layers.size();
    }
    return rand_layer_index;
}
Layer* Internals::get_rand_layer()
{
    return get_rand_point(all_layers);
}

//Relinks all internals to objects defined in current:
void Internals::link_all_layers()
{
    int curr_index{};
    for(auto &l:initial_layers){
        l.link_to_all_layers(all_layers,curr_index);
    }
    assert(curr_index==all_layers.size());
}
void Layer::link_to_all_layers(PLayerVec& PLayer_vec,int &curr_index)
{
    assert(curr_index < PLayer_vec.size());
    PLayer_vec[curr_index] = this;
    ++curr_index;
    for(auto &l:sub_layers){
        l.link_to_all_layers(PLayer_vec,curr_index);
    }
}

//Allocates and deletes parent layers during layer deletion/construction:
void Internals::allocate_parent_layer(Layer* parent_layer,const int source_pos)
{
    if(parent_layer){
        parent_layer->sub_linker.append_linker(source_pos);
    }else{ //If defined within initial layer:
        parent_genome->allocate_IO_position(source_pos);
    }
}
void Internals::erase_parent_layer(Layer* parent_layer,const int pos)
{
    if(parent_layer){
        parent_layer->sub_linker.delete_linker(pos);
    }else{ //If defined within initial layer:
        parent_genome->allocate_IO_position(pos);
    }
}

//Constructors:
Internals::Internals(Genome* _parent_genome,const int num_initial_layers,const int num_initial_genes)
{
    assert(_parent_genome!=nullptr);
    assert(in_range(num_initial_layers,1,INFINITY));
    parent_genome = _parent_genome;
    all_layers.resize(num_initial_layers);
    for(int i=0;i<num_initial_layers;++i){
        initial_layers.push_back(Layer{num_initial_layers,num_initial_genes,i});
    }
    link_all_layers();
}
Internals::Internals(const Internals& I2)
{
    initial_layers = I2.initial_layers;
    all_layers = I2.all_layers;
    assert(initial_layers.size()>0);
    assert(all_layers.size()>0);
    link_all_layers();
}

///Layer:
void Layer::mutate_existing_genes() { g_mutate_gene(get_rand_gene()); }
void Layer::gen_del_substrates()    { get_rand_point(get_rand_gene()).gen_del_substrates(); }
void Layer::gen_del_weight_gen()    { g_gen_del_weight_gen(get_rand_IO_genes()); }
//Generates or deletes random gene:
void Layer::gen_del_gene()
{
    Genes& mut_genes = get_rand_IO_genes();
    g_gen_del_gene(mut_genes);
    assert(mut_genes.size()>0);
}

//Randomly returns either input or output genes:
Genes& Layer::get_rand_IO_genes()
{
    if(b_thresh(0.5)){
        return i_genes;
    }else{
        return o_genes;
    }
}
Gene& Layer::get_rand_gene()
{
    return get_rand_point(get_rand_IO_genes());
}

Layer::Layer(const int num_initial_layers,const int num_initial_genes,const int active_pos):
sub_linker(0,0),
super_linker(num_initial_layers,active_pos)
{
    assert(num_initial_layers>0);
    assert(num_initial_genes>0);
    assert(active_pos<num_initial_layers);
    ///Initializing layer genes:
    
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
    i_genes.resize(num_initial_genes,init_gene);
    o_genes.resize(num_initial_genes,init_gene);
}

///Function definitions:
int find_layer(LayerVec& layers,Layer* curr_layer)
{
    int curr_layer_index{};
    while(1){
        if(&layers[curr_layer_index] == curr_layer) break;
        curr_layer_index++;
    }
    assert(in_range(curr_layer_index,0,layers.size()-1));
    return curr_layer_index;
}
void append_super_linkers(LayerVec& layers,const int source_pos)
{
    for(auto &l:layers){
        l.super_linker.append_linker(source_pos);
    }
}
void erase_super_linkers(LayerVec& layers,const int del_pos)
{
    for(auto &l:layers){
        l.super_linker.delete_linker(del_pos);
    }
}





