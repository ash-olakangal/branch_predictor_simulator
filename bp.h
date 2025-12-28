#ifndef BP_H
#define BP_H

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

class gshared_bp{
    private:
        int index_num_bits;
        int bh_num_bits;
        unsigned long int pc_index;
        unsigned long int current_tag;
        unsigned long int index_size;
        unsigned int bh_register;
        unsigned int final_index;
        unsigned int miss_counter;

        unsigned long int selector_index;
        unsigned long int selector_table_bits;
        unsigned long int selector_table_size;

        std::vector<unsigned int> prediction_table;
        std::vector<unsigned int> selector_table;

    public:
        void config_predictor(std::string, int, int);
        void display();
        void addr_to_identifier(unsigned long int);
        void prediction_table_init();
        char get_prediction(std::string);
        void update_bh_register(char);
        void update_outcome(char);

        void config_hybrid(int);
        void selector_table_init();
        void get_selector_index(unsigned long int);
        int get_selector_value();
        void update_selector(char, char, char);
        void display_selector();
};

#endif
