//
//  linker.hpp
//  sub_network_test3

#ifndef linker_hpp
#define linker_hpp

#include <vector>

using FVec = std::vector<float>;

//Scales weights between genes:
class Linker
{
private:
    FVec linkers;
public:
    void append_linker(const int source_pos);
    void delete_linker(const int pos);
    void mutate();
    friend float get_link_weight(Linker& L1, Linker& L2);
    Linker(const int num_layer_pos,const int active_point);
};

struct LinkerPair
{
    Linker output_linker;
    Linker input_linker;
};

#endif /* linker_hpp */

//Returns linking weight between two linkers:
float get_link_weight(Linker& L1, Linker& L2);
