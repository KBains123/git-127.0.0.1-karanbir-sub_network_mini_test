//
//  internal_gene.cpp
//  sub_network_test3

#include <assert.h>
#include "internal_gene_list.hpp"
#include "mutation_values.hpp"
#include "Rand_Gen.hpp"

int find_layer(LayerVec& layers,Layer* curr_layer);
void append_super_layer(LayerVec& layers,const int source_pos);
void erase_super_layer(LayerVec& layers,const int del_pos);

///Internals:
void Internals::gen_del_gene()              { get_rand_layer()->gen_del_gene();}
void Internals::mutate_existing_genes()     { get_rand_layer()->mutate_existing_genes();}
void Internals::gen_del_substrates()        { get_rand_layer()->gen_del_substrates();}

void Internals::mutate_existing_linkers()
{
    auto rand_layer = get_rand_layer();
    if(b_thresh(0.5)){
        rand_layer->super_linker.mutate();
    }else{
        rand_layer->sub_linker.mutate();
    }
}
void Internals::gen_del_layers()
{
    auto rand_layer = get_rand_leaf_layer();
    assert(rand_layer->sub_layers.size()==0);
    Layer* parent_layer = rand_layer->parent_layer;
    LayerVec* sub_layer_vec;
    Linker* super_linker;
    
    if(parent_layer){
        sub_layer_vec = &parent_layer->sub_layers;
        super_linker = &parent_layer->super_linker;
    }else{ //If defined within initial layer:
        sub_layer_vec = &initial_layers;
    }
    
    int layer_pos = find_layer(*sub_layer_vec,rand_layer);
    if(b_thresh(0.5)){
        
        //Delete structure:
        sub_layer_vec->erase(sub_layer_vec->begin()+layer_pos);
        erase_super_layer(*sub_layer_vec,layer_pos);
    }else{
        
        //Append structure:
        sub_layer_vec->push_back(*rand_layer);
        append_super_layer(*sub_layer_vec,layer_pos);
    }
    link_all_layers();
}

//Returns layer with no sub layers:
Layer* Internals::get_rand_leaf_layer()
{
    int num_sub_layers{1};
    Layer* rand_layer{};
    while(num_sub_layers>0){
        rand_layer = get_rand_layer();
        num_sub_layers = (int)rand_layer->sub_layers.size();
    }
    return rand_layer;
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
    PLayer_vec[curr_index] = this;
    ++curr_index;
    for(auto &l:sub_layers){
        l.link_to_all_layers(PLayer_vec,curr_index);
    }
    assert(curr_index<=PLayer_vec.size());
}

//Constructors:
Internals::Internals(Genome* _parent_genome,const int num_initial_layers,const int num_initial_genes)
{
    assert(_parent_genome!=nullptr);
    assert(in_range(num_initial_layers,1,INFINITY));
    parent_genome = _parent_genome;
    for(int i=0;i<num_initial_layers;++i){
        initial_layers.push_back(Layer{num_initial_layers,num_initial_genes,i});
    }
    link_all_layers();
}

///Layer:
void Layer::mutate_existing_genes() { get_rand_point(get_rand_genes()).mutate_existing_structures();}
void Layer::gen_del_substrates()    { get_rand_point(get_rand_genes()).gen_del_structures();}
//Generates or deletes random gene:
void Layer::gen_del_gene()
{
    Genes& mut_genes = get_rand_genes();
    assert(mut_genes.size()>0);
}

//Randomly returns either input or output genes:
Genes& Layer::get_rand_genes()
{
    if(b_thresh(0.5)){
        return i_genes;
    }else{
        return o_genes;
    }
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
    WeightGenerator initial_gene{
        0,
        0,
        1,
        initial_substrate,
    };
    i_genes.resize(num_initial_genes,initial_gene);
    o_genes.resize(num_initial_genes,initial_gene);
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
void append_super_layer(LayerVec& layers,const int source_pos)
{
    for(auto &l:layers){
        l.super_linker.append_linker(source_pos);
    }
}
void erase_super_layer(LayerVec& layers,const int del_pos)
{
    for(auto &l:layers){
        l.super_linker.delete_linker(del_pos);
    }
}






