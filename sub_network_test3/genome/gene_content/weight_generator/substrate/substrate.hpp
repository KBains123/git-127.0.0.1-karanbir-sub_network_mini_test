//
//  substrate.hpp
//  sub_network_test3

#ifndef substrate_hpp
#define substrate_hpp

class Substrate
{
private:
    /* linker parameters determine interactions
     with external linkers */
    float active_point;
    float response_threshold;
    float max_threshold;
    float out_weight;
    
    float binding{};
    /*Updates binding value based upon linker-linker
     interactions: */
    friend void sim_substrate_interaction(Substrate& l1, Substrate& l2);
public:
    /* Mutates linker parameters */
    void mutate();
    void reset_binding()                     {binding = 0;}
    void scale_out_weight(const float scale) {out_weight *= scale;}
    float  get_output();
    
    Substrate(const Substrate &L);
    Substrate(const float _active_point,
              const float _response_threshold,
              const float _max_threshold,
              const float _out_weight);
    Substrate(){}
};

void sim_substrate_interaction(Substrate& l1, Substrate& l2);

#endif /* substrate_hpp */
