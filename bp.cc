#include "bp.h"

void gshared_bp::config_predictor(std::string bp, int m, int n=0){
    index_num_bits = m;
    bh_num_bits = n;
    index_size = pow(2, index_num_bits); 
    bh_register = 0;
}

void gshared_bp::display(){
    for(unsigned int i=0; i<index_size; i++){
    std::cout << i << "\t" << prediction_table[i] << std::endl;
    }
}

void gshared_bp::addr_to_identifier(unsigned long int addr){

    unsigned long int current_addr = addr;

    current_addr = current_addr/4;
    //std::cout << "Current addr " << current_addr << std::endl;

    pc_index = current_addr%(index_size);
    //std::cout << "Current index: " << pc_index << std::endl;

    current_tag = current_addr/index_size;
    //std::cout << "Current tag: " << current_tag << std::endl;
}

void gshared_bp::prediction_table_init(){
    
    for(unsigned int i=0; i<index_size; i++){
        prediction_table.push_back(2);
    }
}

char gshared_bp::get_prediction(std::string model){
    
    if(model == "gshare"){
        unsigned long int  temp_upper;

        temp_upper = pow(2, index_num_bits-bh_num_bits);
        //std::cout << "temp_upper: " << temp_upper << std::endl;
        //std::cout << "upper_calc: " << ((bh_register ^ (pc_index/temp_upper)) << (index_num_bits-bh_num_bits)) << std::endl;
        //std::cout << "lower_calc: " << (pc_index%temp_upper) << std::endl;
        final_index = ((bh_register ^ (pc_index/temp_upper)) << (index_num_bits-bh_num_bits)) + (pc_index%temp_upper);
        //std::cout << "final_index: " << final_index << std::endl;
    }
    else{
        final_index = pc_index;
    }

    if(prediction_table[final_index] >= 2)
        return 't';
    else
        return 'n';
}

void gshared_bp::update_bh_register(char actual_tn){
    unsigned int temp = pow(2, bh_num_bits-1); 
    
    if(actual_tn == 't'){
        bh_register = bh_register >> 1;
        bh_register = bh_register + temp;
    }
    else {
        bh_register >>= 1;
    }
}

void gshared_bp::update_outcome(char actual_tn){
   
    if(actual_tn == 't')
    (prediction_table[final_index] == 3) ? prediction_table[final_index] = 3 : prediction_table[final_index]++;
    else
    (prediction_table[final_index] == 0) ? prediction_table[final_index] = 0 : prediction_table[final_index]--;
}

void gshared_bp::config_hybrid(int k){

    selector_table_bits = k;
    selector_table_size = pow(2, selector_table_bits);
}

void gshared_bp::selector_table_init(){
    
    for(unsigned int i=0; i<selector_table_size; i++){
        selector_table.push_back(1);
    }
}

void gshared_bp::get_selector_index(unsigned long int addr){
    
    unsigned long int current_addr = addr;

    current_addr = current_addr/4;

    selector_index = current_addr%selector_table_size;
}

int gshared_bp::get_selector_value(){
    
    return selector_table[selector_index];
}

void gshared_bp::update_selector(char actual_tn, char gshared, char bimodal){


    if(actual_tn == gshared && actual_tn == bimodal)
        return;
    else if(actual_tn != gshared && actual_tn != bimodal)
        return;
    else if(actual_tn == gshared && actual_tn != bimodal)
        (selector_table[selector_index] == 3) ? selector_table[selector_index] = 3 : selector_table[selector_index]++;
    else if(actual_tn != gshared && actual_tn == bimodal)
        (selector_table[selector_index] == 0) ? selector_table[selector_index] = 0 : selector_table[selector_index]--;
}

void gshared_bp::display_selector(){

    std::cout << "FINAL CHOOSER CONTENTS" << std::endl;

    for(unsigned int i=0; i<selector_table_size; i++){
    std::cout << i << "\t" << selector_table[i] << std::endl;
    }

}
